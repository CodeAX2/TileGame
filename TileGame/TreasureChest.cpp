#include "TreasureChest.h"
#include "Item.h"
#include "GameState.h"
#include "ChestInventoryState.h"
#include "InputManager.h"

using namespace tg;

TreasureChest::TreasureChest(int x, int y, Handler* handler, World* world) :
	Static(x, y, handler, 9, 32 * 3 - 25, 32 * 3 - 18, 25, 32 * 3, 32 * 3, true, TREASURE_CHEST_E, 73, world),
	Interactable(handler) {

	this->texture = handler->assets->getTreasureChestTexture();
	this->maxHealth = 60;
	this->health = 60;

	if (contents.size() == 0) {
		for (int i = 0; i < CONTENTS_SIZE; i++) {
			contents.push_back(std::pair<int, int>(-1, -1));
		}
	}

}


TreasureChest::~TreasureChest()
{
}


// Drop items when the treasure chest is broken
void TreasureChest::dropItems() {

	srand(time(NULL));
	rand();

	for (int i = 0; i < contents.size(); i++) {

		for (int j = 0; j < contents[i].second; j++) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, contents[i].first, world);
		}


	}

	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 8, world);

}

// Set the amount of a specific item in the chest
void TreasureChest::setItem(int itemId, int amount, int spot) {
	if (amount > 99)
		amount = 99;
	contents[spot] = std::pair<int, int>(itemId, amount);
}

void TreasureChest::tick(sf::Int32 dt) {
	updateState(this);
	timeAlive += dt;
}

void TreasureChest::render(Handler* handler) {
	Entity::render(handler);
	if (enabled) {

		sf::Texture* interactTexture = nullptr;
		if (handler->inputManager->usingController)
			interactTexture = handler->assets->getControllerInteractPrompt();
		else
			interactTexture = handler->assets->getInteractPrompt();

		sf::RectangleShape interact(sf::Vector2f(interactTexture->getSize().x * 3, interactTexture->getSize().y * 3));
		float yExtra = sin(timeAlive / 200.f) * 5;
		interact.setPosition(x - handler->camera->getXOffset(), y - handler->camera->getYOffset() + yExtra - interactTexture->getSize().y * 3.f / 2.f + 5);
		interact.setTexture(interactTexture);
		handler->window->draw(interact);
	}
}

void TreasureChest::onInteract() {
	if (handler->getCurrentState()->getType() != CHEST_INVENTORY) {
		handler->setGameState(CHEST_INVENTORY);
		ChestInventoryState* state = dynamic_cast<ChestInventoryState*>(handler->getCurrentState());
		state->setOpenChest(this);
	} else {
		ChestInventoryState* state = dynamic_cast<ChestInventoryState*>(handler->getCurrentState());
		state->setOpenChest(nullptr);
		handler->setGameState(PLAYING);
	}

}