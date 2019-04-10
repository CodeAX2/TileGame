#include "LoadingState.h"
#include "PlayingState.h"
#include <iostream>
#include "Assets.h"
#include <sstream>
#include "Game.h"
#include "resource.h"

using namespace tg;

LoadingState::LoadingState(Handler* handler) : GameState(LOADING), handler(handler), loader(handler) {
	controlsLayout = handler->assets->loadTextureFromResource(CONTROLS_LAYOUT);
}


LoadingState::~LoadingState() {
}

// Render the loading bar, and title
void LoadingState::render() {

	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(handler->window->getView().getSize().x, handler->window->getView().getSize().y));
	bg.setFillColor(sf::Color::Black);
	handler->window->draw(bg);

	sf::RectangleShape controls;
	controls.setSize(sf::Vector2f(handler->window->getView().getSize().x * 3.f / 4, handler->window->getView().getSize().y * 3.f / 4));
	controls.setPosition(sf::Vector2f(handler->window->getView().getSize().x / 8, handler->window->getView().getSize().y / 8));
	controls.setTexture(controlsLayout);
	handler->window->draw(controls);


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
		handler->window->getView().getSize().y - 10 - loadingMessage.getGlobalBounds().height);

	handler->window->draw(loadingMessage);




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
