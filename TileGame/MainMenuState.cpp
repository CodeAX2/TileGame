#include "MainMenuState.h"
#include "Assets.h"
#include "resource.h"
#include "InputManager.h"
#include "LoadingState.h"

MainMenuState::MainMenuState(Handler* handler) : GameState(MAIN_MENU), handler(handler) {

	mainMenuBG = handler->assets->loadTextureFromResource(MENU_MAIN);

	sf::Image buttons = handler->assets->loadImageFromResource(MENU_MAIN_BUTTONS);

	int height = 132;
	for (int i = 0; i < 4; i++) {
		sf::Texture* curButton = new sf::Texture();
		curButton->loadFromImage(buttons, sf::IntRect(0, i*height, 361, height));
		mainMenuButtons.push_back(curButton);
	}

	mainMenuButtonsPos.push_back(sf::Vector2i(459 * 1.5, 404 * 1.5));
	mainMenuButtonsPos.push_back(sf::Vector2i(459 * 1.5, 552 * 1.5));


	mainMenuButtonHovering.push_back(false);
	mainMenuButtonHovering.push_back(false);

	version = handler->assets->getVersion();

}


MainMenuState::~MainMenuState() {


	delete mainMenuBG;
	for (sf::Texture* t : mainMenuButtons) {
		delete t;
	}

}

void MainMenuState::render() {

	int opacity = timeOpen * 255 / 1000;
	if (selectedPlay) {
		opacity = (500 - playFadeTransition) * 255 / 500;
	}
	if (opacity > 255) opacity = 255;

	handler->worldView = sf::View(sf::Vector2f(handler->window->getSize().x / 2, handler->window->getSize().y / 2), sf::Vector2f(1920.f, 1080.f));
	handler->window->setView(handler->guiView);



	sf::RectangleShape bg(sf::Vector2f(1280 * 1.5, 720 * 1.5));
	bg.setTexture(mainMenuBG);
	bg.setFillColor(sf::Color(255, 255, 255, opacity));
	handler->window->draw(bg);

	sf::RectangleShape playButton(sf::Vector2f(361 * 1.5, 134 * 1.5));
	playButton.setPosition(sf::Vector2f(mainMenuButtonsPos[0]));
	playButton.setTexture(mainMenuButtons[0 + mainMenuButtonHovering[0]]);
	playButton.setFillColor(sf::Color(255, 255, 255, opacity));
	handler->window->draw(playButton);

	sf::RectangleShape exitButton(sf::Vector2f(361 * 1.5, 134 * 1.5));
	exitButton.setPosition(sf::Vector2f(mainMenuButtonsPos[1]));
	exitButton.setTexture(mainMenuButtons[2 + mainMenuButtonHovering[1]]);
	exitButton.setFillColor(sf::Color(255, 255, 255, opacity));
	handler->window->draw(exitButton);

	sf::Text versionText;
	versionText.setString("Version: " + version);
	versionText.setFont(guiFont);
	versionText.setCharacterSize(20 * 1.5);
	versionText.setPosition(5, handler->window->getView().getSize().y - versionText.getGlobalBounds().height);
	versionText.setFillColor(sf::Color(255, 255, 255, opacity));
	handler->window->draw(versionText);


}

void MainMenuState::tick(sf::Int32 dt) {
	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}
	timeOpen += dt;
	if (selectedPlay) {
		playFadeTransition += dt;
		if (playFadeTransition >= 500) {
			if (LoadingState* ls = dynamic_cast<LoadingState*>(handler->getCustomState(LOADING))) {
				if (ls->isLoaded()) {
					handler->setGameState(PLAYING);
				}
			}
			handler->setGameState(LOADING);
		}
	}
}

void MainMenuState::exit() {

}


void MainMenuState::setButtonHover(bool hovering, int buttonId) {
	mainMenuButtonHovering[buttonId] = hovering;
}

void MainMenuState::mouseClicked(sf::Event e) {

	if (timeOpen < 1000 || selectedPlay) return;

	if (e.type != sf::Event::MouseButtonPressed && e.type != sf::Event::MouseButtonReleased) {
		return;
	}

	sf::Vector2f v = handler->window->getView().getSize();
	sf::Vector2u w = handler->window->getSize();
	int mx = v.x * sf::Mouse::getPosition(*(handler->window)).x / w.x;
	int my = v.y * sf::Mouse::getPosition(*(handler->window)).y / w.y;

	sf::Vector2i mp(mx, my);
	MainMenuState* state = dynamic_cast<MainMenuState*>(handler->getCurrentState());

	std::vector<sf::Vector2i> buttonPos = state->getButtonPositions();

	sf::Vector2i buttonSize(361 * 1.5, 132 * 1.5);

	for (int i = 0; i < buttonPos.size(); i++) {
		sf::IntRect buttonBounds(buttonPos[i], buttonSize);
		if (buttonBounds.contains(mp)) {
			if (i == 0) {
				selectedPlay = true;
				return;
			} else if (i == 1) {
				handler->window->close();
				return;
			}
		}
	}
}

