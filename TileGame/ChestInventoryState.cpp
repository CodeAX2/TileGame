#include "ChestInventoryState.h"
#include "Assets.h"
#include "PlayingState.h"
#include "InputManager.h"
#include "Inventory.h"
#include "Player.h"
#include "ItemDesc.h"

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
	renderBackground();
	renderHighlight();
	renderItems();
}

void ChestInventoryState::tick(sf::Int32 dt) {
	handler->getCustomState(PLAYING)->tick(dt);
}

void ChestInventoryState::renderBackground() {
	sf::RectangleShape invRect(handler->window->getView().getSize());
	invRect.setTexture(invBg);

	handler->window->draw(invRect);

}

void ChestInventoryState::renderHighlight() {
	if (xSlot != -1 && ySlot != -1) {

		sf::RectangleShape highlight(sf::Vector2f(invHighlight->getSize().x * 3, invHighlight->getSize().y * 3));
		highlight.setTexture(invHighlight);
		float xPos, yPos;
		xPos = xOffset + xSlot * (size + xSpace);

		if (ySlot < 2) {
			yPos = yOffsetTop + ySlot * (size + ySpace);
		} else if (ySlot < 4) {
			yPos = yOffsetMid + (ySlot - 2) * (size + ySpace);
		} else {
			yPos = yOffsetBottom + (ySlot - 4) * (size + ySpace);
		}

		highlight.setPosition(xPos, yPos);
		handler->window->draw(highlight);
	}
}

void ChestInventoryState::renderItems() {

	Inventory* inv = handler->player->getInventory();

	for (int i = 0; i < 9; i++) {
		if (clickedSlotY == 4 && clickedSlotX == i) continue;
		if (inv->getInventory()[i].first != -1) {
			sf::RectangleShape curItem(sf::Vector2f(96, 96));
			curItem.setTexture(handler->assets->getItemTexture(inv->getInventory()[i].first));
			curItem.setPosition(xOffset + (size + xSpace) * i + 6, yOffsetBottom + 6);
			handler->window->draw(curItem);

			sf::Text count;
			count.setString(std::to_string(inv->getInventory()[i].second));
			count.setFont(guiFont);
			count.setCharacterSize(16);
			count.setPosition(sf::Vector2f(i * 42 * 3 + 28 * 3 + 12, yOffsetBottom + 108 - count.getGlobalBounds().height - 10));
			handler->window->draw(count);
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 7; j++) {
			if (clickedSlotY == i + 2 && clickedSlotX == j + 1) continue;
			int index = 9;
			index += i * 7 + j;
			if (inv->getInventory()[index].first != -1) {
				sf::RectangleShape curItem(sf::Vector2f(96, 96));
				curItem.setTexture(handler->assets->getItemTexture(inv->getInventory()[index].first));
				curItem.setPosition(xOffset + (size + xSpace) * (j + 1) + 6, yOffsetMid + (size + ySpace) * i + 6);
				handler->window->draw(curItem);

				sf::Text count;
				count.setString(std::to_string(inv->getInventory()[index].second));
				count.setFont(guiFont);
				count.setCharacterSize(16);
				count.setPosition(sf::Vector2f((j + 1) * 42 * 3 + 28 * 3 + 12, yOffsetMid + (size + ySpace) * i + size - count.getGlobalBounds().height - 10));
				handler->window->draw(count);


			}
		}
	}

	std::vector<std::pair<int, int>> chestContents = openChest->getContents();
	for (int i = 0; i < chestContents.size(); i++) {
		if ((clickedSlotY == 0 || clickedSlotY == 1) && clickedSlotX == i % 5 + 1) continue;
		if (chestContents[i].first == -1)
			continue;
		sf::RectangleShape curItem(sf::Vector2f(96, 96));
		curItem.setTexture(handler->assets->getItemTexture(chestContents[i].first));

		sf::Text count;
		count.setString(std::to_string(chestContents[i].second));
		count.setFont(guiFont);
		count.setCharacterSize(16);

		sf::Vector2f curItemPos, curCountPos;

		if (i < 5) {
			curItemPos = sf::Vector2f(xOffset + (size + xSpace) * (i + 1) + 6, yOffsetTop + (size + ySpace) * 0 + 6);
			curCountPos = sf::Vector2f((i + 1) * 42 * 3 + 28 * 3 + 12, yOffsetTop + (size + ySpace) * 0 + size - count.getGlobalBounds().height - 10);
		} else {
			curItemPos = sf::Vector2f(xOffset + (size + xSpace) * (i + 1 - 5) + 6, yOffsetTop + (size + ySpace) * 1 + 6);
			curCountPos = sf::Vector2f((i + 1 - 5) * 42 * 3 + 28 * 3 + 12, yOffsetTop + (size + ySpace) * 1 + size - count.getGlobalBounds().height - 10);
		}

		curItem.setPosition(curItemPos);
		count.setPosition(curCountPos);

		handler->window->draw(curItem);
		handler->window->draw(count);

	}


	// Draw the item on the cursor
	if (clickedSlotX != -1 && clickedSlotY != -1) {

		sf::Vector2f v = handler->guiView.getSize();
		sf::Vector2u w = handler->window->getSize();
		int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
		int my = sf::Mouse::getPosition(*(handler->window)).y * (v.y / w.y);

		sf::RectangleShape curItem(sf::Vector2f(96, 96));
		curItem.setTexture(handler->assets->getItemTexture(getItemAt(clickedSlotX, clickedSlotY).first));
		curItem.setPosition(mx - 96 / 2, my - 96 / 2);
		handler->window->draw(curItem);

		sf::Text count;
		count.setString(std::to_string(getItemAt(clickedSlotX, clickedSlotY).second));
		count.setFont(guiFont);
		count.setCharacterSize(16);
		count.setPosition(sf::Vector2f(mx + 14 - size / 2, my + size - count.getGlobalBounds().height - 10 - size / 2));
		handler->window->draw(count);


	} else {
		// We only want to render item info
		// If we don't have an item on the cursor
		renderItemInfo();
	}


}

