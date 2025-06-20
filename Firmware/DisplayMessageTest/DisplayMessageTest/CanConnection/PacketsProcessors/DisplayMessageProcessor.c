#include "DisplayMessageProcessor.h"

#include "../MCP/MCP_Transmitter.h"
#include "../LargePacketsTools.h"

static bool isMessageTransmissionInProgress = false;
static uint8_t remainLongPacketsNumber = 0;

typedef struct DisplayMessageMonitoringStatistics {
	uint8_t single : 4;
	uint8_t first : 4;
	uint8_t consecutive;
} DisplayMessageMonitoringStatistics;

// TODO: log somewhere
DisplayMessageMonitoringStatistics statistics = { .single = 0, .first = 0, .consecutive = 0 };

void ProcessDisplayRxBuffer()
{
	LargeCanPacketHeader header;
	MCP_ReadRxBufferData(DisplayRxBufferIndex, sizeof(LargeCanPacketHeader), (uint8_t*)&header);
	
	if (header.FrameType == FrameType_Single)
	{
		if (remainLongPacketsNumber != 0)
		{
			// Wrong Frame: Expected: Consecutive Frame, Received: Single Frame.
			if (statistics.single != 0x0F) statistics.single++;
			remainLongPacketsNumber = 0;
		}
		
		if (isMessageTransmissionInProgress)
		{
			isMessageTransmissionInProgress = false;
			OnDisplayMessageTransmissionEnded();
		}
	}
	else if (header.FrameType == FrameType_First)
	{
		if (remainLongPacketsNumber != 0)
		{
			// Wrong Frame: Expected: Consecutive Frame, Received: First Frame.
			if (statistics.first != 0x0F) statistics.first++;
		}

		if (!isMessageTransmissionInProgress)
		{
			isMessageTransmissionInProgress = true;
			OnDisplayMessageTransmissionStarted();
		}
		
		remainLongPacketsNumber = GetSmallPacketsNumberFromHeader(&header) - 1;
	}
	else if (header.FrameType == FrameType_Consecutive)
	{
		if (remainLongPacketsNumber == 0)
		{
			// Wrong Frame: Expected: Single or First Frame, Received: Consecutive Frame.
			if (statistics.consecutive != 0xFF) statistics.consecutive++;

			// We detected a transmission, but missed the first frame, so we do not know when it will end.
			// We have to indicate that transmission is in progress, but it will be ended only when new transmission is detected and ended.
			if (!isMessageTransmissionInProgress)
			{
				isMessageTransmissionInProgress = true;
				OnDisplayMessageTransmissionStarted();
			}
		}
		else
		{
			if (--remainLongPacketsNumber == 0)
			{
				if (isMessageTransmissionInProgress)
				{
					isMessageTransmissionInProgress = false;
					OnDisplayMessageTransmissionEnded();
				}
			}
		}
	}
}

bool IsMessageTransmissionInProgress()
{
	return isMessageTransmissionInProgress;
}
