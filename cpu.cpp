#include "cpu.h"
#include <fstream>
#include <cstdint>
#include <ios>
#include <string>
#include <iostream>
#include <bitset>
#include <cstring>

Chip8::Chip8()
{
	memory = new uint8_t[4096];
	registers = new uint8_t[16];
	stack = new uint16_t[16];
	display = new uint16_t[64 * 32];

	SP = (0x200 & 0xFF);

	draw = false;
}

Chip8::~Chip8()
{
	delete [] memory;
	delete [] registers;
	delete [] stack;
	delete [] display;
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
					memset (display, 0, sizeof(display));
					PC += 2;
					break;
				case 0x00EE:
					PC = stack[SP];
					--SP;
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
			if(registers[(instruction & 0x0F00) >> 8] == (instruction & 0x00FF)) PC += 2;
			PC += 2;
			break;
		case 4:
			if(registers[(instruction & 0x0F00) >> 8] != (instruction & 0x00FF)) PC += 2;
			PC += 2;
			break;
		case 5:
			if(registers[(instruction & 0x0F00) >> 8] == registers[(instruction & 0x00F0) >> 4]) PC += 2;
			PC += 2;
			break;
		case 6:
			registers[(instruction & 0x0F00) >> 4] = (instruction & 0x00FF);
			PC += 2;
			break;
		case 7:
			registers[(instruction & 0x0F00) >> 8] = registers[(instruction & 0x0F00) >> 8] + (instruction & 0x00FF);
			PC += 2;
			break;
		case 8:
			switch(instruction & 0x000F)
			{
				case 0:
					registers[(instruction & 0x0F00) >> 8] = registers[(instruction & 0x00F0) >> 4];
					PC += 2;
					break;
				case 1:
					registers[(instruction & 0x0F00) >> 8] |= registers[(instruction & 0x00F0) >> 4];
					PC += 2;
					break;
				case 2:
					registers[(instruction & 0x0F00) >> 8] &= registers[(instruction & 0x00F0) >> 4];
					break;
				case 3:
					registers[(instruction & 0x0F00) >> 8] ^= registers[(instruction & 0x00F0) >> 4];
					break;
				case 4:
					registers[0xF] = registers[(instruction & 0x0F00) >> 8] > 255 ? 1 : 0;
					registers[(instruction & 0x0F00) >> 8] += registers[(instruction & 0x000F0) >> 4];
					break;
				case 5:
					registers[(instruction & 0x0F00) >> 8] -= registers[(instruction & 0x000F0) >> 4];
					registers[0xF] = registers[(instruction & 0x00F0) >> 4] > registers[(instruction & 0x0F00) >> 8] ? 1 : 0;
					break;
				case 6:
					registers[0xF] = registers[(instruction & 0x0F00) >> 8] & 1 ? 1 : 0;
					registers[(instruction & 0x0F00) >> 8] /= 2;
					break;
				case 7:
					registers[0xF] = registers[(instruction & 0x00F0) >> 4] > registers[(instruction & 0x0F00) >> 8] ? 1 : 0;
					registers[(instruction & 0x0F00) >> 8] /= registers[(instruction & 0x00F0) >> 4];
					break;
				case 14:
					registers[0xF] = registers[(instruction & 0x0F00) >> 8] & 1 ? 1 : 0;
					registers[(instruction & 0x0F00) >> 8] *= 2;
					break;
			}
			break;
		case 9:
			PC += registers[(instruction & 0x0F00) >> 8] != registers[(instruction & 0x00F0) >> 4] ? 4 : 2;
			break;
		case 10:
			I = instruction & 0x0FFF; 
			PC += 2;
			break;
		case 11:
			PC = registers[instruction & 0x0FFF] + registers[0x0];
			break;
		case 12:
#include <cstdlib>
			uint8_t random;
			random = rand() % 256;
			registers[(instruction & 0x0F00) >> 8] = random & (instruction & 0x00FF);
			break;
		case 13:
			draw = true;
			for(int i = I; I < (instruction & 0x000F); I++)
			{
				display[(instruction & 0x0F00) >> 8] = display[(instruction & 0x00F0) >> 4] ^= memory[I];
			}
			draw = false;
			break;
		case 15:
			switch((instruction & 0xF))
			{
				case 7:
					registers[(instruction & 0x0F00) >> 8] = delayTimer;
					PC += 2;
					break;
				case 10:
					std::cout << "key press";
					PC += 2;
					break;
				case 5:
					switch((instruction & 0x00F0) >> 4)
					{
						case 1:
							delayTimer = registers[(instruction & 0x0F00) >> 8];
							PC += 2;
							break;
						case 5:
							for(int i = 0; i < ((instruction & 0x00F00) >> 8); i++)
							{
								memory[I + i] = registers[i];
							}
							PC += 2;
							break;
						case 6:
							for(int i = 0; i < ((instruction & 0x00F00) >> 8); i++)
							{
								registers[i] = memory[I + i];
							}
							PC += 2;
							break;
					}
				case 8:
					soundTimer = registers[(instruction & 0x0F00) >> 8];
					PC += 2;
					break;
				case 14:
					I += registers[(instruction & 0x0F00) >> 8];
					PC += 2;
					break;
				case 9:
					PC += 2;
					break;
				case 3:
					PC += 2;
					break;
			}
		default:
			std::cerr << "wrong code " << std::bitset<16>(instruction) << std::endl;
			PC += 2;
			break;
	}
}
