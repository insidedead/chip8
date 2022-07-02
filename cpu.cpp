#include "cpu.h"
#include <cstdint>
#include <fstream>

Chip8::Chip8()
{
	this->mem = new uint8_t[4096];
	this->registers = new uint8_t[16];
	this->stack = new uint16_t[16];
	this->key = new unsigned char[16];

	this->PC = 0x200;
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
		this->mem[0x50 + i] = sprites[i];
	}
}

void Chip8::loadROM(const char *location)
{
	std::ifstream file(location, std::ifstream::binary);

	file.seekg(0, file.end);

	const int size = file.tellg();

	file.seekg(0, file.beg);

	char * buffer =  new char[size];

	file.read(buffer, size);

	file.close();

	for(int i = 0; i < size; ++i)
	{
		this->mem[0x200 + i] = buffer[i];
	}

	delete[] buffer;
}
