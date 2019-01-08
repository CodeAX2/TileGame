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





}


MainMenuState::~MainMenuState() {


	delete mainMenuBG;
	for (sf::Texture* t : mainMenuButtons) {
		delete t;
	}

}

void MainMenuState::render() {

	sf::RectangleShape bg(sf::Vector2f(1280, 720));
	bg.setTexture(mainMenuBG);

	handler->window->draw(bg);

	sf::RectangleShape playButton(sf::Vector2f(361,132));
	playButton.setPosition(459, 405);
	playButton.setTexture(mainMenuButtons[0]);
	handler->window->draw(playButton);

	sf::RectangleShape exitButton(sf::Vector2f(361, 132));
	exitButton.setPosition(459, 553);
	exitButton.setTexture(mainMenuButtons[2]);
	handler->window->draw(exitButton);



}

void MainMenuState::tick(sf::Int32 dt) {
}

void MainMenuState::exit() {

}
