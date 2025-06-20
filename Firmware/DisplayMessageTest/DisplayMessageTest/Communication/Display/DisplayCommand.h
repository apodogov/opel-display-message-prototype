#pragma once

typedef enum DisplayCommandType {
	//DisplayCommandType_ShowMessage = 0x40,
	DisplayCommandType_ShowMessage = 0xC0,
} DisplayCommandType;

typedef struct DisplayCommandHeader {
	DisplayCommandType CommandType;
	uint8_t Reserved;
	uint8_t CommandSize;
} DisplayCommandHeader;

typedef struct DisplayCommandBody {
} DisplayCommandBody;

typedef struct DisplayCommand {
	DisplayCommandHeader Header;
	DisplayCommandBody Body;
} DisplayCommand;