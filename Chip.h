#pragma once

#include "Header.h"
#include <vector>
#include <random>
#include <time.h>

class Chip
{
public:
	Chip();
	Chip(const char *path);
	bool Readrom(const char *path);
	void Init();
	void Cycle();
	void KeyPressed(int key);
	void KeyReleased(int key);
	int CheckIfKeyIsPressed();
private:
	word Fetch();
	word Fetch();
	void Execute(word instruction);
	void Op00E0(word instruction);
	void Op00EE(word instruction);
	void Op1NNN(word instruction);
	void Op2NNN(word instruction);
	void Op3XNN(word instruction);
	void Op4XNN(word instruction);
	void Op5XY0(word instruction);
	void Op6XNN(word instruction);
	void Op7XNN(word instruction);
	void Op8XY0(word instruction);
	void Op8XY1(word instruction);
	void Op8XY2(word instruction);
	void Op8XY3(word instruction);
	void Op8XY4(word instruction);
	void Op8XY5(word instruction);
	void Op8XY6(word instruction);
	void Op8XY7(word instruction);
	void Op8XYE(word instruction);
	void Op9XY0(word instruction);
	void OpANNN(word instruction);
	void OpBNNN(word instruction);
	void OpCXNN(word instruction);
	void OpDXYN(word instruction);
	void OpEX9E(word instruction);
	void OpEXA1(word instruction);
	void OpFX07(word instruction);
	void OpFX0A(word instruction);
	void OpFX15(word instruction);
	void OpFX18(word instruction);
	void OpFX1E(word instruction);
	void OpFX29(word instruction);
	void OpFX33(word instruction);
	void OpFX55(word instruction);
	void OpFX65(word instruction);
	byte memory[0xfff];
	byte registers[0xf];
	byte graphics[64][32];
	bool keys[0xf];
	word address;
	word pc;
	std::vector<word> stack;
	byte delay;
	byte sound; // will not be implemented yet
};

