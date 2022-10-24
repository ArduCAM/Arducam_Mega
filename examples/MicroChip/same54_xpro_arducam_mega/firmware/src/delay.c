#include    "delay.h"
volatile uintptr_t  target_time; 
void timer_callback(TC_COMPARE_STATUS status, volatile uintptr_t  time_cnt){
    target_time ++;
    
}

void delay_init(){
TC0_CompareCallbackRegister(timer_callback,target_time);
TC0_CompareStart();
}
void delay_ms(unsigned int ms){
    target_time = 0;
    while(target_time<= ms*10){
        ;
    } 
}

void delay_us(unsigned int us){
   target_time = 0;
    while(target_time<= us){
        ;
    }
}

