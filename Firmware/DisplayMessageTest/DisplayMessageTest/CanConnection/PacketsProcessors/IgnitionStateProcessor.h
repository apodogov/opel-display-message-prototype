#pragma once

#include <stdio.h>
#include <stdbool.h>

#define IgnitionStateIdentifier			0x450
#define IgnitionStateRxBufferIndex		0
#define IgnitionStateRxBufferFilter		1

typedef enum IgnitionStateCode {
	IgnitionStateCode_NoKey = 0x00,
	IgnitionStateCode_Accessories = 0x05,
	IgnitionStateCode_Ignition = 0x06,
} IgnitionStateCode;

typedef struct IgnitionStatePacket {
	// Two bytes are not known and not required.
	uint8_t : 8;
	uint8_t : 8;
	IgnitionStateCode State;
	// We do not use the last byte (Brightness), so just do not read it.
	//uint8_t Brightness;
} IgnitionStatePacket;

void ProcessIgnitionStateBuffer();

extern void OnIgnitionStateChanged(IgnitionStateCode state);
