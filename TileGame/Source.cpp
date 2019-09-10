#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>
#include <SFML/System.hpp>
#include <random>

using namespace tg;

int main() {

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 4;

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "TileGame!", sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar, settings);
	window->setFramerateLimit(120);
	window->setActive(false);

	Game game(window);

	game.start();

	game.wait();

	std::cout << "Closing" << std::endl;


	exit(0);
}


/*

	===================================================================

	Special Thanks:
		- imode on TPH discord for helping me solve the
		issue with the mass logs crashing the game

		- Dalphat on TPH discord for helping me with some
		issues with tile textures glitching.

		- markhc on TPH discord for helping me with
		issues regarding the Nullable class.









	===================================================================


*/