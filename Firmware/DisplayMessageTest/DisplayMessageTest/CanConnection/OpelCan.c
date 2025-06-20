#include "OpelCan.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../Log.h"

#include "MCP/SPI.h"
#include "MCP/MCP_Transmitter.h"
#include "MCP/MCP_Control.h"

#include "PacketsProcessors/WheelButtonsProcessor.h"
#include "PacketsProcessors/IgnitionStateProcessor.h"
#include "PacketsProcessors/DisplayMessageProcessor.h"

#define InterruptDDR				DDRB
#define InterruptPORT				PORTB
#define InterruptPIN				PINB
#define InterruptPinNumber			1

#define IsInterruptActive			(!(InterruptPIN & (1 << InterruptPinNumber)))

#define ModeChangeDelayQuantumUs				16
#define ModeChangeWaitQuants					50000

#define GetStandardIdentifierHigh(identifier)		(identifier >> 3)
#define GetStandardIdentifierLow(identifier)		(identifier & 0b111)

#define DisplayTxBufferIndex		0
#define DisplayPacketSize			8

static bool WaitForMode(OperationMode mode)
{
	McpCanStatus status;
	uint16_t timeout = ModeChangeWaitQuants;
	do 
	{
		_delay_us(ModeChangeDelayQuantumUs);
		
		status = MCP_GetStatus();
		if (status.OperationMode == mode)
		{
			return true;
		}
	} while (--timeout != 0);
	
	LogText("Failed to change MCP mode. Target: ");
	LogInt(mode);
	LogText(". Actual: ");
	LogIntLn(status.OperationMode);
	return false;
}

bool OpelCan_Init()
{
	SPI_Init();

	// Initialize MCU's interrupts.
	InterruptDDR &= ~(1 << InterruptPinNumber);
	InterruptPORT |= (1 << InterruptPinNumber);
	
	PCMSK0 = (1 << InterruptPinNumber);
	PCICR |= (1 << PCIE0);
	
	return OpelCan_Reset();
}

