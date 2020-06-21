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
							0b1111011};//9

//pin definitions
const unsigned char digit2[]= {27,25,16,9,2,29,31};
const unsigned char digit3[]= {24,22,10,7,14,28,30};
const unsigned char digit4[]= {8,17,19,21,23,18,20};
const unsigned char bc1= 13;
const unsigned char bp= 15;
const unsigned char col= 32;
const unsigned char alarm= 3;

unsigned char digit1num;
unsigned char digit2num;
unsigned char digit3num;
unsigned char digit4num;

/** initializes output regs to display zero*/
void inittimer(){
	digit1num=0;
	digit2num=0;
	digit3num=0;
	digit4num=0;
	clearAll();
	pinSet(col);
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
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit2[i]);
    	unsigned char bitnum= pinnum2bitnum(digit2[i]);
    	*portnumODR(portnum) |= (zcode>>(6-i)&1)<<bitnum;
	}
}

void inctimer(){
	unsigned char old4= digit4num;
	digit4num++;
	if(digit4num==10){
	       digit4num=0;
	       digit3num++;
	       if(digit3num==6){
		       digit3num=0;
		       digit2num++;
		       if(digit2num==10){
			       digit2num=0;
			       digit1num=++;
			       if(digit1num=2){
				       digit1num=1;
				       digit2num=9;
				       digit3num=5;
				       digit4num=9;
			       }
		       }
	       }
	}
	//now toggle some lcd pins
	//display digit 4
	unsigned char d4code= numberlut[old4];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit4[i]);
    	unsigned char bitnum= pinnum2bitnum(digit4[i]);
    	*portnumODR(portnum) ^= (d4code>>(6-i)&1)<<bitnum;
	}
	//now toggle some lcd pins
	//display digit 4
	unsigned char d4code= numberlut[digit4num];
	for(int i=0; i<7; i++){
		unsigned char portnum= pinnum2portnum(digit4[i]);
    	unsigned char bitnum= pinnum2bitnum(digit4[i]);
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
		toggleAll();
		delayloop(29000);
	} while(1);
}
