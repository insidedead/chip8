#include "cpu.h"

int main()
{
	Chip8 *u = new Chip8();

	u->loadRoam("/home/insidedead/Downloads/br8kout.ch8");
	u->loadFont();

	int i {0};

	while(i++ != 5)
	{
		u->run();
	}

	delete u;
}
