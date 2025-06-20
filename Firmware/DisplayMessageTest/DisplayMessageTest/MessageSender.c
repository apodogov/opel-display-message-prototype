#include "MessageSender.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Log.h"

#include "Communication/Display/ShowMessageCommandPreparer.h"
#include "CanConnection/PacketsProcessors/DisplayMessageProcessor.h"
#include "CanConnection/OpelCan.h"
#include "CanConnection/LargePacketsTools.h"

// Timer's period is 4 ms. Experimentally found that too fast override is ignored.
#define FirstPacketTransmissionDelay		(8 / 4)

static uint8_t displayBuffer[LargeCanPacket_MaxSupportedSize];
static uint8_t totalSize = 0;
static uint8_t totalPacketsNumber = 0;

static bool isOverrideActive = false; // Indicates if sending of a message after others' message is active.
static uint8_t firstPacketTransmissionDelay = 0; // Indicates a delay between others' message and the first packet.
static bool isSendingRightNow = false; // Indicates if sending process is started. Matched to the timer's activity. MCU should not sleep when this flag is set.
static bool doSendPacket = false; // Flag to send packet in the main loop.

static uint8_t packetIndexToSend;

static inline void StartTimer()
{
	LogLine("Send Now");

	isSendingRightNow = true;
	TCCR0B = (1 << CS02); // Enable Timer with Prescaler /256
}

static inline void StopTimer()
{
	LogLine("Send End");

	isSendingRightNow = false;
	TCCR0B = 0x00;
	
	// Just in case if timer triggered the packet before it was stopped.
	doSendPacket = false;
}

static void StartTransmission()
{
	// The first packet should be send with some delay. Otherwise, Display ignores it.
	firstPacketTransmissionDelay = FirstPacketTransmissionDelay;
	packetIndexToSend = 0;
	StartTimer();
}

void MessageSender_Init()
{
	// Timer0 is used exclusively for display message transmissions.
	// One 8-byte packet is being sent approximately 1 ms. Also, experientially found
	// that display need at least 300 us delay between packets. So, initially configured timer to 2 ms with a little buffer.
	// But later found that sometimes it is not enough, so increased it to 4 ms.
	// System Clock: 16 MHz. Target: 4 ms (250 Hz)
	// /1 /256 /250 /1 = 4 ms (250 Hz)
	TCCR0A = (1 << WGM01); // CTC, TOP = OCRA
	OCR0A = 250 -1; // Timer Divider
	TIMSK0 = (1 << OCIE0A); // Compare match Interrupt
}

void MessageSender_SetMessage(char* message)
{
	totalSize = PrepareMessageCommand(message, displayBuffer, sizeof(displayBuffer), TextModifiers_Default);
	if (totalSize == 0)
	{
		totalPacketsNumber = 0;
		return;
	}
	
	totalPacketsNumber = GetSmallPacketsNumber(totalSize);

	LogText("Buffer size: ");
	LogIntLn(totalSize);
	LogText("Packets number: ");
	LogIntLn(totalPacketsNumber);
}

void MessageSender_ActivateOverride()
{
	LogLine("Override activated");
	isOverrideActive = true;
	StartTransmission();
}

void MessageSender_DeactivateOverride()
{
	LogLine("Override deactivated");

	// Just indicate that no more transmissions should be started.
	// Active ones should not be interrupted.
	isOverrideActive = false;
}

void MessageSender_ProcessOverride()
{
	if (doSendPacket)
	{
		doSendPacket = false;
		
		if (IsMessageTransmissionInProgress())
		{
			// Somebody other started message transmission. Stop ours. We'll send in on the event when other ends transmission.
			// TODO: statistics?
			LogLine("Message Interrupted");
			StopTimer();
		}
		else
		{
			if (!OpelCan_IsDisplayPacketBufferEmpty())
			{
				// Previous packet was not sent yet. Try later.
				// TODO: log statistics
				LogLine("Buffer is not empty");
				return;
			}
			
			if (packetIndexToSend == totalPacketsNumber)
			{
				// All packets were sent and buffer is empty now, so the timer can be stopped.
				StopTimer();
				return;
			}

			uint8_t packetBuffer[8];
			FillSmallPacketBuffer(packetIndexToSend, (uint8_t*)&displayBuffer, totalSize, (uint8_t*)&packetBuffer);
			OpelCan_TransmitDisplayPacket((uint8_t*)&packetBuffer);

			packetIndexToSend++;
		}
	}
}

bool MessageSender_IsSendingNow()
{
	return isSendingRightNow;
}

void OnDisplayMessageTransmissionStarted()
{
	LogLine("Message start");
}

void OnDisplayMessageTransmissionEnded()
{
	LogLine("Message end");

	// Other device just ended message transmission.
	// If override is enabled, then it is time to transmit our message.
	if (isOverrideActive)
	{
		StartTransmission();
	}
}

ISR(TIMER0_COMPA_vect)
{
	if (firstPacketTransmissionDelay)
	{
		if (--firstPacketTransmissionDelay == 0)
		{
			// Delay passed - start the transmission.
			doSendPacket = true;
		}
	}
	else
	{
		// If there are no delay, send packets on each timer's event.
		doSendPacket = true;
	}
}