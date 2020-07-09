#include "stm8l.h"
#include "pins.h"

const unsigned char numberlut[]={0b1111110,//0
							0b0110000,//1
							0b1101101,//2
							0b1111001,//3
							0b0110011,//4
							0b1011011,//5
							0b1011111,//6
							0b1110000,//7
							0b1111111,//8
							0b1111011,//9
							0b0000000};//10 aka off

//pin definitions				a,b,c,d,e,f,g
const unsigned char digit2[]= {31,30,13,10,9,32,2};
const unsigned char digit3[]= {25,24,16,15,14,27,28};
const unsigned char digit4[]= {21,20,19,18,17,22,23};
const unsigned char bc1= 7;
const unsigned char bp= 8;
const unsigned char col= 29;
const unsigned char alarm= 3;

unsigned char secondsL;
unsigned char secondsH;
unsigned char minutesL;
unsigned char minutesH;
unsigned char hours;
unsigned char digit1num;
unsigned char digit2num;
unsigned char digit3num;
unsigned char digit4num;
unsigned char column;
unsigned char alarmcount=0;

/** initializes output regs to display zero*/
void inittimer(){
	secondsL=0;
	secondsH=0;
	minutesL=0;
	minutesH=0;
	hours=0;
	digit1num=0;
	digit2num=10;
	digit3num=0;
	digit4num=0;
	column=0;
	clearAll();
	//pinSet(col);
	//now set some lcd pins
	//display digit 4
	unsigned char zcode= numberlut[0];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit4[i]);
    	unsigned char bitnum= pinnum2bitnum(digit4[i]);
    	*portnumODR(portnum) |= (zcode>>(6-i)&1)<<bitnum;
	}
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit3[i]);
    	unsigned char bitnum= pinnum2bitnum(digit3[i]);
    	*portnumODR(portnum) |= (zcode>>(6-i)&1)<<bitnum;
	}
}

void inctimer(){
	unsigned char old4= digit4num;
	unsigned char old3= digit3num;
	unsigned char old2= digit2num;
	unsigned char old1= digit1num;
	unsigned char oldcolumn= column;

	secondsL++;
	if(secondsL==10){
	       secondsL=0;
	       secondsH++;
	       if(secondsH==6){
		       secondsH=0;
		       minutesL++;
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
	
	if(column==0 && minutesL==1){
		//move to minutes mode.
		column=1;
	}
	if(column==0){
		//seconds mode
		digit4num=secondsL;
		digit3num=secondsH;
	} else {
		//minutes hours mode
		digit4num= minutesL;
		digit3num= minutesH;
		if (hours<10){
			digit2num=hours;
		} else {
			digit1num=1;
			digit2num= hours-10;
		}
	}
		

	
	if(oldcolumn!=column) pinToggle(col);
	if(old1!=digit1num) pinToggle(bc1);
	//now toggle some lcd pins
	//display digit 4
	unsigned char d4code;
	d4code= numberlut[old4];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit4[i]);
    	unsigned char bitnum= pinnum2bitnum(digit4[i]);
    	*portnumODR(portnum) ^= (d4code>>(6-i)&1)<<bitnum;
	}
	//now toggle some lcd pins
	//display digit 4
	d4code= numberlut[digit4num];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit4[i]);
    	unsigned char bitnum= pinnum2bitnum(digit4[i]);
    	*portnumODR(portnum) ^= (d4code>>(6-i)&1)<<bitnum;
	}
	d4code= numberlut[old3];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit3[i]);
    	unsigned char bitnum= pinnum2bitnum(digit3[i]);
    	*portnumODR(portnum) ^= (d4code>>(6-i)&1)<<bitnum;
	}
	//now toggle some lcd pins
	//display digit 3
	d4code= numberlut[digit3num];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit3[i]);
    	unsigned char bitnum= pinnum2bitnum(digit3[i]);
    	*portnumODR(portnum) ^= (d4code>>(6-i)&1)<<bitnum;
	}
	d4code= numberlut[old2];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit2[i]);
    	unsigned char bitnum= pinnum2bitnum(digit2[i]);
    	*portnumODR(portnum) ^= (d4code>>(6-i)&1)<<bitnum;
	}
	//now toggle some lcd pins
	//display digit 2
	d4code= numberlut[digit2num];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit2[i]);
    	unsigned char bitnum= pinnum2bitnum(digit2[i]);
    	*portnumODR(portnum) ^= (d4code>>(6-i)&1)<<bitnum;
	}
}


int main() {
	// Configure pins
	pinMode(25,OUTPUT);
	for( int i=0; i<7; i++){
            pinMode(digit2[i],OUTPUT);
	    pinMode(digit3[i],OUTPUT);
	    pinMode(digit4[i],OUTPUT);
	}
	pinMode(bc1,OUTPUT);
	pinMode(col,OUTPUT);
	pinMode(bp,OUTPUT);
	pinMode(alarm,INPUT_PULL_UP);

	inittimer();
	// Loop
	do {
		for(int i=0; i<82; i++){
			if(1==pinRead(alarm)){
				alarmcount= 0;	
			}else{
				alarmcount++;
			}
			if(alarmcount>5){
				inittimer();
			}
			toggleAll();
			delayloop(1000);
			toggleAll();
			delayloop(1000);
		}
		inctimer();
	} while(1);
}
