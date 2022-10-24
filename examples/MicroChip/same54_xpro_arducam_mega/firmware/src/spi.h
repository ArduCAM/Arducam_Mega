#ifndef __SPI_H
#define __SPI_H
void spiBegin();
unsigned char spiReadWriteByte(unsigned char val);    //  SPI communication sends a byte
void spiCsHigh(unsigned char pin);    //Set the CS pin of SPI to high level
void spiCsLow(unsigned char pin);  //Set the CS pin of SPI to low level
void spiCsOutputMode(unsigned char pin);



#endif 