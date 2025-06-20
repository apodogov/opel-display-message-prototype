#include "MCP_Control.h"

#include "Contracts/MCP_Registers.h"
#include "MCP_Transmitter.h"

TxControl MCP_GetTxControl(uint8_t bufferIndex)
{
	TxControl control = { .Raw = 0x00 };

	uint8_t reg;
	switch (bufferIndex)
	{
		case 0: reg = TXB0CTRL; break;
		case 1: reg = TXB1CTRL; break;
		case 2: reg = TXB2CTRL; break;
		default: return control;
	}
	
	control.Raw = MCP_Read(reg);
	return control;
}

void MCP_SetTxControl(uint8_t bufferIndex, TxControl control)
{
	uint8_t reg;
	switch (bufferIndex)
	{
		case 0: reg = TXB0CTRL; break;
		case 1: reg = TXB1CTRL; break;
		case 2: reg = TXB2CTRL; break;
		default: return;
	}
	
	MCP_Write(reg, control.Raw);
}

Rx0Control MCP_GetRx0Control()
{
	Rx0Control control;
	control.Raw = MCP_Read(RXB0CTRL);
	return control;
}

void MCP_SetRx0Control(Rx0Control control)
{
	MCP_Write(RXB0CTRL, control.Raw);
}

Rx1Control MCP_GetRx1Control()
{
	Rx1Control control;
	control.Raw = MCP_Read(RXB1CTRL);
	return control;
}

void MCP_SetRx1Control(Rx1Control control)
{
	MCP_Write(RXB1CTRL, control.Raw);
}

void MCP_SetFilter(uint8_t filterIndex, RxFilter* filter)
{
	uint8_t reg;
	switch (filterIndex)
	{
		case 0: reg = RXF0SIDH; break;
		case 1: reg = RXF1SIDH; break;
		case 2: reg = RXF2SIDH; break;
		case 3: reg = RXF3SIDH; break;
		case 4: reg = RXF4SIDH; break;
		case 5: reg = RXF5SIDH; break;
		default: return;
	}

	MCP_WriteSequence(reg, sizeof(RxFilter), (uint8_t*)filter);
}

void MCP_SetMask(uint8_t maskIndex, RxMask* mask)
{
	uint8_t reg;
	switch (maskIndex)
	{
		case 0: reg = RXM0SIDH; break;
		case 1: reg = RXM1SIDH; break;
		default: return;
	}

	MCP_WriteSequence(reg, sizeof(RxMask), (uint8_t*)mask);
}

void MCP_SetBitTiming(BitTimingConfiguration* bitTimingConfig)
{
	MCP_WriteSequence(CNF3, sizeof(BitTimingConfiguration), (uint8_t*)bitTimingConfig);
}

ErrorFlags MCP_GetErrorFlags()
{
	ErrorFlags flags;
	flags.Raw = MCP_Read(EFLG);
	return flags;
}

void MCP_ClearErrorFlags(ErrorFlags mask)
{
	MCP_BitModify(EFLG, mask.Raw, 0x00);
}

void MCP_SetInterruptEnable(InterruptFlags mask)
{
	MCP_Write(CANINTE, mask.Raw);
}

InterruptFlags MCP_GetInterruptFlags()
{
	InterruptFlags flags;
	flags.Raw = MCP_Read(CANINTF);
	return flags;
}

void MCP_ClearInterruptFlags(InterruptFlags mask)
{
	MCP_BitModify(CANINTF, mask.Raw, 0x00);
}

McpCanStatus MCP_GetStatus()
{
	McpCanStatus status;
	status.Raw = MCP_Read(CANSTAT);
	return status;
}

void MCP_SetControl(McpCanControl control)
{
	MCP_Write(CANCTRL, control.Raw);
}

void MCP_AbortPendingTransmissions()
{
	McpCanControl control = { .Raw = 0x00 };
	control.AbortPendingTransmits = true;
	MCP_BitModify(CANCTRL, control.Raw, 0xFF);
}
