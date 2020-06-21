#ifndef PINS_H
#define PINS_H
/** simple pin driver*/

#include "stm8l.h"

/** Returns the port number, 1 is A, 6 is F, 0 is none*/
inline unsigned char pinnum2portnum(unsigned char pinnum){
    const unsigned char portnums[]={0xFF,//0
                                    0xFF,//1
                                    0,//2
                                    0,//3
                                    0xFF,//4
                                    0xFF,//5
                                    0xFF,//6
                                    0,//7
                                    5,//8
                                    1,//9
                                    1,//10
                                    1,//11
                                    1,//12
                                    1,//13
                                    1,//14
                                    1,//15
                                    1,//16
                                    4,//17
                                    2,//18
                                    2,//19
                                    2,//20
                                    2,//21
                                    2,//22
                                    2,//23
                                    2,//24
                                    3,//25
                                    3,//26
                                    3,//27
                                    3,//28
                                    3,//29
                                    3,//30
                                    3,//31
                                    3,//32
                                    };
    return portnums[pinnum];
}

/** Returns the port bit number,0xFF is none*/
inline unsigned char pinnum2bitnum(unsigned char pinnum){
    const unsigned char portnums[]={0xFF,//0
                                    0xFF,//1
                                    1,//2
                                    2,//3
                                    0xFF,//4
                                    0xFF,//5
                                    0xFF,//6
                                    3,//7
                                    4,//8
                                    7,//9
                                    6,//10
                                    5,//11
                                    4,//12
                                    3,//13
                                    2,//14
                                    1,//15
                                    0,//16
                                    5,//17
                                    1,//18
                                    2,//19
                                    3,//20
                                    4,//21
                                    5,//22
                                    6,//23
                                    7,//24
                                    0,//25
                                    1,//26
                                    2,//27
                                    3,//28
                                    4,//29
                                    5,//30
                                    6,//31
                                    7,//32
                                    };
    return portnums[pinnum];
}

inline unsigned char * portnumODR(unsigned char portnum){
    return (unsigned char*)(0x5000 + portnum*5);
}

inline unsigned char * portnumIDR(unsigned char portnum){
    return portnumODR(portnum)+1;
}

inline unsigned char * portnumDDR(unsigned char portnum){
    return portnumODR(portnum)+2;
}

inline unsigned char * portnumCR1(unsigned char portnum){
    return portnumODR(portnum)+3;
}

inline unsigned char * portnumCR2(unsigned char portnum){
    return portnumODR(portnum)+4;
}


#define INPUT               0b000
#define INPUT_PULL_UP       0b010
#define INPUT_INT           0b000
#define INPUT_PULL_UP_INT   0b010

#define OUTPUT_OPEN_DRAIN       0b100
#define OUTPUT                  0b110
#define OUTPUT_OPEN_DRAIN_FAST  0b101
#define OUTPUT_FAST             0b111

/** Set the mode of the pin, pin number is based on LQFP32 pinout */
void pinMode(unsigned char pinnum,unsigned char mode){
    unsigned char portnum= pinnum2portnum(pinnum);
    unsigned char bitnum= pinnum2bitnum(pinnum);
    unsigned char mask= 1<<bitnum;
    *portnumDDR(portnum) = ((*portnumDDR(portnum)) &  ~(mask)) | (((mode & (1<<2))>>2)<<bitnum);
    *portnumCR1(portnum) = ((*portnumCR1(portnum)) &  ~(mask)) | (((mode & (1<<1))>>1)<<bitnum);
    *portnumCR2(portnum) = ((*portnumCR2(portnum)) &  ~(mask)) | (((mode & (1<<0))>>0)<<bitnum);
}

/** toggle the pin, pin number is based on LQFP32 pinout */
void pinToggle(unsigned char pinnum){
    unsigned char portnum= pinnum2portnum(pinnum);
    unsigned char bitnum= pinnum2bitnum(pinnum);
    unsigned char mask= 1<<bitnum;
    *portnumODR(portnum) ^= mask;
}

/** Set the pin, pin number is based on LQFP32 pinout */
void pinSet(unsigned char pinnum){
    unsigned char portnum= pinnum2portnum(pinnum);
    unsigned char bitnum= pinnum2bitnum(pinnum);
    unsigned char mask= 1<<bitnum;
    *portnumODR(portnum) |= mask;
}

/** Clear the pin, pin number is based on LQFP32 pinout */
void pinClear(unsigned char pinnum){
    unsigned char portnum= pinnum2portnum(pinnum);
    unsigned char bitnum= pinnum2bitnum(pinnum);
    unsigned char mask= 1<<bitnum;
    *portnumODR(portnum) &= ~mask;
}

/** read the pin, pin number is based on LQFP32 pinout */
unsigned char pinRead(unsigned char pinnum){
    unsigned char portnum= pinnum2portnum(pinnum);
    unsigned char bitnum= pinnum2bitnum(pinnum);
    unsigned char mask= 1<<bitnum;
    return ((*portnumIDR(portnum)) & mask)>>bitnum;
}

/** delay for loop */
void delayloop(unsigned long count){
    for(unsigned long i=0; i<count; i++){
        __asm__("nop");
    }
}

/** toggle all pins */
void toggleAll(){
    PA_ODR^= 0xFF;
    PB_ODR^= 0xFF;
    PC_ODR^= 0xFF;
    PD_ODR^= 0xFF;
    PE_ODR^= 0xFF;
    PF_ODR^= 0xFF;
}

/** set all pins */
void setAll(){
    PA_ODR = 0xFF;
    PB_ODR = 0xFF;
    PC_ODR = 0xFF;
    PD_ODR = 0xFF;
    PE_ODR = 0xFF;
    PF_ODR = 0xFF;
}

/** clear all pins */
void clearAll(){
    PA_ODR = 0x00;
    PB_ODR = 0x00;
    PC_ODR = 0x00;
    PD_ODR = 0x00;
    PE_ODR = 0x00;
    PF_ODR = 0x00;
}

#endif /* PINS_H */