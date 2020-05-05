#pragma once

#include <vector>
#include <random>
#include <time.h>
#include <fstream>

class Chip
{
public:
	Chip();
	Chip(const char *path);
	void Readrom(const char *path);
	void Restart();
	void Cycle();
	void KeyPressed(int key);
	void KeyReleased(int key);
	int CheckIfKeyIsPressed();
	unsigned char delay;
	unsigned char sound; // will not be implemented yet
	unsigned char graphics[32][64];
	unsigned char memory[0xfff];
	unsigned char registers[16];
	bool keys[16];
	unsigned short int address;
	unsigned short int pc;
	std::vector<unsigned short int> stack;
private:
	void Init();
	unsigned short int Fetch();
	void Execute(unsigned short int instruction);
	void Op00E0(unsigned short int instruction);
	void Op00EE(unsigned short int instruction);
	void Op1NNN(unsigned short int instruction);
	void Op2NNN(unsigned short int instruction);
	void Op3XNN(unsigned short int instruction);
	void Op4XNN(unsigned short int instruction);
	void Op5XY0(unsigned short int instruction);
	void Op6XNN(unsigned short int instruction);
	void Op7XNN(unsigned short int instruction);
	void Op8XY0(unsigned short int instruction);
	void Op8XY1(unsigned short int instruction);
	void Op8XY2(unsigned short int instruction);
	void Op8XY3(unsigned short int instruction);
	void Op8XY4(unsigned short int instruction);
	void Op8XY5(unsigned short int instruction);
	void Op8XY6(unsigned short int instruction);
	void Op8XY7(unsigned short int instruction);
	void Op8XYE(unsigned short int instruction);
	void Op9XY0(unsigned short int instruction);
	void OpANNN(unsigned short int instruction);
	void OpBNNN(unsigned short int instruction);
	void OpCXNN(unsigned short int instruction);
	void OpDXYN(unsigned short int instruction);
	void OpEX9E(unsigned short int instruction);
	void OpEXA1(unsigned short int instruction);
	void OpFX07(unsigned short int instruction);
	void OpFX0A(unsigned short int instruction);
	void OpFX15(unsigned short int instruction);
	void OpFX18(unsigned short int instruction);
	void OpFX1E(unsigned short int instruction);
	void OpFX29(unsigned short int instruction);
	void OpFX33(unsigned short int instruction);
	void OpFX55(unsigned short int instruction);
	void OpFX65(unsigned short int instruction);
};

