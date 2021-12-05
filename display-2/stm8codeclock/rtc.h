#ifndef RTC_H
#define RTC_H
/** simple RTC driver using a 32.768 LSE*/

#include "stm8l.h"

/** Initialize RTC on LSE*/
void inline initrtc(){
    CLK_CRTCR= CLK_CRTCR_RTCDIV(0) | //ClockRTCprescaler
               CLK_CRTCR_RTCSEL(8); //Configurable RTC clock source selection 2: LSI, 8: LSE
    /* Wait for LSE clock to be ready */
    while((CLK_ECKCR & CLK_ECKCR_LSERDY)==0);
    for(unsigned long i=0; i<200000; i++){
        __no_operation();
    }
    //wait till rtc ready
    while((CLK_CRTCR & CLK_CRTCR_RTCSWBSY)!=0);
    //enable rtc peripheral
    CLK_PCKENR2|= 1<<2;
}

void inline initautowakeup(unsigned long mibbiseconds){
  // unlock write protection
  RTC_WPR= 0xCA;
  RTC_WPR= 0x53;
  // disable wakeup timer and wakeup timer interrupt
  RTC_CR2 = 0x00;
  // wait till WUTWF bit is 1, wut can then be written
  while((RTC_ISR1 & RTC_ISR1_WUTWF)==0);
  //set time
  //select RTCCLK/16 AKA 2048 Hz clock 
  RTC_CR1= RTC_CR1_WUCKSEL(0);
  RTC_WUTRL= (mibbiseconds*2-1) & 0xFF;
  RTC_WUTRH= (mibbiseconds*2-1)>>8;
  //enable count down
  RTC_CR2 |= RTC_CR2_WUTE(1);
  //enable interrupt
  RTC_CR2 |= RTC_CR2_WUTIE(1);
  // lock write protection
  RTC_WPR= 0xFF;
  __enable_interrupt();
}

void inline initautowakeupseconds(unsigned long seconds){
  // unlock write protection
  RTC_WPR= 0xCA;
  RTC_WPR= 0x53;
  RTC_CR2 = 0x00;
  // wait till WUTWF bit is 1, wut can then be written
  while((RTC_ISR1 & RTC_ISR1_WUTWF)==0);
  //set time
  //select 1 Hz clock
  RTC_CR1= RTC_CR1_WUCKSEL(4);
  RTC_WUTRL= (seconds-1) & 0xFF;
  RTC_WUTRH= (seconds-1)>>8;
  //enable count down
  RTC_CR2 |= RTC_CR2_WUTE(1);
  //enable interrupt
  RTC_CR2 |= RTC_CR2_WUTIE(1);
  // lock write protection
  RTC_WPR= 0xFF;
  __enable_interrupt();
}

inline void clearwakeupflag(){
  RTC_ISR2 &= ~(RTC_ISR2_WUTF);
}

inline unsigned char checkwakeupflag(){
    return (RTC_ISR2 & RTC_ISR2_WUTF)>>2;
}

#endif /* RTC_H */