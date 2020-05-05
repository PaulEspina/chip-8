#include "Chip.h"

Chip::Chip()
{
	Init();
}

Chip::Chip(const char *path)
	: Chip()
{
	Readrom(path);
}

void Chip::Readrom(const char *path)
{
	std::ifstream file(path, std::ifstream::binary | std::ifstream::in);
	file.seekg(0, std::ios::end);
	std::streamoff length = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> result((unsigned int) length);
	file.read(&result[0], length);
	for(unsigned int i = 0, size = result.size(), mem = 0x200; i < size; i++, mem++)
	{
		memory[mem] = (int) result[i];
	}
}

void Chip::Restart()
{
	Init();
}

void Chip::Init()
{
	srand((unsigned int) time(0));
	address = 0;
	pc = 0x200;
	memset(registers, 0, sizeof(registers));
	memset(memory, 0, sizeof(memory));
	memset(keys, 0, sizeof(keys));
	delay = 0;
	sound = 0;
}

void Chip::Cycle()
{
	unsigned short int instruction = Fetch();
	Execute(instruction);
}

void Chip::KeyPressed(int key)
{
	keys[key] = 1;
}

void Chip::KeyReleased(int key)
{
	keys[key] = 0;
}

int Chip::CheckIfKeyIsPressed()
{
	for(int i = 0; i < 16; i++)
	{
		if(keys[i] != 0)
			return i;
	}
	return -1;
}

unsigned short int Chip::Fetch()
{
	unsigned short int next = memory[pc];
	next <<= 8;
	next |= memory[pc + 1];
	pc += 2;
	return next; 
}

void Chip::Execute(unsigned short int instruction)
{
	switch(instruction & 0xf000)
	{
	case 0x0000:
		switch(instruction & 0x000f)
		{
		case 0x0000:
			Op00E0(instruction);
			break;
		case 0x0001:
			Op00EE(instruction);
			break;
		}
		break;
	case 0x1000:
		Op1NNN(instruction);
		break;
	case 0x2000:
		Op2NNN(instruction);
		break;
	case 0x3000:
		Op3XNN(instruction);
		break;
	case 0x4000:
		Op4XNN(instruction);
		break;
	case 0x5000:
		Op5XY0(instruction);
		break;
	case 0x6000:
		Op6XNN(instruction);
		break;
	case 0x7000:
		Op7XNN(instruction);
		break;
	case 0x8000:
		switch(instruction & 0x000f)
		{
		case 0x0000:
			Op8XY0(instruction);
			break;
		case 0x0001:
			Op8XY1(instruction);
			break;
		case 0x0002:
			Op8XY2(instruction);
			break;
		case 0x0003:
			Op8XY3(instruction);
			break;
		case 0x0004:
			Op8XY4(instruction);
			break;
		case 0x0005:
			Op8XY5(instruction);
			break;
		case 0x0006:
			Op8XY6(instruction);
			break;
		case 0x0007:
			Op8XY7(instruction);
			break;
		case 0x000e:
			Op8XYE(instruction);
			break;
		}
		break;
	case 0x9000:
		Op9XY0(instruction);
		break;
	case 0xa000:
		OpANNN(instruction);
		break;
	case 0xb000:
		OpBNNN(instruction);
		break;
	case 0xc000:
		OpCXNN(instruction);
		break;
	case 0xd000:
		OpDXYN(instruction);
		break;
	case 0xe000:
		switch(instruction & 0x000f)
		{
		case 0x000e:
			OpEX9E(instruction);
			break;
		case 0x0001:
			OpEXA1(instruction);
			break;
		}
		break;
	case 0xf000:
		switch(instruction & 0x00f0)
		{
		case 0x0000:
			switch(instruction & 0x000f)
			{
			case 0x0007:
				OpFX07(instruction);
				break;
			case 0x000a:
				OpFX0A(instruction);
				break;
			}
			break;
		case 0x0010:
			switch(instruction & 0x000f)
			{
			case 0x0005:
				OpFX15(instruction);
				break;
			case 0x0008:
				OpFX18(instruction);
				break;
			case 0x000e:
				OpFX1E(instruction);
				break;
			}
			break;
		case 0x0020:
			OpFX29(instruction);
			break;
		case 0x0030:
			OpFX33(instruction);
			break;
		case 0x0050:
			OpFX55(instruction);
			break;
		case 0x0060:
			OpFX65(instruction);
			break;
		}
		break;
	}
}

// Clears the screen
void Chip::Op00E0(unsigned short int instruction)
{
	for(unsigned int i = 0; i < 64; i++)
	{
		for(unsigned int j = 0; j < 32; j++)
		{
			graphics[j][i] = 0;
		}
	}
}

// Retursn from a subroutine
void Chip::Op00EE(unsigned short int instruction)
{
	pc = stack.back();
	stack.pop_back();
}

// Jumps to address NNN
void Chip::Op1NNN(unsigned short int instruction)
{
	pc = instruction & 0x0fff;
}

// Calls subroutine at NNN
void Chip::Op2NNN(unsigned short int instruction)
{
	stack.push_back(pc);
	pc = instruction & 0x0fff;
}

// Skips the next instruction if VX equals NN
void Chip::Op3XNN(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int nn = instruction & 0x00ff;
	if(registers[x] == nn)
	{
		pc += 2;
	}
}

// Skips the next instruction if VX not equal NN
void Chip::Op4XNN(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int nn = instruction & 0x00ff;
	if(registers[x] != nn)
	{
		pc += 2;
	}
}

// Skips the next insutrction if VX equal VY
void Chip::Op5XY0(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	if(registers[x] == registers[y])
	{
		pc += 2;
	}
}

