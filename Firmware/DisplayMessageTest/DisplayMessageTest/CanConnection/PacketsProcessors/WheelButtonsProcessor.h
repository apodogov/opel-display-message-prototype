#pragma once

#include <stdio.h>
#include <stdbool.h>

#define WheelButtonsIdentifier			0x206
#define WheelButtonsRxBufferIndex		0
#define WheelButtonsRxBufferFilter		0

typedef enum WheelButtonCode {
	WheelButtonCode_VolumeUp = 0x9D,
	WheelButtonCode_VolumeDown = 0x9E,
	WheelButtonCode_Source = 0x9F,
	WheelButtonCode_Previous = 0x8F,
	WheelButtonCode_Next = 0x8E,
	WheelButtonCode_Arrow = 0x90,
} WheelButtonCode;

typedef struct WheelButtonsPacket
{
	bool IsPressed;
	WheelButtonCode Code;
	uint8_t PressDuration;
} WheelButtonsPacket;

void ProcessWheelButtonsBuffer();

extern void OnWheelButtonPressed(WheelButtonCode code);

extern void OnWheelButtonReleased(WheelButtonCode code);
