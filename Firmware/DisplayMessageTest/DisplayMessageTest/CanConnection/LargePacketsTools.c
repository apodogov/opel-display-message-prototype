#include "LargePacketsTools.h"

uint8_t GetSmallPacketsNumber(uint16_t largePacketSize)
{
	if (largePacketSize <= LargeCanPacket_SingleFramePayloadSize)
	{
		return 1;
	}
	
	uint8_t packetsNumber = 1;
	largePacketSize -= LargeCanPacket_FirstFramePayloadSize;
		
	packetsNumber += largePacketSize / LargeCanPacket_ConsecutiveFramePayloadSize;
	if (largePacketSize % LargeCanPacket_ConsecutiveFramePayloadSize != 0) packetsNumber++;
		
	return packetsNumber;
}

uint8_t GetSmallPacketsNumberFromHeader(LargeCanPacketHeader* largePacketHeader)
{
	if (largePacketHeader->FrameType == FrameType_Single)
	{
		return 1;
	}
	
	if (largePacketHeader->FrameType != FrameType_First)
	{
		// Can't get transmission size from a consecutive packet.
		return 0;
	}
	
	uint16_t length = ((uint16_t)largePacketHeader->First.LengthHigh << 8) | largePacketHeader->First.LengthLow;
	return GetSmallPacketsNumber(length);
}

static uint8_t GetPacketStartIndex(uint8_t packetIndex)
{
	if (packetIndex == 0) return 0;
	
	uint8_t consecutivePacketIndex = packetIndex - 1;
	uint16_t indexInBuffer = LargeCanPacket_FirstFramePayloadSize + (consecutivePacketIndex * LargeCanPacket_ConsecutiveFramePayloadSize);
	return indexInBuffer;
}

void FillSmallPacketBuffer(uint8_t packetIndex, uint8_t* largePacketBuffer, uint16_t largePacketSize, uint8_t* smallPacketBuffer)
{
	uint8_t startDataIndex;
	if (packetIndex == 0)
	{
		FirstFrameCanPacketHeader* header = (FirstFrameCanPacketHeader*)smallPacketBuffer;
		header->FrameType = FrameType_First;
		header->LengthHigh = largePacketSize >> 8;
		header->LengthLow = largePacketSize & 0xFF;
		startDataIndex = sizeof(FirstFrameCanPacketHeader);
	}
	else
	{
		ConsecutiveFrameCanPacketHeader* header = (ConsecutiveFrameCanPacketHeader*)smallPacketBuffer;
		header->FrameType = FrameType_Consecutive;
		header->FrameIndex = packetIndex;
		startDataIndex = sizeof(ConsecutiveFrameCanPacketHeader);
	}

	uint8_t position = GetPacketStartIndex(packetIndex);
	for (uint8_t i = startDataIndex; i < CanPacketSize; i++)
	{
		smallPacketBuffer[i] = (position < largePacketSize) ? largePacketBuffer[position++] : 0x00;
	}
}
