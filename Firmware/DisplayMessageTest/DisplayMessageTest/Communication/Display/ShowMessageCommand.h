#pragma once

typedef enum MessageDestination {
	MessageDestination_FMSection = 0x01,
	MessageDestination_MainScreenAudio = 0x03,
	MessageDestination_CDSection = 0x05,
	MessageDestination_MainScreenPhone = 0x07,
	MessageDestination_MainScreenSatNav = 0x09,
	MessageDestination_ACSection = 0x0A,
} MessageDestination;

typedef enum MessageType {
	MessageType_SongTitle = 0x10, // Has higher priority in CD mode.
	MessageType_AlbumTitle = 0x11,
	MessageType_Artist = 0x12,
	MessageType_Source = 0x90, // E.g. "Extern In" is sent with this code. Has higher priority in Extern In mode.
} MessageType;

typedef struct ShowMessageCommandHeader {
	// Theoretically, target message destination for devices with multiple sections on the screen.
	MessageDestination Destination;
	
	// A message type like Song Title, Album Name, Artist, etc.
	MessageType Type;
	
	// The length of the message in Unicode symbols. To get size in bytes it should be multiplied by two.
	uint8_t MessageLength;
} ShowMessageCommandHeader;

typedef struct ShowMessageCommandBody {
} ShowMessageCommandBody;

typedef struct ShowMessageCommand {
	ShowMessageCommandHeader Header;
	ShowMessageCommandBody Body;
} ShowMessageCommand;