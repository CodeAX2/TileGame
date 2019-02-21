#include "ChestInventoryState.h"
#include "Assets.h"
#include "PlayingState.h"
#include "InputManager.h"

using namespace tg;

ChestInventoryState::ChestInventoryState(Handler* handler) : GameState(CHEST_INVENTORY) {
	this->handler = handler;
	invBg = new sf::Texture();
	invBg->loadFromImage(handler->assets->loadImageFromResource(MENU_CHEST_INV));

	invHighlight = new sf::Texture();
	invHighlight->loadFromImage(handler->assets->loadImageFromResource(INV_HIGHLIGHT));

}

ChestInventoryState::~ChestInventoryState() {
	delete invBg;
	delete invHighlight;
}

void ChestInventoryState::render() {
	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}

	handler->getCustomState(PLAYING)->render();
	renderChestInventory();
}

void ChestInventoryState::tick(sf::Int32 dt) {
	handler->getCustomState(PLAYING)->tick(dt);
}

void ChestInventoryState::renderChestInventory() {
	sf::RectangleShape invRect(handler->window->getView().getSize());
	invRect.setTexture(invBg);

	handler->window->draw(invRect);
}

void ChestInventoryState::resume() {
	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	ps->playBGMusic();
}

void ChestInventoryState::pause() {
	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	ps->pauseBGMusic();
}