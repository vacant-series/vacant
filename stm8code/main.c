#include "stm8l.h"
#include "pins.h"

int main() {
	int d;
	// Configure pins
	PD_DDR = 0x01;
	PD_CR1 = 0x01;
	// Loop
	do {
		PD_ODR ^= 0x01;
		for(d = 0; d < 9000; d++) { }
	} while(1);
}
