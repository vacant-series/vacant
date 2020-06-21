#include "stm8l.h"
#include "pins.h"

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
