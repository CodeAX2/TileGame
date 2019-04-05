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
#include "InputManager.h"
#include "ItemMeta.h"
#include "WorldManager.h"
#include <limits>
#include "Static.h"
#include "Zombie.h"
#include "Skeleton.h"

using namespace tg;

PlayingState::PlayingState(Handler* handler) : GameState(PLAYING), handler(handler) {
	prevZoom = 1.f;
	world = handler->mainWorld;
	bgMusic.openFromFile("Audio/airtone_-_panspermia_1.wav");
	bgMusic.setLoop(true);
	hotBarSlotHighlight = handler->assets->loadTextureFromResource(INV_HIGHLIGHT);

	loadStateFile();

}


PlayingState::~PlayingState() {
	bgMusic.stop();
}

void PlayingState::zoom() {
	if (zooming)
		return;
	handler->inputManager->setZoomLevel(.2f);
	zooming = true;
}

// Render every part of the game
void PlayingState::render() {

	lightArr.clear();
	wallArr.clear();
	bldgTextureArr.clear();
	bldgPosArr.clear();

	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}

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

	renderTexture.create(handler->worldView.getSize().x, handler->worldView.getSize().y);
	renderTexture.clear();

	handler->window->setView(handler->worldView);
	renderTexture.setView(handler->worldView);
	if (z != prevZoom) {
		handler->camera->fixForZoom();
	}

	prevZoom = z;

	renderWorld();
	world->getEntityManager()->render();
	renderTime();


	handler->window->setView(handler->guiView);

	if (!deathScreen) {
		if (!(handler->getCurrentState()->getType() == INVENTORY ||
			handler->getCurrentState()->getType() == CHEST_INVENTORY ||
			handler->getCurrentState()->getType() == CRAFTING_INVENTORY)) {
			// If rendering inventory state, we don't want GUI
			renderGUI();
		}
	} else {
		renderDeathScreen();
	}
}

