#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef enum TextModifiers {
	TextModifiers_None = 0x00,
	
	TextModifiers_Normal = 0x01,
	TextModifiers_Small = 0x02,
	TextModifiers_Center = 0x04,
	
	TextModifiers_Default = TextModifiers_Center | TextModifiers_Normal,
} TextModifiers;

// Returns size of the whole packet that was prepared and should be sent. If buffer was overflowed, returns zero.
uint8_t PrepareMessageCommand(char* message, uint8_t* displayBuffer, uint8_t bufferSize, TextModifiers modifiers);
