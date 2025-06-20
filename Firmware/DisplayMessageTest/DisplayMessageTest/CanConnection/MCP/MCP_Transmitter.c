#include "MCP_Transmitter.h"

#include "SPI.h"

#define MCP_Reset_Instruction				0xC0
#define MCP_Read_Instruction				0x03
#define MCP_ReadRxBuffer_InstructionMask	0x90
#define MCP_Write_Instruction				0x02
#define MCP_LoadTxBuffer_InstructionMask	0x40
#define MCP_RequestToSend_InstructionMask	0x80
#define MCP_ReadStatus_Instruction			0xA0
#define MCP_ReadRxStatus_Instruction		0xB0
#define MCP_BitModify_Instruction			0x05

#define MCP_ReadRxBuffer_StartPositionBit	1
#define MCP_ReadRxBuffer_BufferIndexBit		2

#define MCP_LoadTxBuffer_StartPositionBit	0
#define MCP_LoadTxBuffer_BufferIndexBit		1

#define MCP_RequestToSend_BufferIndexBit	0

void MCP_Reset()
{
	SPI_StartWithCommand(MCP_Reset_Instruction);
	SPI_Stop();
}

uint8_t MCP_Read(uint8_t address)
{
	SPI_StartWithCommand(MCP_Read_Instruction);
	
	SPI_TransmitByte(address);
	uint8_t val = SPI_TransmitByte(0x00);
	
	SPI_Stop();
	
	return val;
}

void MCP_ReadSequence(uint8_t address, uint8_t length, uint8_t* buffer)
{
	SPI_StartWithCommand(MCP_Read_Instruction);
	
	SPI_TransmitByte(address);
	
	while (length > 0)
	{
		*buffer++ = SPI_TransmitByte(0x00);
		length--;
	}
	
	SPI_Stop();
}

static void MCP_ReadRxBufferSequence(uint8_t command, uint8_t length, uint8_t* buffer)
{
	SPI_StartWithCommand(command);
	
	while (length > 0)
	{
		*buffer++ = SPI_TransmitByte(0x00);
		length--;
	}
	
	SPI_Stop();
}

void MCP_ReadRxBuffer(uint8_t bufferIndex, RxBuffer* buffer)
{
	uint8_t command = MCP_ReadRxBuffer_InstructionMask | (bufferIndex << MCP_ReadRxBuffer_BufferIndexBit);
	MCP_ReadRxBufferSequence(command, sizeof(RxBuffer), (uint8_t*)buffer);
}

void MCP_ReadRxBufferHeader(uint8_t bufferIndex, RxBufferHeader* header)
{
	uint8_t command = MCP_ReadRxBuffer_InstructionMask | (bufferIndex << MCP_ReadRxBuffer_BufferIndexBit);
	MCP_ReadRxBufferSequence(command, sizeof(RxBufferHeader), (uint8_t*)header);
}

void MCP_ReadRxBufferData(uint8_t bufferIndex, uint8_t length, uint8_t* data)
{
	uint8_t command = MCP_ReadRxBuffer_InstructionMask | (1 << MCP_ReadRxBuffer_StartPositionBit) | (bufferIndex << MCP_ReadRxBuffer_BufferIndexBit);
	MCP_ReadRxBufferSequence(command, length, data);
}

void MCP_Write(uint8_t address, uint8_t val)
{
	SPI_StartWithCommand(MCP_Write_Instruction);
	
	SPI_TransmitByte(address);
	SPI_TransmitByte(val);
	
	SPI_Stop();
}

void MCP_WriteSequence(uint8_t address, uint8_t length, uint8_t* buffer)
{
	SPI_StartWithCommand(MCP_Write_Instruction);
	
	SPI_TransmitByte(address);
	
	while (length > 0)
	{
		SPI_TransmitByte(*buffer++);
		length--;
	}
	
	SPI_Stop();
}

static void MCP_LoadTxBufferSequence(uint8_t command, uint8_t length, uint8_t* buffer)
{
	SPI_StartWithCommand(command);
	
	while (length > 0)
	{
		SPI_TransmitByte(*buffer++);
		length--;
	}
	
	SPI_Stop();
}

void MCP_LoadTxBuffer(uint8_t bufferIndex, TxBuffer* buffer)
{
	uint8_t command = MCP_LoadTxBuffer_InstructionMask | (bufferIndex << MCP_LoadTxBuffer_BufferIndexBit);
	MCP_LoadTxBufferSequence(command, sizeof(TxBuffer), (uint8_t*)buffer);
}

void MCP_LoadTxBufferHeader(uint8_t bufferIndex, TxBufferHeader* header)
{
	uint8_t command = MCP_LoadTxBuffer_InstructionMask | (bufferIndex << MCP_LoadTxBuffer_BufferIndexBit);
	MCP_LoadTxBufferSequence(command, sizeof(TxBufferHeader), (uint8_t*)header);
}

void MCP_LoadTxBufferData(uint8_t bufferIndex, uint8_t length, uint8_t* data)
{
	uint8_t command = MCP_LoadTxBuffer_InstructionMask | (1 << MCP_LoadTxBuffer_StartPositionBit) | (bufferIndex << MCP_LoadTxBuffer_BufferIndexBit);
	MCP_LoadTxBufferSequence(command, length, (uint8_t*)data);
}

void MCP_RequestToSend(RequestToSendBuffers buffers)
{
	uint8_t command = MCP_RequestToSend_InstructionMask | (buffers << MCP_RequestToSend_BufferIndexBit);
	SPI_StartWithCommand(command);
	SPI_Stop();
}

McpStatus MCP_ReadStatus()
{
	SPI_StartWithCommand(MCP_ReadStatus_Instruction);
	
	McpStatus status;
	status.Raw = SPI_TransmitByte(0x00);
	
	SPI_Stop();
	
	return status;
}

McpRxStatus MCP_ReadRxStatus()
{
	SPI_StartWithCommand(MCP_ReadRxStatus_Instruction);
	
	McpRxStatus status;
	status.Raw = SPI_TransmitByte(0x00);
	
	SPI_Stop();
	
	return status;
}

void MCP_BitModify(uint8_t address, uint8_t mask, uint8_t val)
{
	SPI_StartWithCommand(MCP_BitModify_Instruction);
	
	SPI_TransmitByte(address);
	SPI_TransmitByte(mask);
	SPI_TransmitByte(val);
	
	SPI_Stop();
}