// Sets VX to NN
void Chip::Op6XNN(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int nn = instruction & 0x00ff;
	registers[x] = nn;
}

// Adds NN to VX
void Chip::Op7XNN(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int nn = instruction & 0x00ff;
	registers[x] += nn;
}

// Sets VX to the value of VY
void Chip::Op8XY0(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	registers[x] = registers[y];
}

// Sets VX to VX or VY
void Chip::Op8XY1(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	registers[x] = registers[x] | registers[y];
}
 // Sets VX to VX and VY
void Chip::Op8XY2(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	registers[x] = registers[x] & registers[y];
}

// Sets VX to VX xor VY
void Chip::Op8XY3(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	registers[x] = registers[x] ^ registers[y];
}

// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
void Chip::Op8XY4(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	unsigned short int temp = registers[x] + registers[y];
	if(temp > 255)
	{
		registers[0xf] = 1;
	}
	else
	{
		registers[0xf] = 0;
	}
	registers[x] = registers[x] + registers[y];
}
 // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void Chip::Op8XY5(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	if(registers[x] < registers[y])
	{
		registers[0xf] = 0;
	}
	else
	{
		registers[0xf] = 1;
	}
	registers[x] = registers[x] - registers[y];
}

// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
void Chip::Op8XY6(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	registers[0xf] = registers[x] & 0x1;
	registers[x] >>= 1;
}

// 	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
void Chip::Op8XY7(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	if(registers[y] < registers[x])
	{
		registers[0xf] = 0;
	}
	else
	{
		registers[0xf] = 1;
	}
	registers[x] = registers[y] - registers[x];
}

// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
void Chip::Op8XYE(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	registers[0xf] = registers[x] >> 7;
	registers[x] <<= 1;
}

// Skips the next instruction if VX doesn't equal VY
void Chip::Op9XY0(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int y = instruction & 0x00f0;
	y >>= 4;
	if(registers[x] != registers[y])
	{
		pc += 2;
	}
}

// Sets I to the address NNN
void Chip::OpANNN(unsigned short int instruction)
{
	address = instruction & 0x0fff;
}

// Jumps to the address NNN plus V0
void Chip::OpBNNN(unsigned short int instruction)
{
	int nnn = instruction & 0x0fff;
	pc = registers[0] + nnn;
}

// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
void Chip::OpCXNN(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int nn = instruction & 0x00ff;
	registers[x] = rand() & nn;
}

/*
Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. 
As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
*/
void Chip::OpDXYN(unsigned short int instruction)
{
	int regx = instruction & 0x0f00;
	regx >>= 8;
	int regy = instruction & 0x00f0;
	regy >>= 4;
	int n = instruction & 0x000f;
	int x = registers[regx];
	int y = registers[regy];
	registers[0xf] = 0;
	for(unsigned int i = 0; i < n; i++)
	{
		int data = memory[address + i];
		for(unsigned int j = 0; j < 8; j++)
		{
			if(data & (1 << (7 - j)))
			{
				int xcoord = x + j;
				int ycoord = y + i;
				if(graphics[ycoord][xcoord] == 0)
				{
					graphics[ycoord][xcoord] = 0xff;
					registers[0xf] = 1;
				}
				else
				{
					graphics[ycoord][xcoord] = 0;
				}
			}
		}
	}
}

 // Skips the next instruction if the key stored in VX is pressed.
void Chip::OpEX9E(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int key = registers[x];
	if(keys[key] == 1)
	{
		pc += 2;
	}
}

// Skips the next instruction if the key stored in VX isn't pressed.
void Chip::OpEXA1(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int key = registers[x];
	if(!keys[key])
	{
		pc += 2;
	}
}
// Sets VX to the value of the delay timer.
void Chip::OpFX07(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	registers[x] = delay;
}

// A key press is awaited, and then stored in VX
void Chip::OpFX0A(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int key = CheckIfKeyIsPressed();
	if(key == -1)
	{
		pc -= 2;
	}
	else
	{
		registers[x] = key;
	}
}

// Sets the delay timer to VX
void Chip::OpFX15(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	delay = registers[x];
}

void Chip::OpFX18(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	sound = registers[x];
}

// Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
void Chip::OpFX1E(unsigned short int instruction)
{
	int x = instruction & 0x0F00;
	x >>= 8;
	address += registers[x];
}

// Sets I to the location of the sprite for the character in VX
void Chip::OpFX29(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	address = registers[x] * 5;
}

/*
Stores the binary-coded decimal representation of VX, 
with the most significant of three digits at the address in I, 
the middle digit at I plus 1, and the least significant digit at I plus 2.
*/
void Chip::OpFX33(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;
	int value = registers[x];
	int hundreds = value / 100;
	int tens = (value / 10) % 10;
	int units = value % 10;
	memory[address] = hundreds;
	memory[address + 1] = tens;
	memory[address + 2] = units;
}

/*
Stores V0 to VX (including VX) in memory starting at address I. 
The offset from I is increased by 1 for each value written, 
but I itself is left unmodified.
*/
void Chip::OpFX55(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;

	for(int i = 0; i <= x; i++)
	{
		memory[address + i] = registers[i];
	}

	address = address + x + 1;
}

/*
Fills V0 to VX (including VX) with values from memory starting at address I. 
The offset from I is increased by 1 for each value written, 
but I itself is left unmodified.
*/
void Chip::OpFX65(unsigned short int instruction)
{
	int x = instruction & 0x0f00;
	x >>= 8;

	for(int i = 0; i <= x; i++)
	{
		registers[i] = memory[address + i];
	}

	address = address + x + 1;
}


