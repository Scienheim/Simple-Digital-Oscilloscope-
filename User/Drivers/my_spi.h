#include "main.h"

#ifndef _SPI_H_
#define _SPI_H_

 
uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte);
void SPI_Init(void);

#endif