bool OpelCan_Reset()
{
	// Start with reset.
	MCP_Reset();
	
	// Ensure that device is connected by checking that it entered config mode after restart.
	if (!WaitForMode(OperationMode_Configuration))
	{
		// TODO: statistics
		return false;
	}
	
	// Configure bit timings.
	// 16 MHz / 95 kHz = 168.421
	// 168 = 14 * 12
	// Prescaler: 12
	// PS2: 0.35 * 14 = 5
	// PS1: 5 with space for lengthening
	// PS: 14 -5 -5 -1 = 3
	// SJW is maximum because 168 != 168.421
	BitTimingConfiguration bitTimingConfig;
	bitTimingConfig.BaudRatePrescalerBits = 12 / 2 - 1;
	bitTimingConfig.PropagationSegmentLengthBits = 3 - 1;
	bitTimingConfig.PS1LengthBits = 5 - 1;
	bitTimingConfig.PS2LengthBits = 5 - 1;
	bitTimingConfig.SynchronizationJumpWidthLengthBits = 4 - 1;
	bitTimingConfig.SampleThreeTimes = false;
	bitTimingConfig.ConfigurePS2Length = true;
	bitTimingConfig.IsWakeUpLowPassFilterEnabled = false;
	bitTimingConfig.IsStartOfFrameSignalEnabled = false;
	MCP_SetBitTiming(&bitTimingConfig);
	
	// Pins have their default values after reset, do not configure them.
	
	// Configure Filters and Masks.
	// Capture only packets with exact standard IDs. The filtering based on data bytes is disabled.
	RxMask exactMatchMask;
	exactMatchMask.StandardIdentifierHigh = 0xFF;
	exactMatchMask.StandardIdentifierLow = 0b111;
	exactMatchMask.ExtendedIdentifierHighest = 0b00;
	exactMatchMask.Data0 = 0x00;
	exactMatchMask.Data1 = 0x00;
	MCP_SetMask(0, &exactMatchMask);
	MCP_SetMask(1, &exactMatchMask);

	// Because RX0 buffer has priority in status, use it for Wheel Buttons and Ignition State packets,
	// so reading RX status will allow to distinguish packets by matched filter number.
	RxFilter filter;
	filter.IsExtendedIdentifier = false;
	filter.StandardIdentifierHigh = GetStandardIdentifierHigh(WheelButtonsIdentifier);
	filter.StandardIdentifierLow = GetStandardIdentifierLow(WheelButtonsIdentifier);
	MCP_SetFilter(WheelButtonsRxBufferFilter, &filter);
	
	filter.StandardIdentifierHigh = GetStandardIdentifierHigh(IgnitionStateIdentifier);
	filter.StandardIdentifierLow = GetStandardIdentifierLow(IgnitionStateIdentifier);
	MCP_SetFilter(IgnitionStateRxBufferFilter, &filter);

	filter.StandardIdentifierHigh = GetStandardIdentifierHigh(DisplayIdentifier);
	filter.StandardIdentifierLow = GetStandardIdentifierLow(DisplayIdentifier);
	MCP_SetFilter(DisplayRxBufferFilter, &filter);

	// Because filters are undefined after reset and mask is used, all unused filters must be specified.
	RxFilter emptyFilter;
	emptyFilter.IsExtendedIdentifier = false;
	emptyFilter.StandardIdentifierHigh = 0x00;
	emptyFilter.StandardIdentifierLow = 0x00;
	MCP_SetFilter(3, &emptyFilter);
	MCP_SetFilter(4, &emptyFilter);
	MCP_SetFilter(5, &emptyFilter);
	
	// Configure receive buffers (roll over and mode).
	Rx0Control rx0 = { .Raw = 0x00 };
	rx0.IsRollOverEnabled = false;
	rx0.FiltersMode = RxFiltersEnabled;
	MCP_SetRx0Control(rx0);
	
	Rx1Control rx1 = { .Raw = 0x00 };
	rx1.FiltersMode = RxFiltersEnabled;
	MCP_SetRx1Control(rx1);
	
	// The TX0 buffer will always be used for sending to display, so its header will always be the same.
	TxBufferHeader header;
	header.IsExtendedIdentifier = false;
	header.IsRemoteRequest = false;
	header.StandardIdentifierHigh = GetStandardIdentifierHigh(DisplayIdentifier);
	header.StandardIdentifierLow = GetStandardIdentifierLow(DisplayIdentifier);
	header.DataLength = DisplayPacketSize;
	MCP_LoadTxBufferHeader(DisplayTxBufferIndex, &header);

	// Configure interrupts.
	InterruptFlags interruptFlags = { .Raw = 0x00 };
	interruptFlags.MessageReceived0 = true;
	interruptFlags.MessageReceived1 = true;
	MCP_SetInterruptEnable(interruptFlags);

	// Start operation.
	McpCanControl canControl = { .Raw = 0x00 };
	canControl.ClkOutPinPrescalerBits = 0x00;
	canControl.EnableClkOutPin = false;
	canControl.OneShotMode = false;
	canControl.AbortPendingTransmits = false;
	canControl.OperationMode = OperationMode_Normal;
	MCP_SetControl(canControl);
	
	// Ensure that operation mode was changed.
	if (!WaitForMode(OperationMode_Normal))
	{
		// TODO: statistics
		return false;
	}
	
	return true;
}

bool OpelCan_Sleep()
{
	// Start operation.
	McpCanControl canControl = { .Raw = 0x00 };
	canControl.ClkOutPinPrescalerBits = 0x00;
	canControl.EnableClkOutPin = false;
	canControl.OneShotMode = false;
	canControl.AbortPendingTransmits = false;
	canControl.OperationMode = OperationMode_Sleep;
	MCP_SetControl(canControl);
	
	// Ensure that operation mode was changed.
	if (!WaitForMode(OperationMode_Sleep))
	{
		// TODO: log statistics
		return false;
	}
	
	return true;
}

void OpelCan_ProcessRxBuffers()
{
	if (IsInterruptActive)
	{
		McpRxStatus rxStatus = MCP_ReadRxStatus();

		if (rxStatus.IsMessageInBuffer0)
		{
			if (rxStatus.FilterMatch == WheelButtonsRxBufferFilter) ProcessWheelButtonsBuffer();
			else ProcessIgnitionStateBuffer();
		}
		
		if (rxStatus.IsMessageInBuffer1)
		{
			ProcessDisplayRxBuffer();
		}
	}
}

void OpelCan_TransmitDisplayPacket(uint8_t* data)
{
	// Ideally, here we should ensure that transmission request is not active, but we control it on another level.
	MCP_LoadTxBufferData(DisplayTxBufferIndex, DisplayPacketSize, data);
	MCP_RequestToSend(RequestToSendBuffer0);
}

bool OpelCan_IsDisplayPacketBufferEmpty()
{
	McpStatus status = MCP_ReadStatus();
	return !status.TransmitRequest0;
}

ISR(PCINT0_vect)
{
	// Actually, we do not need to do anything here. Goal - just to wake up MCU.
	// All work with CAN will be processed in the main application's loop.
}