#include "LoadingState.h"
#include "PlayingState.h"
#include <iostream>
#include "Assets.h"
#include <sstream>

using namespace tg;

LoadingState::LoadingState(Handler* handler) : GameState(LOADING), handler(handler), loader(handler) {

}


LoadingState::~LoadingState() {
}

// Render the loading bar, and title
void LoadingState::render() {

	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(handler->window->getView().getSize().x, handler->window->getView().getSize().y));
	bg.setFillColor(sf::Color::Black);

	handler->window->draw(bg);

	sf::Text loadingMessage;
	if (guiFont.getInfo().family == "") {
		return;
	}
	loadingMessage.setFont(guiFont);

	std::stringstream ss;
	ss << loader.getLoadingMessage();



	loadingMessage.setString(ss.str());
	loadingMessage.setCharacterSize(20);
	loadingMessage.setPosition(handler->window->getView().getSize().x / 2 - loadingMessage.getGlobalBounds().width / 2,
		handler->window->getView().getSize().y - 60 - loadingMessage.getGlobalBounds().height);

	handler->window->draw(loadingMessage);

	sf::Text signature;
	signature.setFont(guiFont);

	signature.setString("Created by Jacob Hofer");
	signature.setCharacterSize(45);
	signature.setPosition(handler->window->getView().getSize().x / 2 - signature.getGlobalBounds().width / 2,
		handler->window->getView().getSize().y / 2 - signature.getGlobalBounds().height);
	if (transp > 255) transp = 255;
	signature.setFillColor(sf::Color(255, 255, 255, (int)transp));
	handler->window->draw(signature);

}

// Update the loading percent
void LoadingState::tick(sf::Int32 dt) {

	if (!beganLoading) {
		loader.beginLoading();
		beganLoading = true;
	}

	transp += dt / 5.f;

	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}

	if (loader.isFinished()) {
		handler->setGameState(PLAYING);
	}



}

// Stop loading
void LoadingState::exit() {
	loader.exit();
}
