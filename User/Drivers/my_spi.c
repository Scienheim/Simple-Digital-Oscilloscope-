#include "my_spi.h"
#include "spi.h"

/*****************************************************************************
 * @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
 * @date       :2018-08-09
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
                Byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte)
{
	while((SPIx->SR&SPI_FLAG_TXE)==RESET);		//等待发送区空
	SPIx->DR=Byte;	 	//发送一个byte
	for ( uint8_t i = 0; i < 254; ++i ) {
		if ((SPIx->SR&SPI_FLAG_RXNE)!=RESET )
			return SPIx->DR;          	     //返回收到的数据
	}
	return 0xff;
}
//u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
//{
//	HAL_SPI_Transmit(&hspi1,&Byte,1,10);
//	return SPIx->DR;          	     //返回收到的数据			
//}

/*****************************************************************************
 * @name       :void SPI2_Init(void)	
 * @date       :2018-08-09 
 * @function   :Initialize the STM32 hardware SPI2
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void SPI_Init(void)	
{
	__HAL_SPI_ENABLE(&hspi1);
}
