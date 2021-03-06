#include "Chip.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <Windows.h>
#include <iomanip>

void Tick(sf::RenderWindow &window, sf::Event &event);
void Update(sf::RenderWindow &window, sf::Event &event);
void Render(sf::RenderWindow &window, sf::Event &event);
void Debug();

const unsigned int SC_WIDTH = 640, SC_HEIGHT = 320;
const unsigned int scale = 10;
sf::RectangleShape pixels[32][64];
bool close = false;
Chip chip;

int main(int argc, char *argv[])
{
	std::thread debug;
	if(argc > 1)
	{
		if(argc > 2 && argv[2] == "-d")
		{
			debug = std::thread(Debug);
		}
	}
	if(argc > 3)
	{
		std::cout << "Usage: chip8 <rom-path> [<command>]\n";
		return -2;
	}
	sf::RenderWindow window(sf::VideoMode(SC_WIDTH, SC_HEIGHT), "CHIP-8");
	sf::Event event;
	chip.Init();
	if(!chip.LoadRom(argv[1]))
	{
		return -1;
	}
	for(unsigned int j = 0; j < 32; j++)
	{
		for(unsigned int i = 0; i < 64; i++)
		{
			pixels[j][i] = sf::RectangleShape(sf::Vector2f(1 * scale, 1 * scale));
			pixels[j][i].setPosition(sf::Vector2f(i * scale, j * scale));
			pixels[j][i].setFillColor(sf::Color(0, 0, 0));
		}
	}
	sf::Clock clock;
	float interval = 1000 / 60;
	int frames = 1200 / 60;
	unsigned int time = clock.getElapsedTime().asMilliseconds();
	while(window.isOpen())
	{
		Tick(window, event);
		unsigned int current = clock.getElapsedTime().asMilliseconds();
		Update(window, event);
		if((time + interval) < current)
		{
			chip.Countdown();
			for(int i = 0; i < frames; i++)
			{
				chip.Cycle();
			}
			time = current;
		}
		window.clear();
		Render(window, event);
		window.display();
	}
	if(argv[1] == "-d")
	{
		debug.join();
	}
}

void Tick(sf::RenderWindow &window, sf::Event &event)
{
	while(window.pollEvent(event))
	{
		// Input
		if(event.type == sf::Event::Closed)
		{
			window.close();
			close = true;
		}
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Q)
			{
				chip.KeyPressed(0);
			}
			if(event.key.code == sf::Keyboard::W)
			{
				chip.KeyPressed(1);
			}
			if(event.key.code == sf::Keyboard::E)
			{
				chip.KeyPressed(2);
			}
			if(event.key.code == sf::Keyboard::R)
			{
				chip.KeyPressed(3);
			}
			if(event.key.code == sf::Keyboard::A)
			{
				chip.KeyPressed(4);
			}
			if(event.key.code == sf::Keyboard::S)
			{
				chip.KeyPressed(5);
			}
			if(event.key.code == sf::Keyboard::D)
			{
				chip.KeyPressed(6);
			}
			if(event.key.code == sf::Keyboard::F)
			{
				chip.KeyPressed(7);
			}
			if(event.key.code == sf::Keyboard::Z)
			{
				chip.KeyPressed(8);
			}
			if(event.key.code == sf::Keyboard::X)
			{
				chip.KeyPressed(9);
			}
			if(event.key.code == sf::Keyboard::C)
			{
				chip.KeyPressed(10);
			}
			if(event.key.code == sf::Keyboard::V)
			{
				chip.KeyPressed(11);
			}
			if(event.key.code == sf::Keyboard::Num1)
			{
				chip.KeyPressed(12);
			}
			if(event.key.code == sf::Keyboard::Num2)
			{
				chip.KeyPressed(13);
			}
			if(event.key.code == sf::Keyboard::Num3)
			{
				chip.KeyPressed(14);
			}
			if(event.key.code == sf::Keyboard::Num4)
			{
				chip.KeyPressed(15);
			}
		}
		if(event.type == sf::Event::KeyReleased)
		{
			if(event.key.code == sf::Keyboard::Q)
			{
				chip.KeyReleased(0);
			}
			if(event.key.code == sf::Keyboard::W)
			{
				chip.KeyReleased(1);
			}
			if(event.key.code == sf::Keyboard::E)
			{
				chip.KeyReleased(2);
			}
			if(event.key.code == sf::Keyboard::R)
			{
				chip.KeyReleased(3);
			}
			if(event.key.code == sf::Keyboard::A)
			{
				chip.KeyReleased(4);
			}
			if(event.key.code == sf::Keyboard::S)
			{
				chip.KeyReleased(5);
			}
			if(event.key.code == sf::Keyboard::D)
			{
				chip.KeyReleased(6);
			}
			if(event.key.code == sf::Keyboard::F)
			{
				chip.KeyReleased(7);
			}
			if(event.key.code == sf::Keyboard::Z)
			{
				chip.KeyReleased(8);
			}
			if(event.key.code == sf::Keyboard::X)
			{
				chip.KeyReleased(9);
			}
			if(event.key.code == sf::Keyboard::C)
			{
				chip.KeyReleased(10);
			}
			if(event.key.code == sf::Keyboard::V)
			{
				chip.KeyReleased(11);
			}
			if(event.key.code == sf::Keyboard::Num1)
			{
				chip.KeyReleased(12);
			}
			if(event.key.code == sf::Keyboard::Num2)
			{
				chip.KeyReleased(13);
			}
			if(event.key.code == sf::Keyboard::Num3)
			{
				chip.KeyReleased(14);
			}
			if(event.key.code == sf::Keyboard::Num4)
			{
				chip.KeyReleased(15);
			}
		}
	}
}

void Update(sf::RenderWindow &window, sf::Event &event)
{
	for(unsigned int j = 0; j < 32; j++)
	{
		{
		for(unsigned int i = 0; i < 64; i++)
			pixels[j][i].setFillColor(sf::Color(255, 255, 255, chip.graphics[j][i]));
		}
	}
}

void Render(sf::RenderWindow &window, sf::Event &event)
{
	for(unsigned int j = 0; j < 32; j++)
	{
		{
		for(unsigned int i = 0; i < 64; i++)
			window.draw(pixels[j][i]);
		}
	}
}

void Goto(short x, short y)
{
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Debug()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(console, &cursor_info);
	cursor_info.bVisible = false;
	SetConsoleCursorInfo(console, &cursor_info);
	while(!close)
	{
		for(unsigned int i = 0; i <= 0xf; i++)
		{
			std::cout << "REG: V" << std::hex << i << std::dec << ": 0x" << std::hex << (int) chip.registers[i] << std::dec << std::endl;
		}
		std::cout << std::endl;
		std::cout << "PC: 0x" << std::hex << (int) chip.GetPC() << std::dec << std::endl;
		for(unsigned int i = 0; i <= 0xf; i++)
		{
			Goto(20, i);
			std::cout << "Key " << i << ": " << chip.keys[i] << std::endl;
		}
		system("cls");
	}
}
