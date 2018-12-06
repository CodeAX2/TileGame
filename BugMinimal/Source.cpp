#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <math.h>
#include "Assets.h"


void render(sf::RenderWindow* window, Assets* assets);

int xOffset = 0;
int yOffset = 0;

int main() {

	Assets assets;
	assets.init();


	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Minimal!");
	window.setFramerateLimit(120);

	sf::Clock clock;

	while (window.isOpen()) {

		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			} else if (e.type == sf::Event::MouseWheelScrolled) {
				int mwd = e.mouseWheelScroll.delta;
				window.setView(sf::View(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(window.getView().getSize().x + window.getSize().x * -.1f * mwd, window.getView().getSize().y + window.getSize().y * -.1f * mwd))));

			} else if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Up) {
					yOffset += 8;
				}
				if (e.key.code == sf::Keyboard::Down) {
					yOffset -= 8;
				}
				if (e.key.code == sf::Keyboard::Left) {
					xOffset += 8;
				}
				if (e.key.code == sf::Keyboard::Right) {
					xOffset -= 8;
				}
			}
		}

		window.clear();

		sf::RectangleShape bg;
		bg.setFillColor(sf::Color::Black);
		bg.setSize(sf::Vector2f(window.getSize()));

		window.draw(bg);
		render(&window, &assets);


		window.display();


	}

	return 0;
}


void render(sf::RenderWindow* window, Assets* assets) {

	float tileSize = 32.f;

	sf::Vector2f v = window->getView().getSize();
	//std::cout << v.x << " " << v.y << std::endl;
	std::vector<std::vector<sf::RectangleShape*>> renderOrder(256,
		std::vector<sf::RectangleShape*>(0));


	for (int tileId = 0, x = 0, y = 0; tileId < 8; tileId++, x++) {
		if (tileId == 2 || tileId == 4) {
			x--;
			continue; // Id 2 is a water tile, and id 4 is a wood tile
			// They have different textures, so they are unimportant in this
			// minimal code
		}
		if (x >= 3) {
			x = 0; y++;
		}
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {

				for (int k = 0; k < 50; k++) {
					const sf::Texture* t;

					if (i == 0 && j == 0) {
						t = assets->getBaseTexture(tileId);
					} else {
						t = assets->getOuterTexture(tileId, i + j * 16, 0, 0, false);
					}

					sf::RectangleShape* tile = new sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
					tile->setPosition(i * (tileSize + 4) + x * (tileSize + 4) * 16 + xOffset, j * (tileSize + 4) + y * (tileSize + 4) * 16 + yOffset);
					tile->setTexture(t);
					renderOrder[tileId].push_back(tile);
				}
			}
		}
	}


	for (int i = 0; i < renderOrder.size(); i++) {
		for (int j = 0; j < renderOrder[i].size(); j++) {
			sf::RectangleShape* cur = renderOrder[i][j];
			window->draw(*cur);
			delete cur;
		}
	}



}