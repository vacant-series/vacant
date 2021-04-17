#ifndef PINS_H
#define PINS_H
/** simple pin driver*/

#include "stm8l.h"

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


#define INPUT               0x00
#define INPUT_PULL_UP       0x02
#define INPUT_INT           0x00
#define INPUT_PULL_UP_INT   0x02

#define OUTPUT_OPEN_DRAIN       0x04
#define OUTPUT                  0x06
#define OUTPUT_OPEN_DRAIN_FAST  0x05
#define OUTPUT_FAST             0x07

/** Set the mode of the pin */
void inline pinMode(unsigned char portnum,unsigned char bitnum,unsigned char mode){
    unsigned char mask= 1<<bitnum;
    *portnumDDR(portnum) = ((*portnumDDR(portnum)) &  ~(mask)) | (((mode & (1<<2))>>2)<<bitnum);
    *portnumCR1(portnum) = ((*portnumCR1(portnum)) &  ~(mask)) | (((mode & (1<<1))>>1)<<bitnum);
    *portnumCR2(portnum) = ((*portnumCR2(portnum)) &  ~(mask)) | (((mode & (1<<0))>>0)<<bitnum);
}

/** toggle the pin*/
void inline pinToggle(unsigned char portnum,unsigned char bitnum){
    unsigned char mask= 1<<bitnum;
    *portnumODR(portnum) ^= mask;
}

/** Set the pin*/
void inline pinSet(unsigned char portnum,unsigned char bitnum){
    unsigned char mask= 1<<bitnum;
    *portnumODR(portnum) |= mask;
}

/** Clear the pin*/
void inline pinClear(unsigned char portnum,unsigned char bitnum){
    unsigned char mask= 1<<bitnum;
    *portnumODR(portnum) &= ~mask;
}

/** read the pin*/
inline unsigned char pinRead(unsigned char portnum,unsigned char bitnum){
    unsigned char mask= 1<<bitnum;
    return ((*portnumIDR(portnum)) & mask)>>bitnum;
}

/** delay for loop */
void inline delayloop(unsigned long count){
    for(unsigned long i=0; i<count; i++){
//        __asm__("nop");
    }
}

/** toggle all pins */
inline void toggleAll(){
    PA_ODR^= 0xFF;
    PB_ODR^= 0xFF;
    PC_ODR^= 0xFF;
    PD_ODR^= 0xFF;
    PE_ODR^= 0xFF;
    PF_ODR^= 0xFF;
}

/** set all pins */
inline void setAll(){
    PA_ODR = 0xFF;
    PB_ODR = 0xFF;
    PC_ODR = 0xFF;
    PD_ODR = 0xFF;
    PE_ODR = 0xFF;
    PF_ODR = 0xFF;
}

/** clear all pins */
inline void clearAll(){
    PA_ODR = 0x00;
    PB_ODR = 0x00;
    PC_ODR = 0x00;
    PD_ODR = 0x00;
    PE_ODR = 0x00;
    PF_ODR = 0x00;
}

#endif /* PINS_H */
