#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>

#include "MessageSender.h"

int main(void)
{
	SendMessage("Read CD");
	//SendMessage("Bluetooth");

    while (1) 
    {
    }
}

void SendCanPacket(uint16_t identifier, uint8_t size, uint8_t* packet)
{
	// TODO: pass errors to caller
}
