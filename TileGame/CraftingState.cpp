#include "CraftingState.h"
#include "Assets.h"
#include "Player.h"
#include "Inventory.h"
#include "ItemDesc.h"

using namespace tg;


CraftingState::CraftingState(Handler* handler) : GameState(CRAFTING_INVENTORY) {
	this->handler = handler;

	craftingGui = handler->assets->loadTextureFromResource(MENU_CRAFTING_INV);
	upArrow = handler->assets->loadTextureFromResource(CRAFTING_ICON_UP);
	downArrow = handler->assets->loadTextureFromResource(CRAFTING_ICON_DOWN);
	deny = handler->assets->loadTextureFromResource(CRAFTING_ICON_DENY);
	allow = handler->assets->loadTextureFromResource(CRAFTING_ICON_ALLOW);

	invHighlight = handler->assets->loadTextureFromResource(INV_HIGHLIGHT);


}


CraftingState::~CraftingState()
{
}

void CraftingState::tick(sf::Int32 dt) {

}

void CraftingState::render() {
	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}

	handler->getCustomState(PLAYING)->render();
	renderBackground();
	renderHighlight();
	//	renderIcons();
	renderItems();


}


void CraftingState::renderBackground() {
	sf::RectangleShape invRect(handler->window->getView().getSize());
	invRect.setTexture(craftingGui);

	handler->window->draw(invRect);
}

void CraftingState::renderHighlight() {
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


void CraftingState::renderItems() {
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

void CraftingState::renderItemInfo() {
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



void CraftingState::updateMouse() {
	sf::Vector2f v = handler->guiView.getSize();
	sf::Vector2u w = handler->window->getSize();
	int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
	int my = sf::Mouse::getPosition(*(handler->window)).y * (v.y / w.y);
	sf::Vector2i mousePos(mx, my);

	int section = 0; // Indicates what section the mouse is in, 0 for crafting, 1 for inv, 2 for hotbar

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


void CraftingState::mouseClicked(sf::Event e) {
	if (e.type == sf::Event::MouseButtonPressed) {
		if (ySlot >= 2) {
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
}


void CraftingState::resume() {

}

std::pair<int, int> CraftingState::getItemAt(int x, int y) {
	std::pair<int, int> item;
	int index;
	if (y < 2) {
		return std::pair<int, int>(-1, -1);
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


void CraftingState::swapItems(int itemAx, int itemAy, int itemBx, int itemBy) {
	// If items are the same type we need to combine their amounts
	//		Put the combined into the B slot, and any extra into the A slot
	// If not, just swap their positions
	std::pair<int, int> itemA, itemB;
	int itemAIndex = getItemIndex(itemAx, itemAy);
	int itemBIndex = getItemIndex(itemBx, itemBy);
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

		handler->player->getInventory()->setItemAtIndex(itemA.first, itemA.second, itemAIndex);
		handler->player->getInventory()->setItemAtIndex(itemB.first, itemB.second, itemBIndex);


	} else {
		// Swap the items

		handler->player->getInventory()->setItemAtIndex(itemB.first, itemB.second, itemAIndex);
		handler->player->getInventory()->setItemAtIndex(itemA.first, itemA.second, itemBIndex);

	}
}

int CraftingState::getItemIndex(int x, int y) {
	int index;

	if (y == 4) {
		index = x;
	} else {
		index = 9 + 7 * (y - 2) + x - 1;
	}


	return index;

}