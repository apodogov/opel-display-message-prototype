// Splits large packet according to https://en.wikipedia.org/wiki/ISO_15765-2
// Supports only 15 consecutive frames.

#pragma once

#include <stdio.h>
#include <stdbool.h>

#define CanPacketSize									8

#define LargeCanPacket_SingleFramePayloadSize			(CanPacketSize - sizeof(SingleFrameCanPacketHeader))
#define LargeCanPacket_FirstFramePayloadSize			(CanPacketSize - sizeof(FirstFrameCanPacketHeader))
#define LargeCanPacket_ConsecutiveFramePayloadSize		(CanPacketSize - sizeof(ConsecutiveFrameCanPacketHeader))

#define LargeCanPacket_MaxSupportedConsecutiveFrames	15
#define LargeCanPacket_MaxSupportedSize					(LargeCanPacket_FirstFramePayloadSize + (LargeCanPacket_ConsecutiveFramePayloadSize * LargeCanPacket_MaxSupportedConsecutiveFrames))

typedef enum LargeCanPacketFrameType
{
	FrameType_Single = 0x00,
	FrameType_First = 0x01,
	FrameType_Consecutive = 0x02,
} LargeCanPacketFrameType;

typedef struct SingleFrameCanPacketHeader {
	uint8_t Length : 4;
	LargeCanPacketFrameType FrameType : 4;
} SingleFrameCanPacketHeader;

typedef struct FirstFrameCanPacketHeader {
	uint8_t LengthHigh : 4;
	LargeCanPacketFrameType FrameType : 4;
	uint8_t LengthLow;
} FirstFrameCanPacketHeader;

typedef struct ConsecutiveFrameCanPacketHeader {
	uint8_t FrameIndex : 4;
	LargeCanPacketFrameType FrameType : 4;
} ConsecutiveFrameCanPacketHeader;

typedef struct LargeCanPacketHeader {
	union {
		SingleFrameCanPacketHeader Single;
		FirstFrameCanPacketHeader First;
		ConsecutiveFrameCanPacketHeader Consecutive;
		struct {
			uint8_t : 4;
			LargeCanPacketFrameType FrameType : 4;
		};
	};
} LargeCanPacketHeader;

uint8_t GetSmallPacketsNumber(uint16_t largePacketSize);

uint8_t GetSmallPacketsNumberFromHeader(LargeCanPacketHeader* largePacketHeader);

void FillSmallPacketBuffer(uint8_t packetIndex, uint8_t* largePacketBuffer, uint16_t largePacketSize, uint8_t* smallPacketBuffer);
