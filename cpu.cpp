#include "cpu.h"
#include <fstream>
#include <cstdint>
#include <ios>
#include <string>
#include <iostream>
#include <bitset>

Chip8::Chip8()
{
	memory = new uint8_t[4096];
	registers = new uint8_t[16];
	stack = new uint16_t[16];

	SP = (0x200 & 0xFF);
}

Chip8::~Chip8()
{
	delete [] memory;
	delete [] registers;
	delete [] stack;
}

void Chip8::loadRoam(std::string path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if(file)
	{
		int len = file.tellg();
		file.seekg(0, std::ios::beg);

		char *buffer = new char[len];
		
		file.read(buffer, len);
		file.close();

		for(long i = 0; i < len; ++i)
		{
#ifdef DEBUG
			std::cout << std::bitset<8>(buffer[i]) << std::endl;
#endif
			memory[0x200 + i] = buffer[i];
		}

		delete [] buffer;
	}
}

#include <bitset>

void Chip8::loadFont()
{
	const uint8_t fonts[] = {
#include "font.inc"
	};

	for(int i = 0; i < (sizeof(fonts) / sizeof(*fonts)); i++)
	{
#ifdef DEBUG
		std::cout << std::bitset<8>(*(fonts+i)) << std::endl;
#endif
		memory[0x000 + i] = *(fonts + i);
	}

}

void Chip8::run()
{
	uint16_t instruction = (memory[PC] << 8) | memory[PC + 1];

	uint16_t mask = 0xF000;

	switch((instruction & mask) >> 12)
	{
		case 0:
			switch(instruction & 0x0FFF)
			{
				case 0x00E0:
					std::cout << "clear";
					
					PC += 2;
					break;
				case 0x00EE:
					std::cout << "return from sr";
					
					PC += 2;
					break;
			}
			break;
		case 1:
			PC = instruction & 0x0FFF;
			break;
		case 2:
			++SP;
			stack[SP] = PC;
			PC = instruction & 0x0FFF;
		case 3:
			break;
		case 15:
			switch(instruction & 0x00FF)
			{
				case 0x07:
					std::cout << "hello 0x07";
					PC += 2;
					break;
				case 0x0A:
					std::cout << "0x0A";
					PC += 2;
					break;
				case 0x15:
					std::cout << "0x15";
					PC += 2;
					break;
				case 0x18:
					std::cout << "0x18";
					PC += 2;
					break;
				case 0x1E:
					std::cout << "0x1e";
					PC += 2;
					break;
				case 0x29:
					std::cout << "0x29";
					PC += 2;
					break;
				case 0x33:
					std::cout << "0x33";
					PC += 2;
					break;
				case 0x55:
					std::cout << "0x55";
					PC += 2;
					break;
				case 0x65:
					std::cout << "0x65";
					PC += 2;
					break;
			}
			break;
		break;
	}
}
