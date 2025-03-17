#include "MessageSender.h"

#include "ShowMessageCommandPreparer.h"
#include "CanLargePacketSender.h"

#define DisplayPacketIdentifier			0x6C1

static uint8_t displayBuffer[LargeCanPacket_MaxSupportedSize];

void SendMessage(char* message)
{
	uint8_t packetSize = PrepareMessageCommand(message, displayBuffer, sizeof(displayBuffer), TextModifiers_Default);
	if (packetSize == 0)
	{
		// TODO: handle buffer overflow
		return;
	}
	
	SendLargeCanPacket(DisplayPacketIdentifier, displayBuffer, packetSize);
	// TODO: errors handling
}