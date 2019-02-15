#include "InventoryState.h"
#include "PlayingState.h"
#include "InputManager.h"
#include "Assets.h"
#include "Player.h"
#include "ItemDesc.h"

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

}
void InventoryState::tick(sf::Int32 dt) {
	handler->getCustomState(PLAYING)->tick(dt);

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
			if (clickedSlotY == 3 && clickedSlotX == i) {
				continue;
			}
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

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 7; j++) {
			if (clickedSlotY == i + 1 && clickedSlotX == j + 1) continue;
			int index = 9;
			index += i * 7 + j;
			if (inv->getInventory()[index].first != -1) {
				sf::RectangleShape curItem(sf::Vector2f(96, 96));
				curItem.setTexture(handler->assets->getItemTexture(inv->getInventory()[index].first));
				curItem.setPosition(xOffset + (size + xSpace) * (j + 1) + 6, yOffset + (size + ySpace) * (i + 1) + 6);
				handler->window->draw(curItem);

				sf::Text count;
				count.setString(std::to_string(inv->getInventory()[index].second));
				count.setFont(*(handler->assets->getArialiFont()));
				count.setCharacterSize(16);
				count.setPosition(sf::Vector2f((j + 1) * 42 * 3 + 28 * 3 + 12, yOffset + (size + ySpace) * (i + 1) + size - count.getGlobalBounds().height - 10));
				handler->window->draw(count);


			}
		}
	}


	renderHighlight();

	// Draw the item on the cursor
	if (clickedSlotX != -1 && clickedSlotY != -1) {

		sf::Vector2f v = handler->guiView.getSize();
		sf::Vector2u w = handler->window->getSize();
		int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
		int my = sf::Mouse::getPosition(*(handler->window)).y * (v.y / w.y);

		int index = posToId(clickedSlotX, clickedSlotY);
		sf::RectangleShape curItem(sf::Vector2f(96, 96));
		curItem.setTexture(handler->assets->getItemTexture(inv->getInventory()[index].first));
		curItem.setPosition(mx - 96 / 2, my - 96 / 2);
		handler->window->draw(curItem);

		sf::Text count;
		count.setString(std::to_string(inv->getInventory()[index].second));
		count.setFont(*(handler->assets->getArialiFont()));
		count.setCharacterSize(16);
		count.setPosition(sf::Vector2f(mx + 14 - size / 2, my + size - count.getGlobalBounds().height - 10 - size / 2));
		handler->window->draw(count);


	} else {
		// We only want to render item info
		// If we don't have an item on the cursor
		renderItemInfo();
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

void InventoryState::renderItemInfo() {
	if (xSlot == -1 || ySlot == -1) return;
	int itemId = handler->player->getInventory()->getInventory()[posToId(xSlot, ySlot)].first;
	int itemAmnt = handler->player->getInventory()->getInventory()[posToId(xSlot, ySlot)].second;

	if (itemId != -1 && itemAmnt != 0) {

		sf::Vector2f v = handler->guiView.getSize();
		sf::Vector2u w = handler->window->getSize();
		int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
		int my = (sf::Mouse::getPosition(*(handler->window)).y) * (v.y / w.y);


		sf::Text infoText;
		infoText.setFont(*(handler->assets->getArialiFont()));
		infoText.setString(ItemDesc::getInfo(itemId));
		infoText.setCharacterSize(16);
		infoText.setLineSpacing(1.2f);
		infoText.setOrigin(infoText.getGlobalBounds().left, infoText.getGlobalBounds().top + infoText.getGlobalBounds().height);
		infoText.setPosition(mx + 10, my - 10);
		infoText.setFillColor(sf::Color::White);


		sf::RectangleShape infoBoxBg(sf::Vector2f(infoText.getGlobalBounds().width + 20, infoText.getGlobalBounds().height + 20));
		infoBoxBg.setFillColor(sf::Color(0, 8, 20, 225));
		infoBoxBg.setPosition(mx, my - infoBoxBg.getSize().y);
		handler->window->draw(infoBoxBg);

		handler->window->draw(infoText);


	}


}

void InventoryState::mouseClicked(sf::Event e) {

	if (e.type != sf::Event::MouseButtonPressed && e.type != sf::Event::MouseButtonReleased) {
		return;
	}

	bool changedFromPrev = false;

	if (e.type == sf::Event::MouseButtonPressed) {
		if (!handler->inputManager->mouseIsPressed) {
			changedFromPrev = true;
		}
		handler->inputManager->mouseIsPressed = true;
	} else {
		if (handler->inputManager->mouseIsPressed) {
			changedFromPrev = true;
		}
		handler->inputManager->mouseIsPressed = false;
	}

	if (changedFromPrev && handler->inputManager->mouseIsPressed) {
		if (clickedSlotX == -1 || clickedSlotY == -1) {
			if (handler->player->getInventory()->getInventory()[posToId(xSlot, ySlot)].first != -1) {
				// Clicked to pickup
				clickedSlotX = xSlot;
				clickedSlotY = ySlot;
			}
		} else {
			// Clicked to place
			if ((clickedSlotX != xSlot || clickedSlotY != ySlot) && (xSlot != -1 && ySlot != -1)) {
				if (handler->player->getInventory()->getInventory()[posToId(xSlot, ySlot)].first != handler->player->getInventory()->getInventory()[posToId(clickedSlotX, clickedSlotY)].first)
					swapItems(clickedSlotX, clickedSlotY, xSlot, ySlot);
				else
					combineItems(clickedSlotX, clickedSlotY, xSlot, ySlot);
			}
			clickedSlotX = -1;
			clickedSlotY = -1;
		}
	}


}

void InventoryState::updateMouse() {
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


void InventoryState::swapItems(int fromX, int fromY, int toX, int toY) {
	int fromInvId, toInvId;

	fromInvId = posToId(fromX, fromY);
	toInvId = posToId(toX, toY);

	handler->player->getInventory()->swapItems(fromInvId, toInvId);


}

// This assumes that the items at (fromX, fromY) are the same type as those at (toX, toY)
void InventoryState::combineItems(int fromX, int fromY, int toX, int toY) {
	int fromInvId, toInvId;

	fromInvId = posToId(fromX, fromY);
	toInvId = posToId(toX, toY);

	handler->player->getInventory()->combineItems(fromInvId, toInvId);

}

int InventoryState::posToId(int posX, int posY) {
	int id;

	if (posY == 3) {
		id = posX;
	} else {
		id = 9;
		id += (posY - 1) * 7 + (posX - 1);
	}

	return id;

}

void InventoryState::pause() {
	clickedSlotX = -1;
	clickedSlotY = -1;
	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	ps->pauseBGMusic();
}

void InventoryState::updateJoystick(sf::Int32 dt) {

	InputManager* im = handler->inputManager;

	float jX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	float jY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	float jX2 = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
	float jY2 = sf::Joystick::getAxisPosition(0, sf::Joystick::V);




	// Move the mouse
	sf::Vector2i newPos = sf::Mouse::getPosition();
	bool updateMouse = false;

	if (jX <= -30) {
		im->usingController = true;
		newPos.x -= ceil(dt / 1.75f);
		updateMouse = true;
	} else if (jX >= 30) {
		im->usingController = true;
		newPos.x += ceil(dt / 1.75f);
		updateMouse = true;
	}


	if (jY <= -30) {
		im->usingController = true;
		newPos.y -= ceil(dt / 1.75f);
		updateMouse = true;
	} else if (jY >= 30) {
		im->usingController = true;
		newPos.y += ceil(dt / 1.75f);
		updateMouse = true;
	}

	if (updateMouse)
		sf::Mouse::setPosition(newPos);

	// Pickup an item
	if (sf::Joystick::isButtonPressed(0, 1)) {
		bool prevValue = im->joyStickButtons[1];
		im->joyStickButtons[1] = true;

		if (!prevValue) {

			if (clickedSlotX == -1 || clickedSlotY == -1) {
				if (handler->player->getInventory()->getInventory()[posToId(xSlot, ySlot)].first != -1) {
					// Clicked to pickup
					clickedSlotX = xSlot;
					clickedSlotY = ySlot;
				}
			} else {
				// Clicked to place
				if ((clickedSlotX != xSlot || clickedSlotY != ySlot) && (xSlot != -1 && ySlot != -1)) {
					if (handler->player->getInventory()->getInventory()[posToId(xSlot, ySlot)].first != handler->player->getInventory()->getInventory()[posToId(clickedSlotX, clickedSlotY)].first)
						swapItems(clickedSlotX, clickedSlotY, xSlot, ySlot);
					else
						combineItems(clickedSlotX, clickedSlotY, xSlot, ySlot);
				}
				clickedSlotX = -1;
				clickedSlotY = -1;
			}




		}
			

	} else {
		im->joyStickButtons[1] = false;
	}


	// Close inventory
	if (sf::Joystick::isButtonPressed(0, 6)) {
		bool prevValue = im->joyStickButtons[6];
		im->joyStickButtons[6] = true;
		im->usingController = true;
		if (!prevValue) {
			handler->setGameState(PLAYING);
		}

	} else {
		im->joyStickButtons[6] = false;
	}

	// Pause Game
	if (sf::Joystick::isButtonPressed(0, 7)) {
		bool prevValue = im->joyStickButtons[7];
		im->joyStickButtons[7] = true;
		im->usingController = true;
		if (!prevValue) {
			handler->setGameState(MAIN_MENU);
		}

	} else {
		im->joyStickButtons[7] = false;
	}


}