#include "MainMenuState.h"
#include "Assets.h"
#include "resource.h"


MainMenuState::MainMenuState(Handler* handler) : GameState(MAIN_MENU), handler(handler) {

	mainMenuBG = new sf::Texture();
	mainMenuBG->loadFromImage(handler->assets->loadImageFromResource(MENU_MAIN));

	sf::Image buttons = handler->assets->loadImageFromResource(MENU_MAIN_BUTTONS);

	int height = 132;
	for (int i = 0; i < 4; i++) {
		sf::Texture* curButton = new sf::Texture();
		curButton->loadFromImage(buttons, sf::IntRect(0, i*height, 361, height));
		mainMenuButtons.push_back(curButton);
	}

	mainMenuButtonsPos.push_back(sf::Vector2i(459, 404));
	mainMenuButtonsPos.push_back(sf::Vector2i(459, 552));


	mainMenuButtonHovering.push_back(false);
	mainMenuButtonHovering.push_back(false);
}


MainMenuState::~MainMenuState() {


	delete mainMenuBG;
	for (sf::Texture* t : mainMenuButtons) {
		delete t;
	}

}

void MainMenuState::render() {

	handler->worldView = sf::View(sf::Vector2f(handler->window->getSize().x / 2, handler->window->getSize().y / 2), sf::Vector2f(1280.f, 720.f));
	handler->window->setView(handler->guiView);



	sf::RectangleShape bg(sf::Vector2f(1280, 720));
	bg.setTexture(mainMenuBG);

	handler->window->draw(bg);

	sf::RectangleShape playButton(sf::Vector2f(361, 134));
	playButton.setPosition(sf::Vector2f(mainMenuButtonsPos[0]));
	playButton.setTexture(mainMenuButtons[0 + mainMenuButtonHovering[0]]);
	handler->window->draw(playButton);

	sf::RectangleShape exitButton(sf::Vector2f(361, 134));
	exitButton.setPosition(sf::Vector2f(mainMenuButtonsPos[1]));
	exitButton.setTexture(mainMenuButtons[2 + mainMenuButtonHovering[1]]);
	handler->window->draw(exitButton);



}

void MainMenuState::tick(sf::Int32 dt) {
}

void MainMenuState::exit() {

}


void MainMenuState::setButtonHover(bool hovering, int buttonId) {
	mainMenuButtonHovering[buttonId] = hovering;
}

void MainMenuState::mouseClicked(sf::Event e) {

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

	sf::Vector2i buttonSize(361, 132);

	for (int i = 0; i < buttonPos.size(); i++) {
		sf::IntRect buttonBounds(buttonPos[i], buttonSize);
		if (buttonBounds.contains(mp)) {
			if (i == 0) {
				handler->setGameState(LOADING);
				return;
			} else if (i == 1) {
				handler->window->close();
				return;
			}
		}
	}
}

void MainMenuState::updateMouse() {

	sf::Vector2f v = handler->window->getView().getSize();
	sf::Vector2u w = handler->window->getSize();
	int mx = v.x * sf::Mouse::getPosition(*(handler->window)).x / w.x;
	int my = v.y * sf::Mouse::getPosition(*(handler->window)).y / w.y;

	sf::Vector2i mp(mx, my);
	MainMenuState* state = dynamic_cast<MainMenuState*>(handler->getCurrentState());

	std::vector<sf::Vector2i> buttonPos = state->getButtonPositions();

	sf::Vector2i buttonSize(361, 134);

	for (int i = 0; i < buttonPos.size(); i++) {
		sf::IntRect buttonBounds(buttonPos[i], buttonSize);
		if (buttonBounds.contains(mp)) {
			state->setButtonHover(true, i);
		} else {
			state->setButtonHover(false, i);
		}
	}

}