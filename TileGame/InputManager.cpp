#include "InputManager.h"
#include <iostream>
#include "GameState.h"
#include "Player.h"
#include "Game.h"
#include "World.h"
#include "PlayingState.h"
#include "MainMenuState.h"
#include <SFML/Window.hpp>

using namespace tg;

InputManager::InputManager(Handler* handler) : handler(handler) {}

// Events when the mouse is clicked

void InputManager::handleEvents(sf::Event e, sf::Uint32 dt) {
	mouseClicked(e);
	mouseScrolled(e);
	updateKeys(e);
	updateJoystick(dt);
	updateMouse();
}

void InputManager::updateMouse() {
	handler->getCurrentState()->updateMouse();
}


void InputManager::mouseClicked(sf::Event e) {

	handler->getCurrentState()->mouseClicked(e);

}


// Update the joystick if it is connected
void InputManager::updateJoystick(sf::Uint32 dt) {

	if (sf::Joystick::isConnected(0)) {

		World* world = handler->mainWorld;
		if (handler->getCurrentState()->getType() == PLAYING) {
			PlayingState* ps = (PlayingState*)handler->getCurrentState();
			world = ps->getWorld();
		}

		float jX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float jY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		float jX2 = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
		float jY2 = sf::Joystick::getAxisPosition(0, sf::Joystick::V);

		bool updateMouse = false;

		// Move the player
		if (jX <= -30) {
			usingController = true;
			keys[3] = true;
			keys[1] = false;
		} else if (jX >= 30) {
			usingController = true;
			keys[3] = false;
			keys[1] = true;
		} else {
			if (usingController) {
				keys[3] = false;
				keys[1] = false;
			}

		}


		if (jY <= -30) {
			usingController = true;
			keys[0] = true;
			keys[2] = false;
		} else if (jY >= 30) {
			usingController = true;
			keys[0] = false;
			keys[2] = true;
		} else {
			if (usingController) {
				keys[0] = false;
				keys[2] = false;
			}

		}

		sf::Vector2i newPos = sf::Mouse::getPosition();

		// Move the mouse
		if (jX2 <= -30) {
			usingController = true;
			newPos.x -= ceil(dt / 1.5f);
			updateMouse = true;
		} else if (jX2 >= 30) {
			usingController = true;
			newPos.x += ceil(dt / 1.5f);
			updateMouse = true;
		}


		if (jY2 <= -30) {
			usingController = true;
			newPos.y -= ceil(dt / 1.5f);
			updateMouse = true;
		} else if (jY2 >= 30) {
			usingController = true;
			newPos.y += ceil(dt / 1.5f);
			updateMouse = true;
		}

		if (updateMouse)
			sf::Mouse::setPosition(newPos);


		// Check for running
		if (sf::Joystick::isButtonPressed(0, 0)) {
			usingController = true;
			runningKey = true;
		} else {
			if (usingController) {
				runningKey = false;
			}
		}

		// Check for attacking
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) <= -80) {
			usingController = true;
			attackKey = true;
		} else {
			if (usingController) {
				attackKey = false;
			}
		}


		//Handle zoom
		float dPadPos = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
		if (dPadPos >= 50) {
			usingController = true;
			zoom += .000001f;
			if (zoom > 1.2f)
				zoom = 1.2f;
			if (zoom < .5f)
				zoom = .5f;
		} else if (dPadPos <= -50) {
			usingController = true;
			zoom -= .000001f;
			if (zoom > 1.2f)
				zoom = 1.2f;
			if (zoom < .55f)
				zoom = .55f;
		}

		// Place blocks
		if (sf::Joystick::isButtonPressed(0, 1)) {
			usingController = true;
			if (mouseIsPressed) {
				return;
			} else {
				mouseIsPressed = true;
				// Add blocks
				sf::Vector2i htp = world->getHighlightedTile();

				// Check if the block is too far away

				if (world->highlightIsGood()) {
					if (world->getTile(htp.x, htp.y) == 2 || world->getTile(htp.x, htp.y) == 3) {
						if (handler->player->getInventory()->getAmountOfItem(0) >= 1) {
							world->setTile(htp.x, htp.y, 4);
							handler->player->removeItemFromInv(0);
						}
					} else if (world->getTile(htp.x, htp.y) == 0 && handler->player->getInventory()->getAmountOfItem(0) >= 1) {
						world->setTile(htp.x, htp.y, 5);
						handler->player->removeItemFromInv(0);
					} else if (world->getTile(htp.x, htp.y) == 7 && handler->player->getInventory()->getAmountOfItem(0) >= 1) {
						world->setTile(htp.x, htp.y, 5);
						handler->player->removeItemFromInv(0);
					}
				}

			}
		} else {
			if (usingController) {
				mouseIsPressed = false;
			}

		}


	}
}

