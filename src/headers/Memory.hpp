#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <stdint.h>

class Memory
{
public:
	virtual ~Memory() = 0;

	virtual uint8_t read(uint16_t) = 0;
	virtual void write(uint16_t, uint8_t) = 0;
protected:
private:
	
};

#endif