#ifndef LCD_H
#define LCD_H
/** simple static LCD driver*/

#include "stm8l.h"

/** Initialize static LCD
 * Initialize rtc before this.
*/
void inline initlcd(){
    //clock gating
    CLK_PCKENR2|= 1<<3;
    LCD_CR2= LCD_CR2_VSEL(1) | //V_LCD source 0: Internal 1: External
             LCD_CR2_CC(0); //set V_LCD if VSEL is 0
    LCD_CR3= LCD_CR3_LCDEN(1); // LCD enable
    LCD_FRQ= LCD_FRQ_PS(5) | LCD_FRQ_DIV(0); //increase to slow frame rate and use less power
}

/** Enable segment on LCD*/
void inline enablesegment(unsigned char seg){
    unsigned char bitnum= seg & 7;
    LCD_PM[seg>>3] |= 1<<bitnum;
}

/** Disable segment on LCD*/
void inline disablesegment(unsigned char seg){
    unsigned char bitnum= seg & 7;
    LCD_PM[seg>>3] &= ~(1<<bitnum);
}

/** Turn on segment on LCD*/
void inline turnonsegment(unsigned char seg){
    unsigned char bitnum= seg & 7;
    LCD_RAM[seg>>3] |= 1<<bitnum;
}

/** Turn off segment on LCD*/
void inline turnoffsegment(unsigned char seg){
    unsigned char bitnum= seg & 7;
    LCD_RAM[seg>>3] &= ~(1<<bitnum);
}

/** Toggle segment on LCD*/
void inline togglesegment(unsigned char seg){
    unsigned char bitnum= seg & 7;
    LCD_RAM[seg>>3] ^= 1<<bitnum;
}

/** set segment on LCD. val should be 1 for on, 0 for off*/
void inline setsegment(unsigned char seg, unsigned char val){
  val ? turnonsegment(seg) : turnoffsegment(seg);
}

#endif /* LCD_H */