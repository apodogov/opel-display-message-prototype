#pragma once

#include <stdio.h>
#include <stdbool.h>

void SPI_Init();

void SPI_StartWithCommand(uint8_t command);

uint8_t SPI_TransmitByte(uint8_t b);

void SPI_Stop();