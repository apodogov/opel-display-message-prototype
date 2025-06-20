#pragma once

// Transmission

typedef union TxControl {
	struct {
		uint8_t Priority : 2;
		uint8_t : 1;
		bool IsTransmitRequested : 1;
		bool WasErrorDetected : 1;
		bool WasArbitrationLost : 1;
		bool WasAborted : 1;
		uint8_t : 1;
	};
	uint8_t Raw;
} TxControl;

typedef struct TxBufferHeader {
	uint8_t StandardIdentifierHigh;
	
	uint8_t ExtendedIdentifierHighest : 2;
	uint8_t : 1;
	bool IsExtendedIdentifier : 1;
	uint8_t : 1;
	uint8_t StandardIdentifierLow : 3;
	
	uint8_t ExtendeddentifierHigh;
	uint8_t ExtendeddentifierLow;
	
	uint8_t DataLength : 4;
	uint8_t : 2;
	bool IsRemoteRequest: 1;
	uint8_t : 1;
} TxBufferHeader;

typedef struct TxBuffer {
	TxBufferHeader Header;
	uint8_t Data[8];
} TxBuffer;

// Reception

typedef enum RxFiltersMode {
	RxFiltersEnabled = 0b00,
	RxFiltersDisabled = 0b11,
} RxFiltersMode;

typedef union Rx0Control {
	struct {
		uint8_t MatchedFilterIndex : 1;
		uint8_t : 1;
		bool IsRollOverEnabled : 1;
		bool IsRemoteRequest: 1;
		uint8_t : 1;
		RxFiltersMode FiltersMode : 2;
		uint8_t : 1;
	};
	uint8_t Raw;
} Rx0Control;

typedef union Rx1Control {
	struct {
		uint8_t MatchedFilterIndex : 3;
		bool IsRemoteRequest: 1;
		uint8_t : 1;
		RxFiltersMode FiltersMode : 2;
		uint8_t : 1;
	};
	uint8_t Raw;
} Rx1Control;

typedef struct RxBufferHeader {
	uint8_t StandardIdentifierHigh;
	
	uint8_t ExtendedIdentifierHighest : 2;
	uint8_t : 1;
	bool IsExtendedIdentifier : 1;
	bool IsStandardRemoteRequestReceived: 1;
	uint8_t StandardIdentifierLow : 3;
	
	uint8_t ExtendeddentifierHigh;
	uint8_t ExtendeddentifierLow;
	
	uint8_t DataLength : 4;
	uint8_t : 2;
	bool IsExtendedRemoteRequestReceived: 1;
	uint8_t : 1;
} RxBufferHeader;

typedef struct RxBuffer {
	RxBufferHeader Header;
	uint8_t Data[8];
} RxBuffer;

// Filtering

typedef struct RxFilter {
	uint8_t StandardIdentifierHigh;

	uint8_t ExtendedIdentifierHighest : 2;
	uint8_t : 1;
	bool IsExtendedIdentifier : 1;
	uint8_t : 1;
	uint8_t StandardIdentifierLow : 3;
	
	uint8_t ExtendeddentifierHigh;
	uint8_t ExtendeddentifierLow;
} RxFilter;

typedef struct RxMask {
	uint8_t StandardIdentifierHigh;

	uint8_t ExtendedIdentifierHighest : 2;
	uint8_t : 3;
	uint8_t StandardIdentifierLow : 3;
	
	union {
		uint8_t ExtendeddentifierHigh;
		uint8_t Data0;
	};
	union {
		uint8_t ExtendeddentifierLow;
		uint8_t Data1;
	};
} RxMask;

// Bit Timing

typedef struct BitTimingConfiguration {
	uint8_t PS2LengthBits : 3;
	uint8_t : 3;
	bool IsWakeUpLowPassFilterEnabled : 1;
	bool IsStartOfFrameSignalEnabled : 1;
	
	uint8_t PropagationSegmentLengthBits : 3;
	uint8_t PS1LengthBits : 3;
	bool SampleThreeTimes : 1;
	bool ConfigurePS2Length : 1;
	
	uint8_t BaudRatePrescalerBits : 6;
	uint8_t SynchronizationJumpWidthLengthBits : 2;
} BitTimingConfiguration;

// Error Detection

typedef union ErrorFlags {
	struct {
		bool ErrorWarn : 1;
		bool RxErrorWarn : 1;
		bool TxErrorWarn : 1;
		bool RxErrorPassive : 1;
		bool TxErrorPassive : 1;
		bool BusOff : 1;
		bool Rx0Overflow : 1;
		bool Rx1Overflow : 1;
	};	
	uint8_t Raw;
} ErrorFlags;

// Interrupts

typedef union InterruptFlags {
	struct {
		bool MessageReceived0 : 1;
		bool MessageReceived1 : 1;
		bool MessageTransmitted0 : 1;
		bool MessageTransmitted1 : 1;
		bool MessageTransmitted2 : 1;
		bool ErrorFlagChange : 1;
		bool WakeOnActivity : 1;
		bool ErrorOnMessage : 1;
	};
	uint8_t Raw;
} InterruptFlags;

// Modes of operation

typedef enum OperationMode {
	OperationMode_Normal        = 0b000,
	OperationMode_Sleep         = 0b001,
	OperationMode_Loopback      = 0b010,
	OperationMode_ListernOnly   = 0b011,
	OperationMode_Configuration = 0b100,
} OperationMode;

typedef enum InterruptSource {
	InterruptSource_NoInterrupt         = 0b000,
	InterruptSource_Error               = 0b001,
	InterruptSource_WakeUp              = 0b010,
	InterruptSource_MessageTransmitted0 = 0b011,
	InterruptSource_MessageTransmitted1 = 0b100,
	InterruptSource_MessageTransmitted2 = 0b101,
	InterruptSource_MessageReceived0    = 0b110,
	InterruptSource_MessageReceived1    = 0b111,
} InterruptSource;

typedef union McpCanControl {
	struct {
		uint8_t ClkOutPinPrescalerBits : 2;
		bool EnableClkOutPin : 1;
		bool OneShotMode : 1;
		bool AbortPendingTransmits : 1;
		OperationMode OperationMode : 3;
	};
	uint8_t Raw;
} McpCanControl;

typedef union McpCanStatus {
	struct {
		uint8_t : 1;
		InterruptSource InterruptSource : 3;
		uint8_t : 1;
		OperationMode OperationMode : 3;
	};
	uint8_t Raw;
} McpCanStatus;
