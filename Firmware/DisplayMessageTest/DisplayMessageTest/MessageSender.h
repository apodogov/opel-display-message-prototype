#pragma once

#include <stdio.h>
#include <stdbool.h>

void MessageSender_Init();

// Stores message in the buffer for sending to display.
void MessageSender_SetMessage(char* message);

// Activates override of a message on the display.
// When new message is detected on the CAN bus, it will be immediately overwritten by the message that is stored in the buffer.
void MessageSender_ActivateOverride();

// Deactivates override of a message on the display.
void MessageSender_DeactivateOverride();

// Should be called in the main cycle to override message when required.
void MessageSender_ProcessOverride();

// Indicates if message is being sent right now. This means that MCU should not sleep because Timer0 is used for sending.
bool MessageSender_IsSendingNow();
