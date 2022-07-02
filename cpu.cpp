#include "cpu.h"
#include <cstdint>

Chip8::Chip8()
{
	this->mem = new uint8_t[4096];
	this->registers = new uint8_t[16];
	this->stack = new uint16_t[16];
	this->key = new unsigned char[16];
}

Chip8::~Chip8()
{
	delete [] this->mem;
	delete [] this->registers;
	delete [] this->stack;
	delete [] this->key;
}

void Chip8::loadSprites()
{
	const unsigned short sprites[80] {
	#include "sprites.inc"
	};

	for(unsigned short i = 0; i < 80; i++)
	{
		this->mem[0x000 + i] = sprites[i];
	}
}