// Update every part of the game
void PlayingState::tick(sf::Int32 dt) {


	if (world == nullptr) {
		world = handler->player->getWorld();
	}

	if (!deathScreen) {
		handler->player->tick(dt);
		handler->camera->tick();
		world->getEntityManager()->tick(dt);

		for (World* w : handler->worldManager->getAllWorlds()) {
			if (w != world) {
				w->getEntityManager()->tickExtras(dt);
			}
		}

		time += dt;
		if (time >= 1200000) { // 20 minutes
			time = 0;
		}

		if (world->getDarknessPercent() == -1) {

			if (time < 540000) { // First 9 minutes
				darknessPercent = 0;
			} else if (time >= 540000 && time < 600000) { // 10th minute
				darknessPercent = (float)(time - 540000) * 1.f / 60000;
			} else if (time >= 600000 && time < 1140000) { // 11-19th minutes
				darknessPercent = 1;
			} else { // 20th minute
				darknessPercent = 1 - (float)(time - 1140000) * 1.f / 60000;
			}

		} else {
			darknessPercent = world->getDarknessPercent();
		}


		timeSinceLastEnemySpawn += dt;

		if (timeSinceLastEnemySpawn >= sf::seconds(3).asMilliseconds()) {
			spawnEnemies();
			timeSinceLastEnemySpawn = 0;
		}



	} else {
		if (handler->getCurrentState()->getType() != PLAYING) {
			handler->setGameState(PLAYING);
		}
		deathFade += (float)dt * .001;
		if (deathFade > 1)
			deathFade = 1.f;

		gameOverFade += (float)dt * .0007;
		if (gameOverFade > 1)
			gameOverFade = 1.f;

	}


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
			sf::RenderStates state;
			state.texture = curT;
			handler->window->draw(curV, state);
		}
	}

	if (handler->getCurrentState()->getType() == INVENTORY) {
		return;
	}



	int hotBarItemId = handler->player->getItemInfoInHotBar().first;
	int hotBarItemAmount = handler->player->getItemInfoInHotBar().second;
	sf::Vector2i hT = world->getHighlightedTile();
	if (hT.x == -1 || hT.y == -1 || !ItemMeta::itemIsPlacable(hotBarItemId) || hotBarItemAmount == 0) {
		return;
	}
	sf::Uint8 hId = world->getTile(hT.x, hT.y);
	if (ItemMeta::canPlaceOnTile(hotBarItemId, hId)) {
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

	int hotBarSlot = handler->player->getHotBarSlot();
	sf::RectangleShape hotBarHighlight(sf::Vector2f(hotBarSlotHighlight->getSize().x * 3, hotBarSlotHighlight->getSize().y * 3));
	if (guiOnBottom) {
		hotBarHighlight.setPosition(sf::Vector2f(handler->window->getView().getSize().x / 2 - inventory.getSize().x / 2 + (108 + 18) * hotBarSlot, handler->window->getView().getSize().y - inventory.getSize().y - 20));
	} else {
		hotBarHighlight.setPosition(sf::Vector2f(handler->window->getView().getSize().x / 2 - inventory.getSize().x / 2 + (108 + 18) * hotBarSlot, 20));
	}
	hotBarHighlight.setTexture(hotBarSlotHighlight);
	handler->window->draw(hotBarHighlight);



	Inventory* pInv = handler->player->getInventory();

	float y = inventory.getPosition().y + 5;
	for (int i = 0; i < 9; i++) {
		if (pInv->getInventory()[i].first != -1 && pInv->getInventory()[i].second > 0) {
			const sf::Texture* curItemTexture = handler->assets->getItemTexture(pInv->getInventory()[i].first);
			sf::RectangleShape curItem(sf::Vector2f(96, 96));
			curItem.setTexture(curItemTexture);
			curItem.setPosition(sf::Vector2f(i * 42 * 3 + 28 * 3 + 6, y));
			handler->window->draw(curItem);
			sf::Text count;
			count.setString(std::to_string(pInv->getInventory()[i].second));
			count.setFont(guiFont);
			count.setCharacterSize(16);
			count.setPosition(sf::Vector2f(i * 42 * 3 + 28 * 3 + 12, y + 102 - count.getGlobalBounds().height - 10));
			handler->window->draw(count);
		}
	}

	sf::RectangleShape healthBarBackground(sf::Vector2f(300, 20));
	float yPos = handler->window->getView().getSize().y - 160;
	if (!guiOnBottom) {
		yPos = 140;
	}
	healthBarBackground.setPosition(handler->window->getView().getSize().x / 2 - healthBarBackground.getSize().x / 2, yPos);
	healthBarBackground.setFillColor(sf::Color(40, 2, 2));

	handler->window->draw(healthBarBackground);

	sf::RectangleShape healthBar(sf::Vector2f((float)handler->player->getHealth() / handler->player->getMaxHealth() * 300, 20));
	healthBar.setPosition(handler->window->getView().getSize().x / 2 - healthBar.getSize().x / 2, yPos);
	healthBar.setFillColor(sf::Color(132, 5, 5));

	handler->window->draw(healthBar);


	sf::Text healthText;
	healthText.setFont(guiFont);
	std::stringstream ss;
	ss << handler->player->getHealth() << "/" << handler->player->getMaxHealth();

	healthText.setString(ss.str());
	healthText.setCharacterSize(20);
	healthText.setPosition(handler->window->getView().getSize().x / 2 - healthText.getLocalBounds().width / 2, yPos - 10 + healthText.getLocalBounds().height / 2);


	handler->window->draw(healthText);



	sf::RectangleShape stamBarBackground(sf::Vector2f(300, 20));
	if (!guiOnBottom) {
		yPos = 140;
	}
	stamBarBackground.setPosition(3 * handler->window->getView().getSize().x / 4 - stamBarBackground.getSize().x / 2, yPos);
	stamBarBackground.setFillColor(sf::Color(2, 40, 2));

	handler->window->draw(stamBarBackground);

	sf::RectangleShape stamBar(sf::Vector2f((float)handler->player->getStamina() / handler->player->getMaxStamina() * 300, 20));
	stamBar.setPosition(3 * handler->window->getView().getSize().x / 4 - stamBar.getSize().x / 2, yPos);
	stamBar.setFillColor(sf::Color(5, 132, 5));
	if (handler->player->stamIsRegeningSlowly()) {
		stamBar.setFillColor(sf::Color(150, 100, 5));
	}

	handler->window->draw(stamBar);


	sf::Text stamText;
	stamText.setFont(guiFont);
	ss = std::stringstream();
	ss << handler->player->getStamina() << "/" << handler->player->getMaxStamina();

	stamText.setString(ss.str());
	stamText.setCharacterSize(20);
	stamText.setPosition(3 * handler->window->getView().getSize().x / 4 - stamText.getLocalBounds().width / 2, yPos - 10 + stamText.getLocalBounds().height / 2);


	handler->window->draw(stamText);

	sf::RectangleShape magicBarBackground(sf::Vector2f(300, 20));
	if (!guiOnBottom) {
		yPos = 140;
	}
	magicBarBackground.setPosition(handler->window->getView().getSize().x / 4 - magicBarBackground.getSize().x / 2, yPos);
	magicBarBackground.setFillColor(sf::Color(2, 2, 40));

	handler->window->draw(magicBarBackground);

	sf::RectangleShape magicBar(sf::Vector2f((float)handler->player->getMagic() / handler->player->getMaxMagic() * 300, 20));
	magicBar.setPosition(handler->window->getView().getSize().x / 4 - magicBar.getSize().x / 2, yPos);
	magicBar.setFillColor(sf::Color(5, 5, 132));

	handler->window->draw(magicBar);


	sf::Text magicText;
	magicText.setFont(guiFont);
	ss = std::stringstream();
	ss << handler->player->getMagic() << "/" << handler->player->getMaxMagic();

	magicText.setString(ss.str());
	magicText.setCharacterSize(20);
	magicText.setPosition(handler->window->getView().getSize().x / 4 - magicText.getLocalBounds().width / 2, yPos - 10 + stamText.getLocalBounds().height / 2);


	handler->window->draw(magicText);

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

void PlayingState::renderDeathScreen() {

	sf::RectangleShape deathOverlay(handler->window->getView().getSize());
	deathOverlay.setFillColor(sf::Color(0, 0, 0, deathFade * 255));

	handler->window->draw(deathOverlay);

	sf::Text gameOverText((std::string)"GAME OVER!", guiFont);
	gameOverText.setCharacterSize(80);
	sf::Vector2f viewSize = handler->window->getView().getSize();
	gameOverText.setPosition(
		viewSize.x / 2 - gameOverText.getGlobalBounds().width / 2,
		viewSize.y / 2 - 146
	);
	gameOverText.setFillColor(sf::Color(145, 4, 4, gameOverFade * 255));

	handler->window->draw(gameOverText);

	sf::Text deathText(deathMessage, guiFont);
	deathText.setCharacterSize(30);
	deathText.setPosition(
		viewSize.x / 2 - deathText.getGlobalBounds().width / 2,
		gameOverText.getPosition().y + gameOverText.getGlobalBounds().height + 10
	);
	deathText.setFillColor(sf::Color(255, 232, 232, gameOverFade * 255));

	handler->window->draw(deathText);

	int resId = 0;
	int exitId = 2;
	if (hoveringDeathRespawn) resId = 1;
	if (hoveringDeathExit) exitId = 3;

	sf::Texture* respawnButtonTex = handler->assets->getDeathScreenButton(resId);
	sf::Texture* exitButtonTex = handler->assets->getDeathScreenButton(exitId);

	sf::RectangleShape respawnButton(sf::Vector2f(respawnButtonTex->getSize()));
	respawnButton.setPosition(viewSize.x / 2 - respawnButton.getSize().x - 20,
		deathText.getPosition().y + deathText.getGlobalBounds().height + 10);
	respawnButton.setTexture(respawnButtonTex);

	respawnButtonX = respawnButton.getPosition().x;
	respawnButtonY = respawnButton.getPosition().y;

	sf::RectangleShape exitButton(sf::Vector2f(exitButtonTex->getSize()));
	exitButton.setPosition(viewSize.x / 2 + 20,
		deathText.getPosition().y + deathText.getGlobalBounds().height + 10);
	exitButton.setTexture(exitButtonTex);

	exitButtonX = exitButton.getPosition().x;
	exitButtonY = exitButton.getPosition().y;

	buttonWidth = exitButton.getSize().x;
	buttonHeight = exitButton.getSize().y;

	buttonInfoSet = true;

	handler->window->draw(respawnButton);
	handler->window->draw(exitButton);

}

void PlayingState::pause() {
	bgMusic.pause();
}

void PlayingState::resume() {
	if (bgMusic.getStatus() != sf::Music::Playing && !musicPaused)
		bgMusic.play();
}

void PlayingState::mouseClicked(sf::Event e) {
	if (e.type != sf::Event::MouseButtonPressed && e.type != sf::Event::MouseButtonReleased) {
		return;
	}


	handler->inputManager->usingController = false;

	if (e.type == sf::Event::MouseButtonPressed) {
		if (handler->inputManager->mouseIsPressed || e.mouseButton.button != sf::Mouse::Button::Left) {
			return;
		} else {
			handler->inputManager->mouseIsPressed = true;

			if (deathScreen) {

				if (hoveringDeathExit) {
					Player* p = handler->player;
					p->dropItems();
					p->setHealth(p->getMaxHealth());
					p->setStamina(p->getMaxStamina());
					p->setMagic(p->getMaxMagic());
					p->setCurrentInteracting(nullptr);
					p->setRiding(nullptr);
					p->setStamIsRegeningSlowly(false);
					p->setWorld(handler->mainWorld);
					p->setPos(handler->mainWorld->getSpawn().x * 96, handler->mainWorld->getSpawn().y * 96);
					deathScreen = false;
					deathFade = 0;
					gameOverFade = 0;
					handler->window->close();
				}

				if (hoveringDeathRespawn) {
					Player* p = handler->player;
					p->dropItems();
					p->setHealth(p->getMaxHealth());
					p->setStamina(p->getMaxStamina());
					p->setMagic(p->getMaxMagic());
					p->setCurrentInteracting(nullptr);
					p->setRiding(nullptr);
					p->setStamIsRegeningSlowly(false);
					p->setWorld(handler->mainWorld);
					p->setPos(handler->mainWorld->getSpawn().x * 96, handler->mainWorld->getSpawn().y * 96);
					deathScreen = false;
					deathFade = 0;
					gameOverFade = 0;
				}


			} else {

				int hotBarItemId = handler->player->getItemInfoInHotBar().first;
				int hotBarItemAmount = handler->player->getItemInfoInHotBar().second;

				if (world->highlightIsGood() && ItemMeta::itemIsPlacable(hotBarItemId) && hotBarItemAmount >= 1) {
					ItemMeta::placeItem(hotBarItemId, world, handler);
				} else if (ItemMeta::itemIsUsable(hotBarItemId) && hotBarItemAmount >= 1) {
					ItemMeta::useItem(hotBarItemId, world, handler);
				}
			}
		}
	} else {
		handler->inputManager->mouseIsPressed = false;
	}
}

void PlayingState::updateMouse() {

	if (deathScreen) {
		sf::Vector2f v = handler->window->getView().getSize();
		sf::Vector2u w = handler->window->getSize();
		int mx = v.x * sf::Mouse::getPosition(*(handler->window)).x / w.x;
		int my = v.y * sf::Mouse::getPosition(*(handler->window)).y / w.y;

		if (buttonInfoSet) {

			if (mx >= respawnButtonX && mx < respawnButtonX + buttonWidth &&
				my >= respawnButtonY && my < respawnButtonY + buttonHeight) {
				hoveringDeathRespawn = true;
			} else {
				hoveringDeathRespawn = false;

				if (mx >= exitButtonX && mx < exitButtonX + buttonWidth &&
					my >= exitButtonY && my < exitButtonY + buttonHeight) {
					hoveringDeathExit = true;
				} else {
					hoveringDeathExit = false;
				}

			}
		}


	} else {

		sf::Vector2i mp = sf::Mouse::getPosition(*(handler->window));
		if (!(mp.x < 0 || mp.x > handler->window->getSize().x ||
			mp.y < 0 || mp.y > handler->window->getSize().y) &&
			ItemMeta::itemIsPlacable(handler->player->getItemInfoInHotBar().first) && handler->player->getItemInfoInHotBar().second >= 1 && world != nullptr) {

			sf::Vector2f v = handler->worldView.getSize();
			sf::Vector2u w = handler->window->getSize();
			int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x) - ((v.x - w.x) / 2) + handler->camera->getXOffset();
			int my = sf::Mouse::getPosition(*(handler->window)).y * (v.y / w.y) - ((v.y - w.y) / 2) + handler->camera->getYOffset();
			world->setHighlightedTile(mx / 96, my / 96);

			sf::Vector2i htp = world->getHighlightedTile();

			// Check if the block is too far away
			sf::Vector2f playerPos(handler->player->getX() + handler->player->getWidth() / 2, handler->player->getY() + handler->player->getHeight());
			int xDist = (int)(playerPos.x / 96) - htp.x;
			int yDist = (int)(playerPos.y / 96) - htp.y;


			if (xDist*xDist + yDist * yDist > 5) {
				world->setHighlightGood(false);
			} else {
				world->setHighlightGood(true);
			}
		}
	}
}


