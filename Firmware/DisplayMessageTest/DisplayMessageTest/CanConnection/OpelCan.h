#pragma once

#include <stdio.h>
#include <stdbool.h>

// Returns false when mode can't be changed. This might mean that there is no connection to MCP module.
bool OpelCan_Init();
bool OpelCan_Reset();
bool OpelCan_Sleep();

void OpelCan_ProcessRxBuffers();

void OpelCan_TransmitDisplayPacket(uint8_t* data);

bool OpelCan_IsDisplayPacketBufferEmpty();
