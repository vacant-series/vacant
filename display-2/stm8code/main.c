//#include <iostm8l152c6.h>
#include <intrinsics.h>
#include "stm8l.h"
#include "pins.h"
#include "rtc.h"
#include "lcd.h"

/* init alarm io pins*/
inline void initalarm(){
    //pull up un connected pins to save power
    pinMode(0,2,INPUT_PULL_UP);
    pinMode(0,3,INPUT_PULL_UP);
    pinMode(0,3,INPUT_PULL_UP);
    
    pinMode(2,0,INPUT_PULL_UP);
    pinMode(2,1,INPUT_PULL_UP);
    pinMode(2,2,INPUT_PULL_UP);
    pinMode(2,3,INPUT_PULL_UP);
    pinMode(2,4,INPUT_PULL_UP);
    pinMode(2,7,INPUT_PULL_UP);
    
    pinMode(5,0,INPUT_PULL_UP);
    
    pinMode(0,7,INPUT_PULL_UP);//External pin
    pinMode(0,6,INPUT_PULL_UP);//CDS
    //pinMode(0,5,INPUT_PULL_UP);//mag1
    pinMode(0,5,INPUT);//mag1
    //pinMode(3,1,INPUT_PULL_UP);//mag2
    pinMode(3,1,INPUT);//mag2
}

/* Returns if alarm is triggered*/
inline unsigned char checkalarm(){
    return pinRead(0,5) && pinRead(3,1);
}




const unsigned char numberlut[]={ 0x7E,//0b1111110,//0
                                0x30,//0b0110000,//1
                                0x6D,//0b1101101,//2
                                0x79,//0b1111001,//3
                                0x33,//0b0110011,//4
                                0x5B,//0b1011011,//5
                                0x5F,//0b1011111,//6
                                0x70,//0b1110000,//7
                                0x7F,//0b1111111,//8
                                0x7B,//0b1111011,//9
                                0x00};//0b0000000};//10 aka off

//lcd seg definitions		a,b,c,d,e,f,g
const unsigned char digit2[]= {3,4,20,21,26,2,1};
const unsigned char digit3[]= {8,9,17,18,19,7,6};
const unsigned char digit4[]= {12,13,14,15,16,11,10};
const unsigned char bc1= 27;
const unsigned char col= 5;

inline void drawnumber(const unsigned char digitsegs[], unsigned char number){
  unsigned char code= numberlut[number];
  for(int i=0; i<7; i++){
    setsegment(digitsegs[i], (code>>(6-i)&1));
  }
}

volatile unsigned char secondsL;
volatile unsigned char secondsH;
volatile unsigned char minutesL;
volatile unsigned char minutesH;
volatile unsigned char hours;
volatile unsigned char hhmm;

inline void inittimer(){
	secondsL=0;
	secondsH=0;
	minutesL=0;
	minutesH=0;
	hours=0;
        hhmm=0;
	//pinSet(col);
	//now set some lcd pins
	//display digit 4
        drawnumber(digit4, 0);
        drawnumber(digit3, 0);
        drawnumber(digit2, 10);
        setsegment(bc1, 0);
        //setsegment(col, 1);
}


inline void inctimer(){
	secondsL++;
	if(secondsL==10){
	       secondsL=0;
	       secondsH++;
	       if(secondsH==6){
		       secondsH=0;
		       minutesL++;
                       hhmm= 1;
		       if(minutesL==10){
			       minutesL=0;
			       minutesH++;
			       if(minutesH==6){
			       		minutesH=0;
					hours++;
					if (hours==20){
						hours=19;
						minutesH=5;
						minutesL=9;
						secondsH=5;
						secondsL=9;
					}
			       }
		       }
	       }
	}
	if(hhmm==0){
	    //display seconds
            drawnumber(digit4, secondsL);
            drawnumber(digit3, secondsH);
	} else {
            //display in HH:MM format
            drawnumber(digit4, minutesL);
            drawnumber(digit3, minutesH);
            if (hours<10){
                drawnumber(digit2, hours);
            } else {
                drawnumber(digit2, hours-10);
                setsegment(bc1, 1);
            }
        }
}



#define RTC_WAKEUP_vector                     6 /* IRQ No. in STM8 manual:  4 */
#pragma vector = RTC_WAKEUP_vector  /* Symbol from I/O */ 
__interrupt void secondISR(void){
  //togglesegment(col);
  if(checkalarm()){
    inittimer();
  } else {
    inctimer();
  }
 /* Clear the periodic wakeup unit flag */
  clearwakeupflag();
}

int main( void ) {
  initrtc();
  initlcd();
  enablesegment(col);
  enablesegment(bc1);
  for(int i=0; i<7; i++){
    enablesegment(digit4[i]);
    enablesegment(digit3[i]);
    enablesegment(digit2[i]);
  }
  drawnumber(digit4, 8);
  drawnumber(digit3, 8);
  drawnumber(digit2, 8);
  setsegment(bc1, 1);
  setsegment(col, 1);
  for(unsigned long i=0; i<500000; i++){ __no_operation();}
  initalarm();
  inittimer();
  initautowakeup(1024);
  //interrupts auto return to halt
  CFG_GCR|= CFG_GCR_AL(1);
  __halt();
  do{
  }while(1);

}