void PlayingState::updateJoystick(sf::Int32 dt) {

	InputManager* im = handler->inputManager;

	float jX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	float jY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	float jX2 = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
	float jY2 = sf::Joystick::getAxisPosition(0, sf::Joystick::V);

	bool updateMouse = false;

	// Move the player
	if (jX <= -30) {
		im->usingController = true;
		im->keys[3] = true;
		im->keys[1] = false;
	} else if (jX >= 30) {
		im->usingController = true;
		im->keys[3] = false;
		im->keys[1] = true;
	} else {
		if (im->usingController) {
			im->keys[3] = false;
			im->keys[1] = false;
		}

	}


	if (jY <= -30) {
		im->usingController = true;
		im->keys[0] = true;
		im->keys[2] = false;
	} else if (jY >= 30) {
		im->usingController = true;
		im->keys[0] = false;
		im->keys[2] = true;
	} else {
		if (im->usingController) {
			im->keys[0] = false;
			im->keys[2] = false;
		}

	}

	sf::Vector2i newPos = sf::Mouse::getPosition();

	// Move the mouse
	if (jX2 <= -30) {
		im->usingController = true;
		newPos.x -= ceil(dt / 1.5f);
		updateMouse = true;
	} else if (jX2 >= 30) {
		im->usingController = true;
		newPos.x += ceil(dt / 1.5f);
		updateMouse = true;
	}


	if (jY2 <= -30) {
		im->usingController = true;
		newPos.y -= ceil(dt / 1.5f);
		updateMouse = true;
	} else if (jY2 >= 30) {
		im->usingController = true;
		newPos.y += ceil(dt / 1.5f);
		updateMouse = true;
	}

	if (updateMouse)
		sf::Mouse::setPosition(newPos);


	// Check for running
	if (sf::Joystick::isButtonPressed(0, 0)) {
		im->usingController = true;
		im->runningKey = true;
		im->joyStickButtons[0] = true;
	} else {
		if (im->usingController) {
			im->runningKey = false;
			im->joyStickButtons[0] = false;
		}
	}

	// Check for attacking
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) <= -80) {
		im->usingController = true;
		im->attackKey = true;
	} else {
		if (im->usingController) {
			im->attackKey = false;
		}
	}


	//Handle zoom
	float dPadPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
	if (dPadPos >= 50) {
		im->usingController = true;
		im->zoom += .005f;
		if (im->zoom > 1.2f)
			im->zoom = 1.2f;
		if (im->zoom < .5f)
			im->zoom = .5f;
	} else if (dPadPos <= -50) {
		im->usingController = true;
		im->zoom -= .005f;
		if (im->zoom > 1.2f)
			im->zoom = 1.2f;
		if (im->zoom < .55f)
			im->zoom = .55f;
	}

	// Use item
	if (sf::Joystick::isButtonPressed(0, 1)) {
		im->joyStickButtons[1] = true;
		im->usingController = true;
		if (im->mouseIsPressed) {
			return;
		} else {
			im->mouseIsPressed = true;
			sf::Vector2i htp = world->getHighlightedTile();

			int hotBarItemId = handler->player->getItemInfoInHotBar().first;
			int hotBarItemAmount = handler->player->getItemInfoInHotBar().second;

			if (world->highlightIsGood() && ItemMeta::itemIsPlacable(hotBarItemId) && hotBarItemAmount >= 1) {
				ItemMeta::placeItem(hotBarItemId, world, handler);
			} else if (ItemMeta::itemIsUsable(hotBarItemId) && hotBarItemAmount >= 1) {
				ItemMeta::useItem(hotBarItemId, world, handler);
			}

		}
	} else {
		im->joyStickButtons[1] = false;
		if (im->usingController) {
			im->mouseIsPressed = false;
		}

	}

	// Open Inventory
	if (sf::Joystick::isButtonPressed(0, 6)) {
		bool prevValue = im->joyStickButtons[6];
		im->joyStickButtons[6] = true;
		im->usingController = true;
		if (!prevValue) {
			handler->setGameState(INVENTORY);
		}

	} else {
		im->joyStickButtons[6] = false;
	}

	// Pause Game
	if (sf::Joystick::isButtonPressed(0, 7)) {
		bool prevValue = im->joyStickButtons[7];
		im->joyStickButtons[7] = true;
		im->usingController = true;
		if (!prevValue) {
			handler->setGameState(MAIN_MENU);
		}

	} else {
		im->joyStickButtons[7] = false;
	}

	// Change hotbar slot

	int curSlot = handler->player->getHotBarSlot();

	if (sf::Joystick::isButtonPressed(0, 5)) {
		bool prevValue = im->joyStickButtons[5];
		im->joyStickButtons[5] = true;
		im->usingController = true;
		if (!prevValue) {
			curSlot++;
			if (curSlot > 8) {
				curSlot = 0;
			}
		}
	} else {
		im->joyStickButtons[5] = false;
	}

	if (sf::Joystick::isButtonPressed(0, 4)) {
		bool prevValue = im->joyStickButtons[4];
		im->joyStickButtons[4] = true;
		im->usingController = true;
		if (!prevValue) {
			curSlot--;
			if (curSlot < 0) {
				curSlot = 8;
			}
		}
	} else {
		im->joyStickButtons[4] = false;
	}

	handler->player->setHotBarSlot(curSlot);

	if (sf::Joystick::isButtonPressed(0, 3)) {
		bool prevValue = im->joyStickButtons[3];
		im->joyStickButtons[3] = true;
		im->usingController = true;
		if (!prevValue) {
			handler->player->interact();
		}

	} else {
		im->joyStickButtons[3] = false;
	}

}

