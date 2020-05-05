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
	
}

void Chip::Cycle()
{
}

word Chip::Fetch()
{
	word next = memory[pc];
	next <<= 8;
	next |= memory[pc++];
	return next;
}

void Chip::Decode()
{
}

void Chip::Execute()
{
}
