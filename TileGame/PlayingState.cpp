#include "PlayingState.h"
#include "EntityManager.h"
#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "Camera.h"
#include "Player.h"
#include <iostream>
#include "TileData.h"
#include <bitset>
#include "InputManager.h"
#include "Building.h"
#include "Game.h"
#include <sstream>
#include <math.h>

using namespace tg;

PlayingState::PlayingState(Handler* handler) : GameState(PLAYING), handler(handler) {
	prevZoom = 1.f;
	world = handler->mainWorld;
}


PlayingState::~PlayingState() {
}

void PlayingState::zoom() {
	if (zooming)
		return;
	handler->inputManager->setZoomLevel(.2f);
	zooming = true;
}

// Render every part of the game
void PlayingState::render() {

	if (world == nullptr) {
		world = handler->mainWorld;
	}

	if (zooming) {
		if (handler->inputManager->getZoomLevel() >= 1.2f) {
			zooming = false;
		} else {
			handler->inputManager->setZoomLevel(handler->inputManager->getZoomLevel() + .001f);
		}


	}
	float z = handler->inputManager->getZoomLevel();
	handler->worldView = sf::View(sf::Vector2f(handler->window->getSize().x / 2, handler->window->getSize().y / 2), sf::Vector2f(1280.f*1.f / z, 720.f*1.f / z));


	handler->window->setView(handler->worldView);
	if (z != prevZoom) {
		handler->camera->fixForZoom();
	}

	prevZoom = z;

	renderWorld();
	world->getEntityManager()->render();

	handler->window->setView(handler->guiView);
	//renderVignette();
	//renderAllTextures();
	renderGUI();

}

// Update every part of the game
void PlayingState::tick(sf::Int32 dt) {
	if (world == nullptr) {
		world = handler->player->getWorld();
	}
	handler->player->tick(dt);
	handler->camera->tick();
	world->getEntityManager()->tick(dt);


}

// Draw the world to the screen


void PlayingState::renderWorld() {

	if (world != handler->player->getWorld()) {
		world = handler->player->getWorld();
	}

	float xOffset = floor(handler->camera->getXOffset());
	float yOffset = floor(handler->camera->getYOffset());

	float tileSize = 96.f;

	sf::Vector2f v = handler->window->getView().getSize();
	//std::cout << v.x << " " << v.y << std::endl;
	std::vector<std::vector<sf::VertexArray>> renderOrderV(256,
		std::vector<sf::VertexArray>(0));

	std::vector<std::vector<const sf::Texture*>> renderOrderT(256,
		std::vector<const sf::Texture*>(0));


	int lowerX = std::max((int)floor((xOffset + (handler->window->getSize().x / 2 - v.x / 2) - tileSize) / tileSize) + 1, 0);
	int upperX = std::min((int)ceil((handler->window->getSize().x + xOffset - (handler->window->getSize().x / 2 - v.x / 2)) / tileSize), world->getWidth() - 1);
	int lowerY = std::max((int)floor((yOffset + (handler->window->getSize().y / 2 - v.y / 2) - tileSize) / tileSize) + 1, 0);
	int upperY = std::min((int)ceil((handler->window->getSize().y + yOffset - (handler->window->getSize().y / 2 - v.y / 2)) / tileSize), world->getHeight() - 1);

	for (int x = lowerX; x <= upperX; x++) {
		for (int y = lowerY; y <= upperY; y++) {

			sf::Uint8 tileId = world->getTile(x, y);

			sf::Vector2f basicPosition((x * tileSize - xOffset), (y * tileSize - yOffset));

			sf::VertexArray tile(sf::Quads, 4);
			sf::Texture* t;

			if (tileId != 8 && tileId != 12 && tileId != 14) {
				tile[0].position = basicPosition;
				tile[1].position = sf::Vector2f(basicPosition.x + tileSize, basicPosition.y);
				tile[2].position = sf::Vector2f(basicPosition.x + tileSize, basicPosition.y + tileSize);
				tile[3].position = sf::Vector2f(basicPosition.x, basicPosition.y + tileSize);

				tile[0].texCoords = sf::Vector2f(0, 0);
				tile[1].texCoords = sf::Vector2f(32, 0);
				tile[2].texCoords = sf::Vector2f(32, 32);
				tile[3].texCoords = sf::Vector2f(0, 32);
			} else {

				tile[0].position = basicPosition;
				tile[0].position.y -= 96;
				tile[1].position = sf::Vector2f(basicPosition.x + tileSize, basicPosition.y - 96);
				tile[2].position = sf::Vector2f(basicPosition.x + tileSize, basicPosition.y + tileSize);
				tile[3].position = sf::Vector2f(basicPosition.x, basicPosition.y + tileSize);

				tile[0].texCoords = sf::Vector2f(0, 0);
				tile[1].texCoords = sf::Vector2f(32, 0);
				tile[2].texCoords = sf::Vector2f(32, 64);
				tile[3].texCoords = sf::Vector2f(0, 64);
			}

			t = handler->assets->getBaseTexture(tileId);

			if (t != nullptr) {
				renderOrderT[0].push_back(t); // We always want the base tiles to be drawn first
				renderOrderV[0].push_back(tile);
			}

			TileData currentTileData = world->getTileData(x, y);

			std::map<sf::Uint8, sf::Uint8> adjInfo = currentTileData.getAdjacentInfo();
			for (std::map<sf::Uint8, sf::Uint8>::iterator it = adjInfo.begin(); it != adjInfo.end(); it++) {

				sf::Uint8 key = it->first;
				sf::Uint8 value = it->second;

				const sf::Texture* newT = handler->assets->getOuterTexture(key, value, x, y);
				if (newT == nullptr) continue;
				if (handler->assets->getTilePriority(key) == -1) {
					std::cout << "Invalid tile priority!" << std::endl;
					continue;
				}
				renderOrderT[handler->assets->getTilePriority(key) + 1].push_back(newT);
				renderOrderV[handler->assets->getTilePriority(key) + 1].push_back(tile);
			}

		}
	}

	for (int i = 0; i < renderOrderV.size(); i++) {
		for (int j = 0; j < renderOrderV[i].size(); j++) {
			sf::VertexArray curV = renderOrderV[i][j];
			const sf::Texture* curT = renderOrderT[i][j];
			handler->window->draw(curV, curT);
		}
	}

	sf::Vector2i hT = world->getHighlightedTile();
	if (hT.x == -1 || hT.y == -1) {
		return;
	}
	sf::Uint8 hId = world->getTile(hT.x, hT.y);
	if (hId == 0 || hId == 2 || hId == 3 || hId == 7) {
		sf::RectangleShape highlight(sf::Vector2f(tileSize - 10, tileSize - 10));
		highlight.setPosition((int)(hT.x * tileSize - xOffset + 5), (int)(hT.y * tileSize - yOffset + 5));
		if (world->highlightIsGood()) {
			highlight.setFillColor(sf::Color(151, 255, 145, 150));
		} else {
			highlight.setFillColor(sf::Color(255, 0, 0, 150));
		}

		handler->window->draw(highlight);
	}

}