// Update based on what keys are pushed
void InputManager::updateKeys(sf::Event e) {

	bool value;
	if (e.type == sf::Event::KeyPressed) {
		value = true;
	} else if (e.type == sf::Event::KeyReleased) {
		value = false;
	} else {
		return;
	}

	usingController = false;

	sf::Keyboard::Key key = e.key.code;
	switch (key) {

	case sf::Keyboard::W:
		if (handler->getCurrentState()->getType() == PLAYING)
			keys[0] = value;

		break;
	case sf::Keyboard::D:
		if (handler->getCurrentState()->getType() == PLAYING)
			keys[1] = value;

		break;
	case sf::Keyboard::S:
		if (handler->getCurrentState()->getType() == PLAYING)
			keys[2] = value;

		break;
	case sf::Keyboard::A:
		if (handler->getCurrentState()->getType() == PLAYING)
			keys[3] = value;

		break;
	case sf::Keyboard::Q:
		if (handler->getCurrentState()->getType() == PLAYING) {
			toggleKey = value;
		}
		break;
	case sf::Keyboard::LShift:
		runningKey = value;
		break;
	case sf::Keyboard::Space:
		if (handler->getCurrentState()->getType() == PLAYING)
			attackKey = value;

		break;
	case sf::Keyboard::F11:
		if (value && handler->getCurrentState()->getType() != LOADING)
			handler->game->toggleFullscreen();
		break;
	case sf::Keyboard::Escape:
		if (value && handler->getCurrentState()->getType() == PLAYING) {
			handler->setGameState(MAIN_MENU);
		} else if (value && handler->getCurrentState()->getType() == INVENTORY) {
			handler->setGameState(PLAYING);
		} else if (value && handler->getCurrentState()->getType() == MAIN_MENU) {
			handler->setGameState(PLAYING);
		}
		break;
	case sf::Keyboard::E:
		if (value) {

			if (handler->getCurrentState()->getType() == PLAYING) {
				handler->setGameState(INVENTORY);
			} else if (handler->getCurrentState()->getType() == INVENTORY) {
				handler->setGameState(PLAYING);
			}
		}
		break;

	default:
		if (handler->getCurrentState()->getType() == PLAYING && value) {
			std::vector<sf::Keyboard::Key>::iterator pos = std::find(numberKeys.begin(), numberKeys.end(), key);
			if (pos != numberKeys.end()) {
				int index = pos - numberKeys.begin();
				handler->player->setHotBarSlot(index);
			}
		}

	}

	if (previousToggleKey != getToggleKey() && getToggleKey()) {
		hitboxShown = !hitboxShown;
	}

	previousToggleKey = getToggleKey();


}

// Get all the directional keys
bool* InputManager::getAllKeys() {
	return keys;
}


void InputManager::mouseScrolled(sf::Event e) {
	if (handler->getCurrentState()->getType() != INVENTORY) {
		if (e.type == sf::Event::MouseWheelScrolled) {
			float delta = e.mouseWheelScroll.delta;
			zoom += delta * .05f;
			if (zoom > 1.2f)
				zoom = 1.2f;
			if (zoom < .55f)
				zoom = .55f;


		}
	}
}

void InputManager::disableCurrentMovement() {
	keys[0] = false;
	keys[1] = false;
	keys[2] = false;
	keys[3] = false;
	runningKey = false;
	attackKey = false;
}