void PlayingState::renderTime() {

	if (darknessPercent == 0)
		return;


	sf::View v = handler->window->getView();

	sf::Shader* shader = handler->assets->getShader();

	shader->setUniformArray("lights", &lightArr[0], lightArr.size());
	shader->setUniform("lightSize", (int)lightArr.size());

	shader->setUniformArray("walls", &wallArr[0], wallArr.size());
	shader->setUniform("wallSize", (int)wallArr.size());

	shader->setUniform("darknessPercent", darknessPercent);

	shader->setUniform("sizeMultiplier", (float)(handler->window->getSize().x / handler->window->getView().getSize().x));

	shader->setUniform("numBuildings", (int)bldgTextureArr.size());

	//std::cout << lightArr.size() << " " << wallArr.size() << " " << bldgTextureArr.size() << std::endl;

	for (int i = 0; i < bldgTextureArr.size(); i++) {

		std::stringstream ss;
		ss << "buildings[" << i << "]";

		shader->setUniform(ss.str(), *bldgTextureArr[i]);

		std::stringstream ss2;
		ss2 << "buildingPositions[" << i << "]";

		shader->setUniform(ss2.str(), bldgPosArr[i]);
	}

	sf::RectangleShape lighting(sf::Vector2f(handler->window->getView().getSize()));
	lighting.setPosition((handler->window->getSize().x / 2 - v.getSize().x / 2), (handler->window->getSize().y / 2 - v.getSize().y / 2));
	handler->window->draw(lighting, shader);

}

