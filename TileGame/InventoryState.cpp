#include "InventoryState.h"
#include "PlayingState.h"
#include "InputManager.h"
#include "Assets.h"
#include "Player.h"

using namespace tg;

InventoryState::InventoryState(Handler* handler) : GameState(INVENTORY) {
	this->handler = handler;

	invBg = new sf::Texture();
	invBg->loadFromImage(handler->assets->loadImageFromResource(MENU_INV));

	invHighlight = new sf::Texture();
	invHighlight->loadFromImage(handler->assets->loadImageFromResource(INV_HIGHLIGHT));
}


InventoryState::~InventoryState() {
	delete invBg;
	delete invHighlight;
}


void InventoryState::render() {

	handler->getCustomState(PLAYING)->render();
	renderInventory();
	renderHighlight();

}
void InventoryState::tick(sf::Int32 dt) {
	handler->getCustomState(PLAYING)->tick(dt);

	sf::Vector2f v = handler->guiView.getSize();
	sf::Vector2u w = handler->window->getSize();
	int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
	int my = sf::Mouse::getPosition(*(handler->window)).y * (v.y / w.y);
	sf::Vector2i mousePos(mx, my);

	mousePos -= sf::Vector2i(xOffset, yOffset);


	if (mousePos.y < 0 || mousePos.x < 0) {
		mousePos.y = -1;
		mousePos.x = -1;
	} else {

		mousePos.y /= (size + ySpace);
		mousePos.x /= (size + xSpace);

	}

	xSlot = mousePos.x;
	ySlot = mousePos.y;

	if (mx - xOffset - size * (xSlot + 1) - xSpace * (xSlot) >= 0) {
		xSlot = -1;
		ySlot = -1;
	} else if (my - yOffset - size * (ySlot + 1) - ySpace * ySlot >= 0) {
		xSlot = -1;
		ySlot = -1;
	}

	if (xSlot >= 9) {
		xSlot = -1;
		ySlot = -1;
	}

	if (ySlot >= 3) {
		if (my >= bottomRowOffset &&
			my < bottomRowOffset + size) {
			ySlot = 3;
		} else {
			xSlot = -1;
			ySlot = -1;
		}
	}

	bool valid = false;
	if (xSlot == -1 || ySlot == -1) {
		valid = false;
	} else {
		if (grid[ySlot][xSlot]) {
			valid = true;
		}
	}

	if (!valid) {
		xSlot = -1, ySlot = -1;
	}

}

void InventoryState::resume() {
	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	ps->playBGMusic();
}

void InventoryState::renderInventory() {
	sf::RectangleShape invRect(handler->window->getView().getSize());
	invRect.setTexture(invBg);

	handler->window->draw(invRect);



	Inventory* inv = handler->player->getInventory();

	for (int i = 0; i < 9; i++) {
		if (inv->getInventory()[i].first != -1) {
			sf::RectangleShape curItem(sf::Vector2f(96, 96));
			curItem.setTexture(handler->assets->getItemTexture(inv->getInventory()[i].first));
			curItem.setPosition(xOffset + (size + xSpace) * i + 6, bottomRowOffset + 6);
			handler->window->draw(curItem);

			sf::Text count;
			count.setString(std::to_string(inv->getInventory()[i].second));
			count.setFont(*(handler->assets->getArialiFont()));
			count.setCharacterSize(16);
			count.setPosition(sf::Vector2f(i * 42 * 3 + 28 * 3 + 12, bottomRowOffset + 108 - count.getGlobalBounds().height - 10));
			handler->window->draw(count);


		}
	}



}

void InventoryState::renderHighlight() {
	if (xSlot == -1 || ySlot == -1) return;

	sf::RectangleShape highlight(sf::Vector2f(size, size));
	highlight.setTexture(invHighlight);

	highlight.setPosition(xOffset + size * (xSlot)+xSpace * xSlot, yOffset + size * (ySlot)+ySpace * ySlot);
	if (ySlot == 3) {
		highlight.setPosition(highlight.getPosition().x, bottomRowOffset);
	}
	handler->window->draw(highlight);

}