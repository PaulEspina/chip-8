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
	bool keys[16];;
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
	unsigned char font[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};
};

