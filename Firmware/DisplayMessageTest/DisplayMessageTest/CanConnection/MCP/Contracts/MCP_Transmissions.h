#pragma once

typedef enum RequestToSendBuffers {
	RequestToSendBuffer0 = (1 << 0),
	RequestToSendBuffer1 = (1 << 1),
	RequestToSendBuffer2 = (1 << 2),
} RequestToSendBuffers;

typedef union McpStatus {
	struct {
		bool ReceivedInterrupt0 : 1;
		bool ReceivedInterrupt1 : 1;
		bool TransmitRequest0 : 1;
		bool TransmittedInterrupt0 : 1;
		bool TransmitRequest1 : 1;
		bool TransmittedInterrupt1 : 1;
		bool TransmitRequest2 : 1;
		bool TransmittedInterrupt2 : 1;
	};
	uint8_t Raw;
} McpStatus;

typedef union McpRxStatus {
	struct {
		// Note. Indexes 6 and 7 here mean filters 0 and 1 match, but with roll over.
		uint8_t FilterMatch : 3;
		bool IsRemoteTransmissionRequest : 1;
		bool IsExtendedFrame : 1;
		uint8_t : 1;
		bool IsMessageInBuffer0 : 1;
		bool IsMessageInBuffer1 : 1;
	};
	uint8_t Raw;
} McpRxStatus;
