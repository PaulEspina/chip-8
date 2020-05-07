#include "Chip.h"

Chip::Chip()
{
	srand((unsigned int) time(0));
	Init();
}

void Chip::Init()
{
	memset(graphics, 0, sizeof(graphics));
	memset(memory, 0, sizeof(memory));
	memset(registers, 0, sizeof(registers));
	memset(keys, 0, sizeof(keys));
	for(int i = 0; i < sizeof(fontset); i++)
	{
		memory[0x50 + i] = fontset[i];
	}
	sp = 0;
	pc = (word)0x200;
	delay = 0;
	sound = 0;
}

bool Chip::LoadRom(const char *path)
{
	rompath = path;
	std::ifstream file(path, std::ifstream::binary | std::ifstream::in);
	if(!file.is_open())
	{
		return false;
	}
	file.seekg(0, std::ios::end);
	std::streamoff length = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> result((unsigned int) length);
	file.read(&result[0], length);
	for(unsigned int i = 0, size = result.size(), mem = 0x200; i < size; i++, mem++)
	{
		memory[mem] = (int) result[i];
	}
	return true;
}

void Chip::Restart()
{
	Init();
	LoadRom(rompath.c_str());
}

void Chip::Countdown()
{
	if(delay > 0)
	{
		delay--;
	}
	if(sound > 0)
	{
		sound--;
	}
}

void Chip::Cycle()
{
	word opcode = Fetch();
	Execute(opcode);
}

void Chip::KeyPressed(int key)
{
	keys[key] = 1;
}

void Chip::KeyReleased(int key)
{
	keys[key] = 0;
}

const word Chip::GetPC()
{
	return pc;
}

int Chip::CheckForKeyPress()
{
	for(int i = 0; i < 16; i++)
	{
		if(keys[i])
		{
			return i;
		}
	}
	return -1;
}

word Chip::Fetch()
{
	word opcode = memory[pc];
	opcode <<= 8;
	opcode += memory[pc + 1];
	pc += 2;
	return opcode;
}

void Chip::Execute(word opcode)
{
	switch(opcode & 0xf000)
	{
	case 0x0000:
		switch(opcode & 0x000f)
		{
		case 0x0000:
			Op00E0(opcode);
			break;
		case 0x000e:
			Op00EE(opcode);
			break;
		}
		break;
	case 0x1000:
		Op1NNN(opcode);
		break;
	case 0x2000:
		Op2NNN(opcode);
		break;
	case 0x3000:
		Op3XNN(opcode);
		break;
	case 0x4000:
		Op4XNN(opcode);
		break;
	case 0x5000:
		Op5XY0(opcode);
		break;
	case 0x6000:
		Op6XNN(opcode);
		break;
	case 0x7000:
		Op7XNN(opcode);
		break;
	case 0x8000:
		switch(opcode & 0x000f)
		{
		case 0x0000:
			Op8XY0(opcode);
			break;
		case 0x0001:
			Op8XY1(opcode);
			break;
		case 0x0002:
			Op8XY2(opcode);
			break;
		case 0x0003:
			Op8XY3(opcode);
			break;
		case 0x0004:
			Op8XY4(opcode);
			break;
		case 0x0005:
			Op8XY5(opcode);
			break;
		case 0x0006:
			Op8XY6(opcode);
			break;
		case 0x0007:
			Op8XY7(opcode);
			break;
		case 0x000e:
			Op8XYE(opcode);
			break;
		}
		break;
	case 0x9000:
		Op9XY0(opcode);
		break;
	case 0xa000:
		OpANNN(opcode);
		break;
	case 0xb000:
		OpBNNN(opcode);
		break;
	case 0xc000:
		OpCXNN(opcode);
		break;
	case 0xd000:
		OpDXYN(opcode);
		break;
	case 0xe000:
		switch(opcode & 0x000f)
		{
		case 0x000e:
			OpEX9E(opcode);
			break;
		case 0x0001:
			OpEXA1(opcode);
			break;
		}
		break;
	case 0xf000:
		switch(opcode & 0x00f0)
		{
		case 0x0000:
			switch(opcode & 0x000f)
			{
			case 0x0007:
				OpFX07(opcode);
				break;
			case 0x000a:
				OpFX0A(opcode);
				break;
			}
			break;
		case 0x0010:
			switch(opcode & 0x000f)
			{
			case 0x0005:
				OpFX15(opcode);
				break;
			case 0x0008:
				OpFX18(opcode);
				break;
			case 0x000e:
				OpFX1E(opcode);
				break;
			}
			break;
		case 0x0020:
			OpFX29(opcode);
			break;
		case 0x0030:
			OpFX33(opcode);
			break;
		case 0x0050:
			OpFX55(opcode);
			break;
		case 0x0060:
			OpFX65(opcode);
			break;
		}
		break;
	}
}

// Clears the screen.
void Chip::Op00E0(word opcode)
{
	memset(graphics, 0, sizeof(graphics));
}

// Returns from a subroutine.
void Chip::Op00EE(word opcode)
{
	pc = stack.back();
	stack.pop_back();
}

// Jumps to address at NNN.
void Chip::Op1NNN(word opcode)
{
	pc = opcode & 0x0fff;
}

// Calls subroutine NNN.
void Chip::Op2NNN(word opcode)
{
	stack.push_back(pc);
	pc = opcode & 0x0fff;
}

// Skips the next instruction if VX == NN.
void Chip::Op3XNN(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int nn = opcode & 0x00ff;
	if(registers[regx] == nn)
	{
		pc += 2;
	}
}

// Skips the next instruction if VX != NN.
void Chip::Op4XNN(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int nn = opcode & 0x00ff;
	if(registers[regx] != nn)
	{
		pc += 2;
	}
}

