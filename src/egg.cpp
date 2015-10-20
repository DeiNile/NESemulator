#include <iostream>
#include <stdint.h>

int main() {

	uint8_t a;
	uint16_t b = 122;
	a = b;
	std::cout << (int)a << std::endl;
	b = 257;
	a = b;
	std::cout << (int)a << std::endl;
	b = 300;
	a = b;
	std::cout << (int)a << std::endl;
	return 0;
}