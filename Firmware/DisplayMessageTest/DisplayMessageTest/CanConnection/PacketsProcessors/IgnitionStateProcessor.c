#include "IgnitionStateProcessor.h"

#include "../MCP/MCP_Transmitter.h"

static IgnitionStateCode lastState = IgnitionStateCode_NoKey;

void ProcessIgnitionStateBuffer()
{
	IgnitionStatePacket packet;
	MCP_ReadRxBufferData(IgnitionStateRxBufferIndex, sizeof(IgnitionStatePacket), (uint8_t*)&packet);
	
	if (packet.State != lastState)
	{
		lastState = packet.State;
		OnIgnitionStateChanged(packet.State);
	}
}
