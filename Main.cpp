#include "Chip.h"
#include <iostream>
#include <SFML/Graphics.hpp>

const unsigned int SC_WIDTH = 640, SC_HEIGHT = 320;
const unsigned int scale = 10;
Chip chip("pong.ch8");
sf::RectangleShape pixels[32][64];

void Tick(sf::RenderWindow &window, sf::Event &event);
void Update(sf::RenderWindow &window, sf::Event &event);
void Render(sf::RenderWindow &window, sf::Event &event);

int main()
{
	sf::RenderWindow window(sf::VideoMode(SC_WIDTH, SC_HEIGHT), "CHIP-8");
	sf::Event event;
	for(unsigned int i = 0; i < 64; i++)
	{
		for(unsigned int j = 0; j < 32; j++)
		{
			pixels[j][i] = sf::RectangleShape(sf::Vector2f(1 * scale, 1 * scale));
			pixels[j][i].setPosition(sf::Vector2f(i * scale, j * scale));
			pixels[j][i].setFillColor(sf::Color(0, 0, 0));
		}
	}
	sf::Clock clock;
	float interval = 1000 / 60;
	int frames = 400 / 60;
	unsigned int time = clock.getElapsedTime().asMilliseconds();
	while(window.isOpen())
	{
		Tick(window, event);
		unsigned int current = clock.getElapsedTime().asMilliseconds();
		if((time + interval) < current)
		{
			Update(window, event);
			if(chip.delay > 0)
			{
				chip.delay--;
			}
			if(chip.sound > 0)
			{
				chip.sound--;
			}
			for(int i = 0; i < frames; i++)
			{
				chip.Cycle();
			}
			time = current;
			window.clear();
			Render(window, event);
			window.display();
		}
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
		}
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::W)
			{
				chip.KeyPressed(8);
			}
			if(event.key.code == sf::Keyboard::A)
			{
				chip.KeyPressed(4);
			}
			if(event.key.code == sf::Keyboard::S)
			{
				chip.KeyPressed(6);
			}
			if(event.key.code == sf::Keyboard::D)
			{
				chip.KeyPressed(2);
			}
			if(event.key.code == sf::Keyboard::R)
			{
				chip.KeyPressed(0);
			}
			if(event.key.code == sf::Keyboard::T)
			{
				chip.KeyPressed(1);
			}
			if(event.key.code == sf::Keyboard::Y)
			{
				chip.KeyPressed(3);
			}
			if(event.key.code == sf::Keyboard::U)
			{
				chip.KeyPressed(5);
			}
			if(event.key.code == sf::Keyboard::F)
			{
				chip.KeyPressed(7);
			}
			if(event.key.code == sf::Keyboard::G)
			{
				chip.KeyPressed(9);
			}
			if(event.key.code == sf::Keyboard::H)
			{
				chip.KeyPressed(10);
			}
			if(event.key.code == sf::Keyboard::J)
			{
				chip.KeyPressed(11);
			}
			if(event.key.code == sf::Keyboard::V)
			{
				chip.KeyPressed(12);
			}
			if(event.key.code == sf::Keyboard::B)
			{
				chip.KeyPressed(13);
			}
			if(event.key.code == sf::Keyboard::N)
			{
				chip.KeyPressed(14);
			}
			if(event.key.code == sf::Keyboard::M)
			{
				chip.KeyPressed(15);
			}
		}
	}
}

void Update(sf::RenderWindow &window, sf::Event &event)
{
	for(unsigned int i = 0; i < 64; i++)
	{
		for(unsigned int j = 0; j < 32; j++)
		{
			pixels[j][i].setFillColor(sf::Color(255, 255, 255, chip.graphics[j][i]));
		}
	}
	std::cout << chip.pc << std::endl;
}

void Render(sf::RenderWindow &window, sf::Event &event)
{
	for(unsigned int i = 0; i < 64; i++)
	{
		for(unsigned int j = 0; j < 32; j++)
		{
			window.draw(pixels[j][i]);
		}
	}
}