void MainMenuState::updateMouse() {

	if (timeOpen < 1000 || selectedPlay) return;

	sf::Vector2f v = handler->window->getView().getSize();
	sf::Vector2u w = handler->window->getSize();
	int mx = v.x * sf::Mouse::getPosition(*(handler->window)).x / w.x;
	int my = v.y * sf::Mouse::getPosition(*(handler->window)).y / w.y;

	sf::Vector2i mp(mx, my);
	MainMenuState* state = dynamic_cast<MainMenuState*>(handler->getCurrentState());

	std::vector<sf::Vector2i> buttonPos = state->getButtonPositions();

	sf::Vector2i buttonSize(361 * 1.5, 134 * 1.5);

	for (int i = 0; i < buttonPos.size(); i++) {
		sf::IntRect buttonBounds(buttonPos[i], buttonSize);
		if (buttonBounds.contains(mp)) {
			state->setButtonHover(true, i);
		} else {
			state->setButtonHover(false, i);
		}
	}

}

void MainMenuState::updateJoystick(sf::Int32 dt) {

	if (timeOpen < 1000 || selectedPlay) return;

	InputManager* im = handler->inputManager;

	float jX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	float jY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	sf::Vector2f v = handler->window->getView().getSize();
	sf::Vector2u w = handler->window->getSize();
	sf::Vector2i buttonSize(361 * 1.5, 134 * 1.5);

	if (jY <= -30) {
		im->usingController = true;
		if (prevJoystickPos == 0) {
			// Move down a selection
			controllerSelectedButton++;
			if (controllerSelectedButton > 1)
				controllerSelectedButton = 0;
			std::vector<sf::Vector2i> buttonPos = getButtonPositions();
			sf::Vector2i curButton = buttonPos[controllerSelectedButton];
			curButton += buttonSize / 2;
			curButton.x *= w.x / v.x;
			curButton.y *= w.y / v.y;

			sf::Mouse::setPosition(curButton, *(handler->window));
		}

		prevJoystickPos = -30;

	} else if (jY >= 30) {

		im->usingController = true;
		if (prevJoystickPos == 0) {
			// Move up a selection
			controllerSelectedButton--;
			if (controllerSelectedButton < 0)
				controllerSelectedButton = 1;

			std::vector<sf::Vector2i> buttonPos = getButtonPositions();
			sf::Vector2i curButton = buttonPos[controllerSelectedButton];
			curButton += buttonSize / 2;
			curButton.x *= w.x / v.x;
			curButton.y *= w.y / v.y;

			sf::Mouse::setPosition(curButton, *(handler->window));
		}

		prevJoystickPos = 30;

	} else {
		prevJoystickPos = 0;
	}




	// Select an option
	if (sf::Joystick::isButtonPressed(0, 1)) {
		bool prevValue = im->joyStickButtons[1];
		im->joyStickButtons[1] = true;
		if (!prevValue) {

			int mx = v.x * sf::Mouse::getPosition(*(handler->window)).x / w.x;
			int my = v.y * sf::Mouse::getPosition(*(handler->window)).y / w.y;

			sf::Vector2i mp(mx, my);
			MainMenuState* state = dynamic_cast<MainMenuState*>(handler->getCurrentState());

			std::vector<sf::Vector2i> buttonPos = state->getButtonPositions();

			sf::Vector2i buttonSize(361 * 1.5, 132 * 1.5);

			for (int i = 0; i < buttonPos.size(); i++) {
				sf::IntRect buttonBounds(buttonPos[i], buttonSize);
				if (buttonBounds.contains(mp)) {
					if (i == 0) {
						selectedPlay = true;
						return;
					} else if (i == 1) {
						handler->window->close();
						return;
					}
				}
			}

		}
	} else {
		im->joyStickButtons[1] = false;
	}


}

void MainMenuState::pause() {
	timeOpen = 0;
	playFadeTransition = 0;
	selectedPlay = false;

	for (int i = 0; i < mainMenuButtonHovering.size(); i++) {
		mainMenuButtonHovering[i] = false;
	}

}