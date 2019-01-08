#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>
#include "Handler.h"

using namespace tg;

class MainMenuState :
	public GameState
{

private:
	sf::Texture* mainMenuBG;
	std::vector<sf::Texture*> mainMenuButtons;
	std::vector<sf::Vector2i> mainMenuButtonsPos;
	std::vector<bool> mainMenuButtonHovering;
	Handler* handler;

public:
	MainMenuState(Handler* handler);
	~MainMenuState();

public:
	void render() override;
	void tick(sf::Int32 dt) override;
	void exit() override;

	std::vector<sf::Vector2i> getButtonPositions() { return mainMenuButtonsPos; }
	void setButtonHover(bool hovering, int buttonId);

	bool hoveringOverButton(int id) { return mainMenuButtonHovering[id]; }


};

