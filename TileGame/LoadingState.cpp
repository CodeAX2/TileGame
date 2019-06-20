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

	int opacity = timeOpen * 255 / 500;
	if (finished) {
		opacity = (250 - finishedFadeTransition) * 255 / 250;
	}
	if (opacity > 255) opacity = 255;

	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(handler->window->getView().getSize().x, handler->window->getView().getSize().y));
	bg.setFillColor(sf::Color::Black);
	handler->window->draw(bg);

	sf::RectangleShape controls;
	controls.setSize(sf::Vector2f(handler->window->getView().getSize().x * 3.f / 4, handler->window->getView().getSize().y * 3.f / 4));
	controls.setPosition(sf::Vector2f(handler->window->getView().getSize().x / 8, handler->window->getView().getSize().y / 8));
	controls.setTexture(controlsLayout);
	controls.setFillColor(sf::Color(255, 255, 255, opacity));
	handler->window->draw(controls);


	sf::Text loadingMessage;
	if (guiFont.getInfo().family == "") {
		return;
	}
	loadingMessage.setFont(guiFont);
	loadingMessage.setFillColor(sf::Color(255, 255, 255, opacity));

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
	timeOpen += dt;

	if (finished) {
		finishedFadeTransition += dt;
		if (finishedFadeTransition >= 250) {
			handler->setGameState(PLAYING);
		}
	}

	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}

	if (loader.isFinished()) {
		finished = true;
		loaded = true;
	}



}

// Stop loading
void LoadingState::exit() {
	loader.exit();
}
