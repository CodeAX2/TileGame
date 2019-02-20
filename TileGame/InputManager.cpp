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

		handler->getCurrentState()->updateJoystick(dt);

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
	case sf::Keyboard::Tab:
		if (value) {

			if (handler->getCurrentState()->getType() == PLAYING) {
				handler->setGameState(INVENTORY);
			} else if (handler->getCurrentState()->getType() == INVENTORY) {
				handler->setGameState(PLAYING);
			}
		}
		break;

	case sf::Keyboard::E:
		if (value) {

			handler->player->interact();
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