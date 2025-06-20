#include "WheelButtonsProcessor.h"

#include "../MCP/MCP_Transmitter.h"

void ProcessWheelButtonsBuffer()
{
	WheelButtonsPacket packet;
	MCP_ReadRxBufferData(WheelButtonsRxBufferIndex, sizeof(WheelButtonsPacket), (uint8_t*)&packet);
	
	if (packet.IsPressed)
	{
		if (packet.PressDuration == 0) OnWheelButtonPressed(packet.Code);
	}
	else
	{
		OnWheelButtonReleased(packet.Code);
	}
}