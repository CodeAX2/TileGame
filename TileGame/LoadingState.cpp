#include "LoadingState.h"
#include "PlayingState.h"
#include <iostream>
#include "Assets.h"
#include <sstream>

using namespace tg;

LoadingState::LoadingState(Handler* handler) : GameState(LOADING), handler(handler), loader(handler) {


	loader.beginLoading();

}


LoadingState::~LoadingState() {
}

// Render the loading bar, and title
void LoadingState::render() {

	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(handler->window->getSize().x, handler->window->getSize().y));
	bg.setFillColor(sf::Color::Black);

	handler->window->draw(bg);

	sf::Text loadingMessage;
	if (loadingFont == nullptr) {
		return;
	}
	loadingMessage.setFont(*loadingFont);

	std::stringstream ss;
	ss << loader.getLoadingMessage();



	loadingMessage.setString(ss.str());
	loadingMessage.setCharacterSize(20);
	loadingMessage.setPosition(handler->window->getSize().x / 2 - loadingMessage.getGlobalBounds().width / 2,
		handler->window->getSize().y - 60 - loadingMessage.getGlobalBounds().height);

	handler->window->draw(loadingMessage);

	sf::Text signature;
	signature.setFont(*loadingFont);

	signature.setString("Created by Jacob Hofer");
	signature.setCharacterSize(45);
	signature.setPosition(handler->window->getSize().x / 2 - signature.getGlobalBounds().width / 2,
		handler->window->getSize().y / 2 - signature.getGlobalBounds().height);
	if (transp > 255) transp = 255;
	signature.setFillColor(sf::Color(255, 255, 255, (int)transp));
	handler->window->draw(signature);

}

// Update the loading percent
void LoadingState::tick(sf::Int32 dt) {

	transp += dt / 5.f;

	if (loadingFont == nullptr) {
		loadingFont = handler->assets->getArialiFont();
	}

	if (loader.isFinished()) {
		handler->setGameState(PLAYING);
	}



}

// Stop loading
void LoadingState::exit() {
	loader.exit();
}