// Draw the GUI to the screen
void PlayingState::renderGUI() {

	const sf::Texture* invText = handler->assets->getGuiInv();
	sf::RectangleShape inventory(sf::Vector2f(invText->getSize().x * 3, invText->getSize().y * 3));
	inventory.setTexture(invText);

	if (guiOnBottom) {
		inventory.setPosition(handler->window->getView().getSize().x / 2 - inventory.getSize().x / 2, handler->window->getView().getSize().y - inventory.getSize().y - 20);
	} else {
		inventory.setPosition(handler->window->getView().getSize().x / 2 - inventory.getSize().x / 2, 20);
	}

	handler->window->draw(inventory);
	std::map<sf::Uint8, int> pInv = handler->player->getInventory();
	int hotbarSpot = 0;
	float y = inventory.getPosition().y + 5;
	for (int i = 0; i < pInv.size(); i++) {
		if (pInv[i] != 0) {
			const sf::Texture* curItemTexture = handler->assets->getItemTexture(i);
			sf::RectangleShape curItem(sf::Vector2f(96, 96));
			curItem.setTexture(curItemTexture);
			curItem.setPosition(sf::Vector2f(hotbarSpot * 42 * 3 + 28 * 3 + 6, y));
			handler->window->draw(curItem);
			sf::Text count;
			count.setString(std::to_string(pInv[i]));
			count.setFont(*(handler->assets->getArialiFont()));
			count.setCharacterSize(16);
			count.setPosition(sf::Vector2f(hotbarSpot * 42 * 3 + 28 * 3 + 12, y + 102 - count.getGlobalBounds().height - 10));
			handler->window->draw(count);
			hotbarSpot++;
		}
	}

}

void PlayingState::renderAllTextures() {

	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(handler->window->getSize().x, handler->window->getSize().y));
	bg.setFillColor(sf::Color(0, 0, 0, 220));
	handler->window->draw(bg);

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {

			const sf::Texture* t;

			if (i == 0 && j == 0) {
				t = handler->assets->getBaseTexture(1);
			} else {
				t = handler->assets->getOuterTexture(1, i + j * 16, 0, 0, false);
			}

			sf::RectangleShape shape(sf::Vector2f(32, 32));
			shape.setTexture(t);
			shape.setPosition(i * 38, j * 38);
			handler->window->draw(shape);
		}
	}
}

void PlayingState::renderVignette() {
	sf::RectangleShape shape(sf::Vector2f(1280, 720));
	shape.setTexture(handler->assets->getVignette());
	handler->window->draw(shape);
}
