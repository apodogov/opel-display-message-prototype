// Splits large packet according to https://en.wikipedia.org/wiki/ISO_15765-2
// Supports only 15 consecutive frames.

#pragma once

#include <stdio.h>
#include <stdbool.h>

#define CanPacketSize									8

#define LargeCanPacket_FirstFrameControlSize			2
#define LargeCanPacket_ConsecutiveFrameControlSize		1
#define LargeCanPacket_MaxSupportedConsecutiveFrames	15

#define LargeCanPacket_FirstFramePayloadSize			(CanPacketSize - LargeCanPacket_FirstFrameControlSize)
#define LargeCanPacket_ConsecutiveFramePayloadSize		(CanPacketSize - LargeCanPacket_ConsecutiveFrameControlSize)

#define LargeCanPacket_MaxSupportedSize					(LargeCanPacket_FirstFramePayloadSize + (LargeCanPacket_ConsecutiveFramePayloadSize * LargeCanPacket_MaxSupportedConsecutiveFrames))

void SendLargeCanPacket(uint16_t identifier, uint8_t* buffer, uint8_t size);

extern void SendCanPacket(uint16_t identifier, uint8_t size, uint8_t* packet);