// Skips the next instrution if VX == VY.
void Chip::Op5XY0(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	if(registers[regx] == registers[regy])
	{
		pc += 2;
	}
}

// Sets register VX to NN.
void Chip::Op6XNN(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int nn = opcode & 0x00ff;
	registers[regx] = nn;
}

// Adds NN to VX.
void Chip::Op7XNN(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int nn = opcode & 0x00ff;
	registers[regx] += nn;
}

// Sets VX to the value of VY.
void Chip::Op8XY0(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	registers[regx] = registers[regy];
}

// Sets VX to VX or VY.
void Chip::Op8XY1(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	registers[regx] |= registers[regy];
}

// Set VX to VX and VY.
void Chip::Op8XY2(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	registers[regx] &= registers[regy];
}

// Sets VX to VX xor VY.
void Chip::Op8XY3(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	registers[regx] ^= registers[regy];
}

// Adds VY to VX. VF is set to 1 when there's carry otherwise 0.
void Chip::Op8XY4(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	int value = registers[regx] + registers[regy];
	if(value > 0xff)
	{
		registers[0xf] = 1;
	}
	else
	{
		registers[0xf] = 0;
	}
	registers[regx] = value;
}

// VY is subtracted from VX. VF is set to 0 when there's a borrow otherwise 1.
void Chip::Op8XY5(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	if(registers[regx] < registers[regy])
	{
		registers[0xf] = 0;
	}
	else
	{
		registers[0xf] = 1;
	}
	registers[regx] -= registers[regy];

}

// Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
void Chip::Op8XY6(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	registers[0xf] = registers[regx] & 0x1;
	registers[regx] >>= 1;
}

// Sets VX to VY minus VX. VF is set to 0 when there's a borrow otherwise 1.
void Chip::Op8XY7(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	if(registers[regy] < registers[regx])
	{
		registers[0xf] = 0;
	}
	else
	{
		registers[0xf] = 1;
	}
	registers[regx] = registers[regy] - registers[regx];
}

// Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
void Chip::Op8XYE(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	registers[0xf] = (registers[regx] & 0x80) >> 7;
	registers[regx] <<= 1;
}

// Skips the next instruction if VX != VY.
void Chip::Op9XY0(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	if(registers[regx] != registers[regy])
	{
		pc += 2;
	}
}

// Sets sp to address NNN;
void Chip::OpANNN(word opcode)
{
	int nnn = opcode & 0x0fff;
	sp = nnn;
}

// Jumps to the address NNN plus V0
void Chip::OpBNNN(word opcode)
{
	int nnn = opcode & 0x0fff;
	pc = registers[0] + nnn;
}

// Sets VX to the result of a bitwise & operation on a random number and NN.
void Chip::OpCXNN(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int nn = opcode & 0x00ff;
	int random = rand() % 256;
	registers[regx] = random & nn;
}

// Display
void Chip::OpDXYN(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int regy = opcode & 0x00f0;
	regy >>= 4;
	int height = opcode & 0x000f;
	int x = registers[regx];
	int y = registers[regy];
	registers[0xf] = 0;
	for(int row = 0; row < height; row++)
	{
		int data = memory[sp + row];
		for(int col = 0; col < 8; col++)
		{
			if((data & (0x80 >> col)) != 0)
			{
				if(graphics[y + row][x + col] == 0xff)
				{
					registers[0xf] = 1;
				}
				graphics[y + row][x + col] ^= 0xff;
			}
		}
	}
}

// Skips the next instruction if the key stored in VX is pressed.
void Chip::OpEX9E(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	if(keys[registers[regx]])
	{
		pc += 2;
	}
}

// Skips the next instruction if the key stored in VX is not pressed.
void Chip::OpEXA1(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	if(!keys[registers[regx]])
	{
		pc += 2;
	}
}

// Sets VX to the value of the delay timer.
void Chip::OpFX07(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	registers[regx] = delay;
}

// Waits for a key press then store that key in VX
void Chip::OpFX0A(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int key = CheckForKeyPress();
	if(key != -1)
	{
		registers[regx] = key;
	}
	else
	{
		pc -= 2;
	}
}

// Sets the delay timer to VX.
void Chip::OpFX15(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	delay = registers[regx];
}

// Sets the sound timer to VX.
void Chip::OpFX18(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	sound = registers[regx];
}

// Adds VX to sp. VF is set to 1 when sp + vx > 0xfff, otherwise 0.
void Chip::OpFX1E(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	if(sp + registers[regx] > 0x0fff)
	{
		registers[0xf] = 1;
	}
	else
	{
		registers[0xf] = 0;
	}
	sp += registers[regx];
}

// Sets sp to the location of the sprite for the character in VX.
void Chip::OpFX29(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	sp = 0x50 + (registers[regx] * 5);
}

// Hard to explain what this does lol.
void Chip::OpFX33(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	int dec = registers[regx];
	int hundreds = dec / 100;
	int tens = (dec / 10) % 10;
	int ones = dec % 10;
	memory[sp] = hundreds;
	memory[sp + 1] = tens;
	memory[sp + 2] = ones;
}

// Stores V0 to VX in memory starting at sp. sp is not modified.
void Chip::OpFX55(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	for(int i = 0; i <= regx; i++)
	{
		memory[sp + i] = registers[i];
	}
}

// Fills V0 to VX with values from memory starting at sp. sp is not modified.
void Chip::OpFX65(word opcode)
{
	int regx = opcode & 0x0f00;
	regx >>= 8;
	for(int i = 0; i <= regx; i++)
	{
		registers[i] = memory[sp + i];
	}
}
