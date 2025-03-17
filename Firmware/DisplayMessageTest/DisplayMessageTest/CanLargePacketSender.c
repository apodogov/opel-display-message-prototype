#include "CanLargePacketSender.h"

void SendLargeCanPacket(uint16_t identifier, uint8_t* buffer, uint8_t size)
{
	uint8_t packetBuffer[CanPacketSize];
	uint8_t position = 0;
	if (size <= 7)
	{
		packetBuffer[0] = size;
		for (uint8_t i = 1; i < CanPacketSize; i++)
		{
			if (i <= size) packetBuffer[i] = buffer[position++];
			else packetBuffer[i] = 0x00;
		}
		
		SendCanPacket(identifier, CanPacketSize, packetBuffer);
		return;
	}
	
	packetBuffer[0] = 0x10;
	packetBuffer[1] = size;
	for (uint8_t i = 2; i < CanPacketSize; i++)
	{
		packetBuffer[i] = buffer[position++];
	}
	
	size -= LargeCanPacket_FirstFramePayloadSize;
	SendCanPacket(identifier, CanPacketSize, packetBuffer);
	
	uint8_t consecutiveFrameIndex = 1;
	while (size > 0)
	{
		packetBuffer[0] = 0x20 | consecutiveFrameIndex++;
		for (uint8_t i = 1; i < CanPacketSize; i++)
		{
			if (size > 0)
			{
				packetBuffer[i] = buffer[position++];
				size--;
			}
			else packetBuffer[i] = 0x00;
		}
	
		SendCanPacket(identifier, CanPacketSize, packetBuffer);
	}
}