void ChestInventoryState::renderItemInfo() {
	if (xSlot == -1 || ySlot == -1) return;
	int itemId = getItemAt(xSlot, ySlot).first;
	int itemAmnt = getItemAt(xSlot, ySlot).second;

	if (itemId != -1 && itemAmnt != 0) {

		sf::Vector2f v = handler->guiView.getSize();
		sf::Vector2u w = handler->window->getSize();
		int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
		int my = (sf::Mouse::getPosition(*(handler->window)).y) * (v.y / w.y);


		sf::Text infoText;
		infoText.setFont(guiFont);
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


void ChestInventoryState::updateMouse() {
	sf::Vector2f v = handler->guiView.getSize();
	sf::Vector2u w = handler->window->getSize();
	int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
	int my = sf::Mouse::getPosition(*(handler->window)).y * (v.y / w.y);
	sf::Vector2i mousePos(mx, my);

	int section = 0; // Indicates what section the mouse is in, 0 for chest, 1 for inv, 2 for hotbar

	if (mousePos.y >= 0 && mousePos.y < yOffsetMid) {
		mousePos.y -= yOffsetTop;
	} else if (mousePos.y >= yOffsetMid && mousePos.y < yOffsetBottom) {
		section = 1;
		mousePos.y -= yOffsetMid;
	} else {
		section = 2;
		mousePos.y -= yOffsetBottom;
	}

	mousePos.x -= xOffset;


	if (mousePos.y < 0 || mousePos.x < 0) {
		mousePos.y = -1;
		mousePos.x = -1;
	} else {

		mousePos.y /= (size + ySpace);
		mousePos.x /= (size + xSpace);

	}

	xSlot = mousePos.x;
	ySlot = mousePos.y;

	if (section == 0) {
		if (ySlot >= 2) {
			xSlot = -1;
			ySlot = -1;
		}
	} else if (section == 1) {
		if (ySlot >= 2) {
			ySlot = -1;
			xSlot = -1;
		}
	} else if (section == 2) {
		if (ySlot >= 1) {
			ySlot = -1;
			xSlot = -1;
		}
	}

	int usingYOffset;
	if (section == 0)
		usingYOffset = yOffsetTop;
	else if (section == 1)
		usingYOffset = yOffsetMid;
	else
		usingYOffset = yOffsetBottom;

	if (mx - xOffset - size * (xSlot + 1) - xSpace * (xSlot) >= 0) {
		xSlot = -1;
		ySlot = -1;
	} else if (my - usingYOffset - size * (ySlot + 1) - ySpace * ySlot >= 0) {
		xSlot = -1;
		ySlot = -1;
	}

	if (xSlot != -1 && ySlot != -1) {

		if (section == 1) {
			ySlot += 2;
		} else if (section == 2) {
			ySlot += 4;
		}

	}
	if (xSlot >= 9) {
		xSlot = -1;
		ySlot = -1;
	}

	if (!grid[ySlot][xSlot]) {
		xSlot = -1;
		ySlot = -1;
	}

}

void ChestInventoryState::mouseClicked(sf::Event e) {
	if (e.type == sf::Event::MouseButtonPressed) {
		if (clickedSlotX == -1 || clickedSlotY == -1) {
			if (getItemAt(xSlot, ySlot).first != -1) {
				clickedSlotX = xSlot;
				clickedSlotY = ySlot;
			}
		} else {
			// Swap items
			if (xSlot != -1 && ySlot != -1) {
				swapItems(clickedSlotX, clickedSlotY, xSlot, ySlot);
				clickedSlotX = -1;
				clickedSlotY = -1;
			}
		}
	}
}

void ChestInventoryState::swapItems(int itemAx, int itemAy, int itemBx, int itemBy) {
	// If items are the same type we need to combine their amounts
	//		Put the combined into the B slot, and any extra into the A slot
	// If not, just swap their positions
	std::pair<int, int> itemA, itemB;

	std::pair<int, bool> itemAInfo, itemBInfo;

	itemAInfo = getItemIndex(itemAx, itemAy);
	itemBInfo = getItemIndex(itemBx, itemBy);


	itemA = getItemAt(itemAx, itemAy);
	itemB = getItemAt(itemBx, itemBy);

	if (itemA.first == itemB.first && !(itemAx == itemBx && itemAy == itemBy)) {
		// Combine items
		int totalAmount = itemA.second + itemB.second;
		if (totalAmount > 99) {
			itemB.second = 99;
			totalAmount -= 99;
			itemA.second = totalAmount;
		} else {
			itemB.second = totalAmount;
			itemA.first = -1;
			itemA.second = -1;
		}

		if (itemAInfo.second) {
			openChest->setItem(itemA.first, itemA.second, itemAInfo.first);
		} else {
			handler->player->getInventory()->setItemAtIndex(itemA.first, itemA.second, itemAInfo.first);
		}

		if (itemBInfo.second) {
			openChest->setItem(itemB.first, itemB.second, itemBInfo.first);
		} else {
			handler->player->getInventory()->setItemAtIndex(itemB.first, itemB.second, itemBInfo.first);
		}


	} else {
		// Swap the items
		if (itemAInfo.second) {
			openChest->setItem(itemB.first, itemB.second, itemAInfo.first);
		} else {
			handler->player->getInventory()->setItemAtIndex(itemB.first, itemB.second, itemAInfo.first);
		}

		if (itemBInfo.second) {
			openChest->setItem(itemA.first, itemA.second, itemBInfo.first);
		} else {
			handler->player->getInventory()->setItemAtIndex(itemA.first, itemA.second, itemBInfo.first);
		}

	}




}

std::pair<int, int> ChestInventoryState::getItemAt(int x, int y) {
	std::pair<int, int> item;
	int index;
	if (y < 2) {
		index = y * 5 + x - 1;
		item = openChest->getContents()[index];
	} else {
		if (y == 4) {
			index = x;
		} else {
			index = 9 + 7 * (y - 2) + x - 1;
		}
		item = handler->player->getInventory()->getInventory()[index];
	}

	return item;

}

// int is index, bool is if its in the chest or not
std::pair<int, bool> ChestInventoryState::getItemIndex(int x, int y) {
	int index;
	bool inChest = false;
	if (y < 2) {
		index = y * 5 + x - 1;
		inChest = true;
	} else {
		if (y == 4) {
			index = x;
		} else {
			index = 9 + 7 * (y - 2) + x - 1;
		}
	}

	return std::pair<int, bool>(index, inChest);

}

void ChestInventoryState::updateJoystick(sf::Int32 dt) {
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
				if (getItemAt(xSlot, ySlot).first != -1) {
					// Clicked to pickup
					clickedSlotX = xSlot;
					clickedSlotY = ySlot;
				}
			} else {
				// Clicked to place
				if ((clickedSlotX != xSlot || clickedSlotY != ySlot) && (xSlot != -1 && ySlot != -1)) {
					swapItems(clickedSlotX, clickedSlotY, xSlot, ySlot);

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

	if (sf::Joystick::isButtonPressed(0, 3)) {
		bool prevValue = im->joyStickButtons[3];
		im->joyStickButtons[3] = true;
		im->usingController = true;
		if (!prevValue) {
			handler->setGameState(PLAYING);
		}

	} else {
		im->joyStickButtons[3] = false;
	}

	if (sf::Joystick::isButtonPressed(0, 0)) {
		bool prevValue = im->joyStickButtons[0];
		im->joyStickButtons[0] = true;
		im->usingController = true;
		if (!prevValue) {
			handler->setGameState(PLAYING);
		}

	} else {
		im->joyStickButtons[0] = false;
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