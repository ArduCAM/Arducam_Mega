#include "spi.h"
#include "definitions.h"                // SYS function prototypes
#include "delay.h"
void spiBegin(){
//dummy 
}
unsigned char spiReadWriteByte(unsigned char val){
    unsigned char read_val;
    SERCOM6_SPI_WriteRead (&val, 1, &read_val, 1);
    return read_val;
} 
void spiCsHigh(unsigned char pin){
    GPIO_PC06_Set();
} 
void spiCsLow(unsigned char pin){
    GPIO_PC06_Clear();
}  //Set the CS pin of SPI to low level
void spiCsOutputMode(unsigned char pin){
    //dummy
}