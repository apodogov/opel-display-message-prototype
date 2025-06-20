#include "SPI.h"

#include <avr/io.h>

#define SPI_DDR				DDRB
#define SPI_PORT			PORTB

#define SPI_SCK_PIN			5
#define SPI_MISO_PIN		4
#define SPI_MOSI_PIN		3
#define SPI_SS_PIN			2

#define SPI_OutputPinsMask	((1 << SPI_SCK_PIN) | (1 << SPI_MOSI_PIN) | (1 << SPI_SS_PIN))
#define SPI_InputPinsMask	(1 << SPI_MISO_PIN)

void SPI_Init()
{
	// SS pin is low when active.
	SPI_PORT = (SPI_PORT & ~(SPI_InputPinsMask | SPI_OutputPinsMask)) | (1 << SPI_SS_PIN);
	SPI_DDR = (SPI_DDR & ~SPI_InputPinsMask) | SPI_OutputPinsMask;
	
	// Master, MSB first, CPOL = 0, CPHA = 0, F_OSC/2
	SPSR = (1 << SPI2X);
	SPCR = (1 << SPE) | (1 << MSTR);
}

void SPI_StartWithCommand(uint8_t command)
{
	SPI_PORT &= ~(1 << SPI_SS_PIN);
	SPI_TransmitByte(command);
}

uint8_t SPI_TransmitByte(uint8_t b)
{
	SPDR = b;
	while (!(SPSR & (1 << SPIF))) ;
	return SPDR;
}

void SPI_Stop()
{
	SPI_PORT |= (1 << SPI_SS_PIN);
}
