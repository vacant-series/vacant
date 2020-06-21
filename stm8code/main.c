#include "stm8l.h"
#include "pins.h"

int main() {
	int d;
	// Configure pins
	PE_DDR = 0x01;
	PE_CR1 = 0x01;
	// Loop
	do {
		PE_ODR ^= 0x01;
		for(d = 0; d < 29000; d++) { }
	} while(1);
}
