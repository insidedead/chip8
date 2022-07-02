// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0
#ifndef INCLUDE_CHIP8_H
#define INCLUDE_CHIP8_H

#include <cstdint>

struct Chip8 {
	uint8_t *mem;
	uint8_t *registers;
	uint16_t I;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint16_t PC;
	uint8_t SP;
	uint16_t *stack;
	unsigned char *key;

	Chip8();
	~Chip8();

	void loadSprites();
};

#endif
