#pragma once

#include <stdio.h>
#include <stdbool.h>

#define DisplayIdentifier				0x6C1
#define DisplayRxBufferIndex			1
#define DisplayRxBufferFilter			2

void ProcessDisplayRxBuffer();

bool IsMessageTransmissionInProgress();

extern void OnDisplayMessageTransmissionStarted();

extern void OnDisplayMessageTransmissionEnded();
