#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "Contracts/MCP_Models.h"

TxControl MCP_GetTxControl(uint8_t bufferIndex);
void MCP_SetTxControl(uint8_t bufferIndex, TxControl control);

Rx0Control MCP_GetRx0Control();
void MCP_SetRx0Control(Rx0Control control);
Rx1Control MCP_GetRx1Control();
void MCP_SetRx1Control(Rx1Control control);

// Filters [0..1] are associated with RX0 buffer;
// Filters [2..5] are associated with RX1 buffer;
void MCP_SetFilter(uint8_t filterIndex, RxFilter* filter);
void MCP_SetMask(uint8_t maskIndex, RxMask* mask);

void MCP_SetBitTiming(BitTimingConfiguration* bitTimingConfig);

ErrorFlags MCP_GetErrorFlags();
void MCP_ClearErrorFlags(ErrorFlags mask);

void MCP_SetInterruptEnable(InterruptFlags mask);
InterruptFlags MCP_GetInterruptFlags();
void MCP_ClearInterruptFlags(InterruptFlags mask);

McpCanStatus MCP_GetStatus();
void MCP_SetControl(McpCanControl control);

void MCP_AbortPendingTransmissions();

// MCP behavior:
// When new packet is received and buffer it not empty, new packet is discarded.
// When the buffer 1 contains its own packet and new packet matched to buffer 0 filter is received and should be rolled over, the new packet will be discarded.