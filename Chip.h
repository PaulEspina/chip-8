#pragma once

#include "Header.h"
#include <vector>

class Chip
{
public:
	Chip();
	Chip(const char *path);
	bool Readrom(const char *path);
	void Init();
	void Cycle();
private:
	word Fetch();
	void Decode();
	void Execute();
	void Op0NNN();
	void Op00E0();
	void Op00EE();
	void Op1NNN();
	void Op2NNN();
	void Op3XNN();
	void Op4XNN();
	void Op5XY0();
	void Op6XNN();
	void Op7xNN();
	void Op8XY0();
	void Op8XY1();
	void Op8XY2();
	void Op8XY3();
	void Op8XY4();
	void Op8XY5();
	void Op8XY6();
	void Op8XY7();
	void Op8XYE();
	void Op9XY0();
	void OpANNN();
	void OpBNNN();
	void OpCXNN();
	void OpDXNN();
	void OpEX9E();
	void OpEXA1();
	void OpFX07();
	void OpFX0A();
	void OpFX15();
	void OpFX18();
	void OpFX1E();
	void OpFX29();
	void OpFX33();
	void OpFX55();
	void OpFX65();
	byte memory[0xfff];
	byte registers[16];
	byte graphics[64][32];
	word address;
	word pc;
	std::vector<word> stack;
};

