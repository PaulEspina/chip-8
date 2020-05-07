#pragma once

#include <vector>
#include <random>
#include <time.h>
#include <fstream>

typedef unsigned char byte;
typedef unsigned short int word;

class Chip
{
public:
	Chip();
	void Init();
	bool LoadRom(const char *path);
	void Restart();
	void Countdown();
	void Cycle();
	void KeyPressed(int key);
	void KeyReleased(int key);
	const word GetPC();
	byte graphics[32][64];
	byte memory[0x1000];
	byte registers[16];
	bool keys[16];
private:
	int CheckForKeyPress();
	word Fetch();
	void Execute(word opcode);
	void Op00E0(word opcode);
	void Op00EE(word opcode);
	void Op1NNN(word opcode);
	void Op2NNN(word opcode);
	void Op3XNN(word opcode);
	void Op4XNN(word opcode);
	void Op5XY0(word opcode);
	void Op6XNN(word opcode);
	void Op7XNN(word opcode);
	void Op8XY0(word opcode);
	void Op8XY1(word opcode);
	void Op8XY2(word opcode);
	void Op8XY3(word opcode);
	void Op8XY4(word opcode);
	void Op8XY5(word opcode);
	void Op8XY6(word opcode);
	void Op8XY7(word opcode);
	void Op8XYE(word opcode);
	void Op9XY0(word opcode);
	void OpANNN(word opcode);
	void OpBNNN(word opcode);
	void OpCXNN(word opcode);
	void OpDXYN(word opcode);
	void OpEX9E(word opcode);
	void OpEXA1(word opcode);
	void OpFX07(word opcode);
	void OpFX0A(word opcode);
	void OpFX15(word opcode);
	void OpFX18(word opcode);
	void OpFX1E(word opcode);
	void OpFX29(word opcode);
	void OpFX33(word opcode);
	void OpFX55(word opcode);
	void OpFX65(word opcode);
	word sp;
	word pc;
	std::vector<word> stack;
	byte delay;
	byte sound; // will not be implemented yet
	std::string rompath;
	byte fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
};

