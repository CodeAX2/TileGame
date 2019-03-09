#include "Workbench.h"
#include "InputManager.h"
#include "GameState.h"
#include "CraftingState.h"
#include "Item.h"

using namespace tg;

Workbench::Workbench(int x, int y, Handler* handler, World* world) : Static(x, y, handler, 9, 32 * 3 - 25, 32 * 3 - 18, 25, 32 * 3, 32 * 3, true, WORKBENCH_E, 73, world),
Interactable(handler) {

	this->texture = handler->assets->getWorkbenchTexture();
	this->maxHealth = 60;
	this->health = 60;


}


Workbench::~Workbench()
{
}

void Workbench::tick(sf::Int32 dt) {
	updateState(this);
	timeAlive += dt;
}


void Workbench::render(Handler* handler) {
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

void Workbench::onInteract() {

	if (handler->getCurrentState()->getType() != CRAFTING_INVENTORY) {
		handler->setGameState(CRAFTING_INVENTORY);
	} else {
		handler->setGameState(PLAYING);
	}

}

void Workbench::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 11, world);
}