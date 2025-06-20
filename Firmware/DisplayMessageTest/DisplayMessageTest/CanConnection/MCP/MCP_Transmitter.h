#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "Contracts/MCP_Models.h"
#include "Contracts/MCP_Transmissions.h"

// Please note, some delay is required between reset and the next command is sent (approx. 128 clock cycles).
void MCP_Reset();

uint8_t MCP_Read(uint8_t address);
void MCP_ReadSequence(uint8_t address, uint8_t length, uint8_t* buffer);

// This instruction clears the associated receive flag, RXnIF (CANINTF), when CS is raised at the end of the command.
void MCP_ReadRxBuffer(uint8_t bufferIndex, RxBuffer* buffer);
// This instruction clears the associated receive flag, RXnIF (CANINTF), when CS is raised at the end of the command.
// So, it won't be a good idea to read header and data separately by these commands. Use this one only when you need only header!
void MCP_ReadRxBufferHeader(uint8_t bufferIndex, RxBufferHeader* header);
// This instruction clears the associated receive flag, RXnIF (CANINTF), when CS is raised at the end of the command.
// So, it won't be a good idea to read header and data separately by these commands. Use this one only when you need only data!
void MCP_ReadRxBufferData(uint8_t bufferIndex, uint8_t length, uint8_t* data);

void MCP_Write(uint8_t address, uint8_t val);
void MCP_WriteSequence(uint8_t address, uint8_t length, uint8_t* buffer);

void MCP_LoadTxBuffer(uint8_t bufferIndex, TxBuffer* buffer);
void MCP_LoadTxBufferHeader(uint8_t bufferIndex, TxBufferHeader* header);
void MCP_LoadTxBufferData(uint8_t bufferIndex, uint8_t length, uint8_t* data);

void MCP_RequestToSend(RequestToSendBuffers buffers);
McpStatus MCP_ReadStatus();
McpRxStatus MCP_ReadRxStatus();

void MCP_BitModify(uint8_t address, uint8_t mask, uint8_t val);
