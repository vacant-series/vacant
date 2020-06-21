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
const unsigned char digit2[]= {};
const unsigned char digit3[]= {};
const unsigned char digit4[]= {};
const unsigned char bc1= ;
const unsigned char bp= ;
const unsigned char col= ;
const unsigned char alarm= ;


int main() {
	// Configure pins
	pinMode(25,OUTPUT);
	//PD_DDR = 0x01;
	//PD_CR1 = 0x01;
	// Loop
	do {
		pinToggle(25);
		delayloop(29000);
	} while(1);
}