void PlayingState::addLightPoint(sf::Vector2f point, float spread, float extraDist) {

	sf::View v = handler->window->getView();

	float x = (point.x + ((v.getSize().x - handler->window->getSize().x) / 2)) / (v.getSize().x / handler->window->getSize().x);
	float y = (point.y + ((v.getSize().y - handler->window->getSize().y) / 2)) / (v.getSize().y / handler->window->getSize().y);

	lightArr.push_back(sf::Glsl::Vec4(x, y, spread, extraDist));

}
void PlayingState::addWallLine(sf::Vector2f pointA, sf::Vector2f pointB) {
	sf::View v = handler->window->getView();

	float x1 = (pointA.x + ((v.getSize().x - handler->window->getSize().x) / 2)) / (v.getSize().x / handler->window->getSize().x);
	float y1 = (pointA.y + ((v.getSize().y - handler->window->getSize().y) / 2)) / (v.getSize().y / handler->window->getSize().y);

	float x2 = (pointB.x + ((v.getSize().x - handler->window->getSize().x) / 2)) / (v.getSize().x / handler->window->getSize().x);
	float y2 = (pointB.y + ((v.getSize().y - handler->window->getSize().y) / 2)) / (v.getSize().y / handler->window->getSize().y);


	wallArr.push_back(sf::Glsl::Vec4(x1, y1, x2, y2));
}

