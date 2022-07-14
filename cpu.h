#ifndef chip8
#define chip8
#include <cstdint>
#include <string>

struct Chip8 {
	uint8_t *memory;
	uint8_t *registers;
	uint16_t I;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint16_t PC;
	uint8_t SP;
	uint16_t *stack;
	uint16_t *diplay;
	
	Chip8();
	~Chip8();

	void loadRoam(std::string);
	void loadFont();
	void run();
};
#endif
