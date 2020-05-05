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

bool Chip::Readrom(const char *path)
{
	FILE *file = fopen(path, "rb");
	fread(&memory[0x200], 0xfff, 1, file);
	fclose(file);
}

void Chip::Init()
{
	srand((unsigned int) time(0));
	address = 0;
	pc = 0x200;
	memset(registers, 0, sizeof(registers));
	memset(memory, 0, sizeof(memory));
	memset(graphics, 0, sizeof(graphics));
	delay = 0;
	sound = 0;
}

void Chip::Cycle()
{
	word instruction = Fetch();
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

word Chip::Fetch()
{
	word next = memory[pc];
	next <<= 8;
	next |= memory[pc++];
	return next;
}

void Chip::Execute(word instruction)
{
	switch(instruction & 0xf000)
	{
	case 0x0000:
		switch(instruction & 0x000f)
		{
		case 0x0000:
			Op00E0(instruction);
		case 0x0001:
			Op00EE(instruction);
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
		case 0x000E:
			OpEX9E(instruction);
			break;
		case 0x0001:
			OpEXA1(instruction);
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
void Chip::Op00E0(word instruction)
{
	memset(graphics, 0, sizeof(graphics));
}

// Retursn from a subroutine
void Chip::Op00EE(word instruction)
{
	pc = stack.back();
	stack.pop_back();
}

// Jumps to address NNN
void Chip::Op1NNN(word instruction)
{
	pc = 0x0fff & instruction;
}

// Calls subroutine at NNN
void Chip::Op2NNN(word instruction)
{
	stack.push_back(pc);
	pc = instruction & 0x0fff;
}

// Skips the next instruction if VX equals NN
void Chip::Op3XNN(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	if(registers[x] == (instruction & 0x00ff))
	{
		pc++;
	}
}

// Skips the next instruction if VX not equal NN
void Chip::Op4XNN(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	if(registers[x] != (instruction & 0x00ff))
	{
		pc++;
	}
}

// Skips the next insutrction if VX equal VY
void Chip::Op5XY0(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	if(registers[x] == registers[y])
	{
		pc++;
	}
}

// Sets VX to NN
void Chip::Op6XNN(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	registers[x] = instruction & 0x00ff;
}

// Adds NN to VX
void Chip::Op7XNN(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	registers[x] += instruction & 0x00ff;
}

// Sets VX to the value of VY
void Chip::Op8XY0(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	registers[x] = registers[y];
}

// Sets VX to VX or VY
void Chip::Op8XY1(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	registers[x] |= registers[y];
}
 // Sets VX to VX and VY
void Chip::Op8XY2(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	registers[x] &= registers[y];
}

// Sets VX to VX xor VY
void Chip::Op8XY3(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	registers[x] ^= registers[y];
}

// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
void Chip::Op8XY4(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	word temp = registers[x] + registers[y];
	if((temp & 0x0f00) != 0)
	{
		registers[0xf] = 1;
	}
	else
	{
		registers[0xf] = 0;
	}
	registers[x] = temp;
}
 // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void Chip::Op8XY5(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	if(registers[x] > registers[y])
	{
		registers[0xf] = 1;
	}
	else
	{
		registers[0xf] = 0;
	}
	registers[x] -= registers[y];
}

// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
void Chip::Op8XY6(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	registers[0xf] = registers[x] & 0x1;
	registers[x] >>= 1;
}

// 	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
void Chip::Op8XY7(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	if(registers[y] > registers[x])
	{
		registers[0xf] = 1;
	}
	else
	{
		registers[0xf] = 0;
	}
	registers[y] -= registers[x];
}

// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
void Chip::Op8XYE(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	registers[0xf] = registers[x] >> 7;
	registers[x] <<= 1;
}

// Skips the next instruction if VX doesn't equal VY
void Chip::Op9XY0(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	if(registers[x] != registers[y])
	{
		pc++;
	}
}

// Sets I to the address NNN
void Chip::OpANNN(word instruction)
{
	address = instruction & 0x0fff;
}

// Jumps to the address NNN plus V0
void Chip::OpBNNN(word instruction)
{
	pc = registers[0] + (instruction & 0x0fff);
}

// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
void Chip::OpCXNN(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	registers[x] = rand() & (instruction & 0x0fff);
}

/*
Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. 
As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
*/
void Chip::OpDXYN(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	byte y = instruction & 0x00f0;
	y >>= 4;
	for(unsigned int i = 0; i < 8; i++)
	{
		for(unsigned int j = 0, n = (unsigned int) instruction & 0x000f; j < n; j++)
		{
			graphics[x + i][y + j] ^= memory[address];
		}
		address++;
	}
}
 // Skips the next instruction if the key stored in VX is pressed.
void Chip::OpEX9E(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	if(keys[registers[x]])
	{
		pc++;
	}
}

// Skips the next instruction if the key stored in VX isn't pressed.
void Chip::OpEXA1(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	if(!keys[registers[x]])
	{
		pc++;
	}
}
// Sets VX to the value of the delay timer.
void Chip::OpFX07(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	registers[x] = delay;
}

// A key press is awaited, and then stored in VX
void Chip::OpFX0A(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	int key = CheckIfKeyIsPressed();
	if(key != -1)
	{
		registers[x] = key;
	}
	else
	{
		pc -= 2;
	}
}

// Sets the delay timer to VX
void Chip::OpFX15(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	delay = registers[x];
}

void Chip::OpFX18(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	sound = registers[x];
}

// Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
void Chip::OpFX1E(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	address += registers[x];
	byte temp = address + registers[x];
	if((temp & 0xf000) == 0)
	{
		registers[0xf] = 0;
	}
	else
	{
		registers[0xf] = 1;
	}
}

// Sets I to the location of the sprite for the character in VX
void Chip::OpFX29(word instruction)
{
	byte x = instruction & 0x0f00;
	x >>= 8;
	address = registers[x] * 5;
}

/*
Stores the binary-coded decimal representation of VX, 
with the most significant of three digits at the address in I, 
the middle digit at I plus 1, and the least significant digit at I plus 2.
*/
void Chip::OpFX33(word instruction)
{
	int regx = instruction & 0x0f00;
	regx >>= 8;

	int value = registers[regx];

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
void Chip::OpFX55(word instruction)
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
void Chip::OpFX65(word instruction)
{
	int regx = instruction & 0x0f00;
	regx >>= 8;

	for(int i = 0; i <= regx; i++)
	{
		registers[i] = memory[address + i];
	}

	address = address + regx + 1;
}


