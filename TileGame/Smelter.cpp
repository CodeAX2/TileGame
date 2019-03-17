#include "Smelter.h"
#include "InputManager.h"
#include "Player.h"
#include "ItemMeta.h"
#include "Item.h"
#include "PlayingState.h"

using namespace tg;


Smelter::Smelter(int x, int y, Handler* handler, World* world) :
	Static(x, y, handler, 9, 32 * 3 - 25, 32 * 3 - 18, 25, 32 * 3, 32 * 3, true, SMELTER_E, 73, world),
	Interactable(handler) {

	this->texture = handler->assets->getSmelterTexture(0);
	this->maxHealth = 60;
	this->health = 60;

	world->getEntityManager()->addTickAnywhereEntity(this);

	lightSizeX = 640;
	lightSizeY = 640;
	lightIntensity = 200;

	lightX = this->x + 9 - lightSizeX / 2 + 32 * 3 - 18 / 2;
	lightY = this->y + 32 * 3 - 25 - lightSizeY / 2;
}


Smelter::~Smelter()
{
}

void Smelter::updateState(Entity* caller) {
	Interactable::updateState(this);

	if (enabled && !finished && !smelting) {

		std::pair<int, int> holdingItem = handler->player->getItemInfoInHotBar();

		if (!(ItemMeta::isBurnable(holdingItem.first) && !hasFuel || ItemMeta::isSmelatable(holdingItem.first) && !hasSmeltable)) {
			enabled = false;
		}

	}

	if (smelting && enabled) {
		enabled = false;
	}

}


void Smelter::tick(sf::Int32 dt) {

	updateState(this);
	timeAlive += dt;

	if (smelting) {
		smeltTimer -= (double)dt * burnSpeed;

		if (smeltTimer <= 0) {

			hasFuel = false;
			burnSpeed = 0;

			hasSmeltable = false;

			smeltTimer = 0;
			smelting = false;

			finished = true;

		}

	}

	if (!smelting) {
		if (hasFuel) {
			texture = handler->assets->getSmelterTexture(2);
		} else if (hasSmeltable) {
			texture = handler->assets->getSmelterTexture(1);
		} else {
			if (finished) {
				texture = handler->assets->getSmelterTexture(4);
			} else {
				texture = handler->assets->getSmelterTexture(0);
			}
		}
	} else {
		texture = handler->assets->getSmelterTexture(3);
	}
}


void Smelter::render(Handler* handler) {
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

void Smelter::onInteract() {

	if (smelting) return;

	if (finished) {

		for (int i = 0; i < ItemMeta::getSmeltResult(smeltingItem).second; i++) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21, handler, ItemMeta::getSmeltResult(smeltingItem).first, world);
		}

		smeltingItem = -1;
		finished = false;

		return;

	}


	std::pair<int, int> holdingItem = handler->player->getItemInfoInHotBar();

	bool inserted = false;


	if (ItemMeta::isBurnable(holdingItem.first)) {

		if (!hasFuel) {

			hasFuel = true;
			burnSpeed = ItemMeta::getBurnSpeed(holdingItem.first);
			handler->player->removeItemFromHotbar();
			inserted = true;

		}
	}

	if (ItemMeta::isSmelatable(holdingItem.first) && !inserted) {

		if (!hasSmeltable) {

			hasSmeltable = true;
			smeltingItem = holdingItem.first;
			handler->player->removeItemFromHotbar();
			inserted = true;

		}
	}


	if (hasSmeltable && hasFuel) {
		// Begin smelting
		smeltTimer = BASE_SMELT_TIME;
		smelting = true;
		finished = false;
	}

	if (!(ItemMeta::isBurnable(holdingItem.first) && !hasFuel || ItemMeta::isSmelatable(holdingItem.first) && !hasSmeltable)) {
		enabled = false;
	}

}

void Smelter::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 13, world);
	if (smeltingItem != -1) {
		if (!finished) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 + 10, handler, smeltingItem, world);
		} else {
			for (int i = 0; i < ItemMeta::getSmeltResult(smeltingItem).second; i++) {
				new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 + 10, handler, ItemMeta::getSmeltResult(smeltingItem).first, world);
			}
		}

	}
}

void Smelter::renderLighting(Handler* handler) {
	if (smelting) {
		PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
		if (ps->getDarknessPercent() != 0) {
			sf::Color color(255, 255, 0, lightIntensity);

			sf::RectangleShape light(sf::Vector2f(lightSizeX, lightSizeY));
			light.setPosition(
				(int)(lightX - floor(handler->camera->getXOffset())),
				(int)(lightY - floor(handler->camera->getYOffset())));

			sf::Texture* lightT = handler->assets->getLightGFX();

			light.setFillColor(color);
			light.setTexture(lightT);

			sf::BlendMode bm2(
				sf::BlendMode::Factor::Zero,
				sf::BlendMode::Factor::DstColor,
				sf::BlendMode::Equation::Add,
				sf::BlendMode::Factor::Zero,
				sf::BlendMode::Factor::OneMinusSrcAlpha,
				sf::BlendMode::Equation::Add
			);

			ps->getLightRenderer()->draw(light, bm2);
		}
	}
}