void PlayingState::addBuildingTexture(const sf::Texture* texture, sf::Vector2f pos) {


	bldgTextureArr.push_back(texture);

	sf::View v = handler->window->getView();

	sf::Vector2f bldgPos = pos;
	bldgPos.x = (bldgPos.x + ((v.getSize().x - handler->window->getSize().x) / 2)) / (v.getSize().x / handler->window->getSize().x);
	bldgPos.y = (bldgPos.y + ((v.getSize().y - handler->window->getSize().y) / 2)) / (v.getSize().y / handler->window->getSize().y);

	bldgPosArr.push_back(bldgPos);

}

void PlayingState::loadStateFile() {
	std::ifstream file(handler->saveDirName + "\\" + "Gamestate.tgs", std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Could not open gamestate file!" << std::endl;
		return;
	}

	file.read((char*)&time, sizeof(sf::Int32));

	file.close();
}

void PlayingState::saveStateFile() {
	std::ofstream file(handler->saveDirName + "\\" + "Gamestate.tgs", std::ios::out | std::ios::trunc | std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Could not save gamestate file!" << std::endl;
		return;
	}

	file.write((char*)&time, sizeof(sf::Int32));

	file.close();

}

void PlayingState::spawnEnemies() {



	int playerTileX = handler->player->getX() / 96;
	int playerTileY = handler->player->getY() / 96;

	// Spawn at most 16 tiles away from player
	int maxTileDist = 16;
	int minTileDist = 6;
	int buildingDistSafety = 5;

	for (int tileX = playerTileX - maxTileDist, distX = -maxTileDist; tileX <= playerTileX + maxTileDist; tileX++, distX++) {
		for (int tileY = playerTileY - maxTileDist, distY = -maxTileDist; tileY <= playerTileY + maxTileDist; tileY++, distY++) {

			if (!world->tileIsSolid(tileX * 96, tileY * 96) && !world->tileIsWater(tileX * 96, tileY * 96) && Static::tileIsEmpty(tileX, tileY, world)) {
				if (distX * distX + distY * distY >= minTileDist * minTileDist) {

					bool canSpawn = true;

					std::vector<Building*> buildings = Building::getAllBuildings();
					for (int i = 0; i < buildings.size(); i++) {
						if (buildings[i]->getOutWorld() == world) {

							Building* b = buildings[i];
							int bTileX = b->getX() + b->getWidth() / 2;
							int bTileY = b->getY() + b->getHeight() / 2;

							int bDistX = bTileX - tileX;
							int bDistY = bTileY - tileY;

							if (bDistX * bDistX + bDistY * bDistY < buildingDistSafety * buildingDistSafety) {
								canSpawn = false;
								break;
							}

						}
					}



					if (canSpawn) {
						// Try to spawn

						if (rand() % (int)((1.f - 2 * darknessPercent / 3.f) * 2500) == 0) {

							if (world->getEntityManager()->getNumPathfinders() < world->getMaxNumPathfinders()) {
								// Decide what to spawn
								if (rand() % 5 < 3) {
									Zombie* z = new Zombie(tileX * 96, tileY * 96, handler, world);
									z->setFollowing(handler->player);
									std::cout << "Spawned zombie at: " << tileX << ", " << tileY << std::endl;
								} else {
									Skeleton* s = new Skeleton(tileX * 96, tileY * 96, handler, world);
									s->setFollowing(handler->player);
									std::cout << "Spawned skeleton at: " << tileX << ", " << tileY << std::endl;
								}
							}

						}
					}

				}



			}
		}
	}
}