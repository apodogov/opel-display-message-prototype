#include "ShowMessageCommandPreparer.h"

#include "DisplayCommand.h"
#include "ShowMessageCommand.h"

static uint8_t* position;
static uint8_t messageSize;
static uint8_t freeSize;
static bool wasOverflowed;

static void AddUnicodeSymbol(uint8_t asciiCode)
{
	// Because we support only ASCII symbols, for simplification, instead of supporting Unicode, just prefix each symbol by 0x00.
	if (freeSize < 2)
	{
		wasOverflowed = true;
		return;
	}
	
	*position++ = 0x00;
	*position++ = asciiCode;
	
	messageSize += 2;
	freeSize -= 2;
}

static void AddString(char* s)
{
	while (*s) AddUnicodeSymbol(*s++);
}

static void AddModifierString(char* s)
{
	AddUnicodeSymbol(0x1B);
	AddUnicodeSymbol('[');
	AddString(s);
	AddUnicodeSymbol('m');
}

static void AddModifiers(TextModifiers modifiers)
{
	if (modifiers & TextModifiers_Center) AddModifierString("c");
	if (modifiers & TextModifiers_Small) AddModifierString("fS_d");
	if (modifiers & TextModifiers_Normal) AddModifierString("fS_g");
}

uint8_t PrepareMessageCommand(char* message, uint8_t* displayBuffer, uint8_t bufferSize, TextModifiers modifiers)
{
	DisplayCommand* command = (DisplayCommand*)displayBuffer;
	command->Header.CommandType = DisplayCommandType_ShowMessage;
	
	ShowMessageCommand* showMessageCommand = (ShowMessageCommand*)&command->Body;
	showMessageCommand->Header.Destination = MessageDestination_MainScreenAudio;
	showMessageCommand->Header.Type = MessageType_SongTitle;
	
	position = (uint8_t*)&showMessageCommand->Body;
	messageSize = 0;
	freeSize = bufferSize - sizeof(DisplayCommandHeader) - sizeof(ShowMessageCommandHeader);
	wasOverflowed = false;
	
	AddModifiers(modifiers);
	AddString(message);
	AddUnicodeSymbol(0x00);

	if (wasOverflowed) return 0;
	
	showMessageCommand->Header.MessageLength = messageSize / 2;
	command->Header.CommandSize = sizeof(ShowMessageCommandHeader) + messageSize;
	
	uint8_t wholeCommandPacketSize = sizeof(DisplayCommandHeader) + sizeof(ShowMessageCommandHeader) + messageSize;
	return wholeCommandPacketSize;
}
