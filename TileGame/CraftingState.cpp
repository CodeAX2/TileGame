#include "CraftingState.h"
#include "Assets.h"
#include "Player.h"
#include "Inventory.h"
#include "ItemDesc.h"
#include "ItemMeta.h"
#include "InputManager.h"
#include "PlayingState.h"

using namespace tg;


CraftingState::CraftingState(Handler* handler) : GameState(CRAFTING_INVENTORY) {
	this->handler = handler;

	craftingGui = handler->assets->loadTextureFromResource(MENU_CRAFTING_INV);
	upArrow = handler->assets->loadTextureFromResource(CRAFTING_ICON_UP);
	downArrow = handler->assets->loadTextureFromResource(CRAFTING_ICON_DOWN);
	deny = handler->assets->loadTextureFromResource(CRAFTING_ICON_DENY);
	allow = handler->assets->loadTextureFromResource(CRAFTING_ICON_ALLOW);

	invHighlight = handler->assets->loadTextureFromResource(INV_HIGHLIGHT);
	craftHighlight = handler->assets->loadTextureFromResource(CRAFT_HIGHLIGHT);


}


CraftingState::~CraftingState() {
}

void CraftingState::tick(sf::Int32 dt) {
	handler->getCustomState(PLAYING)->tick(dt);
	if (selectedCraftSlotX != -1 && selectedCraftSlotY != -1) {
		if (playerHasIngredients()) {
			canCraftSelected = true;
		} else {
			canCraftSelected = false;
		}
	}
}

void CraftingState::render() {
	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}

	handler->getCustomState(PLAYING)->render();
	renderBackground();
	renderHighlight();
	renderIcons();
	renderCraftables();
	renderItems();

}


void CraftingState::renderBackground() {
	sf::RectangleShape invRect(handler->window->getView().getSize());
	invRect.setTexture(craftingGui);

	handler->window->draw(invRect);
}

void CraftingState::renderHighlight() {

	if (selectedCraftSlotX != -1 && selectedCraftSlotY != -1) {

		sf::RectangleShape highlight(sf::Vector2f(invHighlight->getSize().x * 3 * 1.5, invHighlight->getSize().y * 3 * 1.5));
		highlight.setTexture(craftHighlight);
		float xPos, yPos;
		xPos = xOffset + selectedCraftSlotX * (size + xSpace);
		yPos = yOffsetTop + selectedCraftSlotY * (size + ySpace);

		highlight.setPosition(xPos, yPos);
		handler->window->draw(highlight);
	}



	if (xSlot != -1 && ySlot != -1) {

		sf::RectangleShape highlight(sf::Vector2f(invHighlight->getSize().x * 3 * 1.5, invHighlight->getSize().y * 3 * 1.5));
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
			sf::RectangleShape curItem(sf::Vector2f(96 * 1.5, 96 * 1.5));
			curItem.setTexture(handler->assets->getItemTexture(inv->getInventory()[i].first));
			curItem.setPosition(xOffset + (size + xSpace) * i + 6 * 1.5, yOffsetBottom + 6 * 1.5);
			handler->window->draw(curItem);

			sf::Text count;
			count.setString(std::to_string(inv->getInventory()[i].second));
			count.setFont(guiFont);
			count.setCharacterSize(16 * 1.5);
			count.setPosition(sf::Vector2f(i * 42 * 3 * 1.5f + 28 * 3 * 1.5f + 12 * 1.5f, yOffsetBottom + 108 * 1.5f - count.getGlobalBounds().height - 10 * 1.5f));
			handler->window->draw(count);
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 7; j++) {
			if (clickedSlotY == i + 2 && clickedSlotX == j + 1) continue;
			int index = 9;
			index += i * 7 + j;
			if (inv->getInventory()[index].first != -1) {
				sf::RectangleShape curItem(sf::Vector2f(96 * 1.5, 96 * 1.5));
				curItem.setTexture(handler->assets->getItemTexture(inv->getInventory()[index].first));
				curItem.setPosition(xOffset + (size + xSpace) * (j + 1) + 6 * 1.5, yOffsetMid + (size + ySpace) * i + 6 * 1.5);
				handler->window->draw(curItem);

				sf::Text count;
				count.setString(std::to_string(inv->getInventory()[index].second));
				count.setFont(guiFont);
				count.setCharacterSize(16);
				count.setPosition(sf::Vector2f((j + 1) * 42 * 3 * 1.5f + 28 * 3 * 1.5f + 12 * 1.5f, yOffsetMid + (size + ySpace) * i + size - count.getGlobalBounds().height - 10 * 1.5f));
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

		sf::RectangleShape curItem(sf::Vector2f(96 * 1.5, 96 * 1.5));
		curItem.setTexture(handler->assets->getItemTexture(getItemAt(clickedSlotX, clickedSlotY).first));
		curItem.setPosition(mx - 96 * 1.5 / 2, my - 96 * 1.5 / 2);
		handler->window->draw(curItem);

		sf::Text count;
		count.setString(std::to_string(getItemAt(clickedSlotX, clickedSlotY).second));
		count.setFont(guiFont);
		count.setCharacterSize(16 * 1.5);
		count.setPosition(sf::Vector2f(mx + 14 * 1.5 - size / 2, my + size - count.getGlobalBounds().height - 10 * 1.5 - size / 2));
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
		infoText.setCharacterSize(16 * 1.5);
		infoText.setLineSpacing(1.2f);
		infoText.setOrigin(infoText.getGlobalBounds().left, infoText.getGlobalBounds().top + infoText.getGlobalBounds().height);
		infoText.setPosition(mx + 10 * 1.5, my - 10 * 1.5);
		infoText.setFillColor(sf::Color::White);



		sf::RectangleShape infoBoxBg(sf::Vector2f(infoText.getGlobalBounds().width + 20 * 1.5, infoText.getGlobalBounds().height + 20 * 1.5));
		infoBoxBg.setFillColor(sf::Color(0, 8, 20, 225));
		infoBoxBg.setPosition(mx, my - infoBoxBg.getSize().y);


		// Allign text correctly
		if (infoText.getGlobalBounds().left + infoText.getGlobalBounds().width > v.x || infoBoxBg.getPosition().x + infoBoxBg.getSize().x > v.x) {
			infoText.setPosition(infoText.getPosition().x - infoBoxBg.getGlobalBounds().width, infoText.getPosition().y);
			infoBoxBg.setPosition(infoBoxBg.getPosition().x - infoBoxBg.getSize().x, infoBoxBg.getPosition().y);
		}

		if (infoText.getGlobalBounds().top < 0 || infoBoxBg.getPosition().y < 0) {
			infoText.setPosition(infoText.getPosition().x, infoText.getPosition().y + infoBoxBg.getSize().y);
			infoBoxBg.setPosition(infoBoxBg.getPosition().x, infoBoxBg.getPosition().y + infoBoxBg.getSize().y);
		}


		handler->window->draw(infoBoxBg);
		handler->window->draw(infoText);


	}


}

void CraftingState::renderIcons() {
	if (hoverUp) {
		sf::RectangleShape upArrowIcon(sf::Vector2f(upArrow->getSize()) * 1.5f);
		upArrowIcon.setTexture(upArrow);
		handler->window->draw(upArrowIcon);
	}

	if (hoverDown) {
		sf::RectangleShape downArrowIcon(sf::Vector2f(downArrow->getSize()) * 1.5f);
		downArrowIcon.setTexture(downArrow);
		handler->window->draw(downArrowIcon);
	}

	if (canCraftSelected) {
		sf::RectangleShape canCraftIcon(sf::Vector2f(allow->getSize()) * 1.5f);
		canCraftIcon.setTexture(allow);
		handler->window->draw(canCraftIcon);
	} else {
		sf::RectangleShape cannotCraftIcon(sf::Vector2f(deny->getSize()) * 1.5f);
		cannotCraftIcon.setTexture(deny);
		handler->window->draw(cannotCraftIcon);
	}

}

void CraftingState::renderCraftables() {
	std::vector<int> craftableItems = ItemMeta::getCraftableItems();
	int row = 0;
	int pos = 0;
	for (int i = 0; i < 10; i++, pos++) {
		if (i == 5) {
			row = 1;
			pos = 0;
		}

		if (i + craftSlotOffset >= craftableItems.size()) continue;

		int itemId = craftableItems[i + craftSlotOffset];
		sf::RectangleShape curItem(sf::Vector2f(96 * 1.5, 96 * 1.5));
		curItem.setTexture(handler->assets->getItemTexture(itemId));
		curItem.setPosition(xOffset + (size + xSpace) * (pos)+6 * 1.5, yOffsetTop + (size + ySpace) * row + 6 * 1.5);
		handler->window->draw(curItem);

		if (selectedCraftSlotX != pos || selectedCraftSlotY != row) {
			continue;
		}

		std::map<int, int> recipe = ItemMeta::getCraftingRecipe(itemId);
		if (recipe.size() != 0) {
			int ingredientRow = 0;
			int ingredientPos = 0;
			for (std::map<int, int>::iterator iter = recipe.begin(); iter != recipe.end(); ++iter, ingredientPos++) {

				if (ingredientPos == 5) {
					ingredientRow++;
					ingredientPos = 0;
				}
				int key = iter->first;
				int value = iter->second;

				sf::RectangleShape curIngredient(sf::Vector2f(96 * 1.5, 96 * 1.5));
				curIngredient.setTexture(handler->assets->getItemTexture(key));
				curIngredient.setPosition(xOffsetIngredients + (96 * 1.5 + xIngredientSpace) * ingredientPos, yOffsetIngredients + (96 * 1.5 + yIngredientSpace) * ingredientRow);
				handler->window->draw(curIngredient);

				sf::Text count;
				count.setString(std::to_string(value));
				count.setFont(guiFont);
				count.setCharacterSize(16 * 1.5);
				count.setPosition(sf::Vector2f(curIngredient.getPosition().x + 8 * 1.5, yOffsetIngredients + (96 * 1.5 + yIngredientSpace) * ingredientRow + 102 * 1.5 - count.getGlobalBounds().height - 10 * 1.5));
				handler->window->draw(count);

			}
		}

	}
}

void CraftingState::updateMouse() {
	sf::Vector2f v = handler->guiView.getSize();
	sf::Vector2u w = handler->window->getSize();
	int mx = sf::Mouse::getPosition(*(handler->window)).x * (v.x / w.x);
	int my = sf::Mouse::getPosition(*(handler->window)).y * (v.y / w.y);
	sf::Vector2i mousePos(mx, my);

	// Check for hovering on arrows
	if (mx >= 22 * 1.5 && mx <= 60 * 1.5 && my >= 46 * 1.5 && my <= 111 * 1.5) {
		hoverUp = true;
	} else {
		hoverUp = false;
	}


	if (mx >= 22 * 1.5 && mx <= 60 * 1.5 && my >= 169 * 1.5 && my <= 234 * 1.5) {
		hoverDown = true;
	} else {
		hoverDown = false;
	}

	if (mx >= 712 * 1.5 && mx <= 807 * 1.5 && my >= 90 * 1.5 && my <= 185 * 1.5) {
		hoverCraft = true;
	} else {
		hoverCraft = false;
	}


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
		} else {
			// Clicked to select a crafting item, or to interact with a button
			if (clickedSlotX == -1 && clickedSlotY == -1) {

				if (xSlot != -1 && ySlot != -1) {
					// Clicked a slot
					selectedCraftSlotX = xSlot;
					selectedCraftSlotY = ySlot;

					if (playerHasIngredients()) {
						canCraftSelected = true;
					} else {
						canCraftSelected = false;
					}

				} else {
					// Clicked a button (maybe)
					if (hoverCraft) {
						// Clicked the craft button
						craftSelectedItem();
					}


					if (hoverUp) {

						if (craftSlotOffset != 0) {
							craftSlotOffset -= 5;
							selectedCraftSlotX = -1;
							selectedCraftSlotY = -1;
						}
					}

					if (hoverDown) {

						if (craftSlotOffset + 10 < ItemMeta::getCraftableItems().size()) {
							craftSlotOffset += 5;
							selectedCraftSlotX = -1;
							selectedCraftSlotY = -1;
						}
					}

				}

			} else {
				canCraftSelected = false;
			}
		}
	}
}


void CraftingState::resume() {

	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	if (!ps->musicIsPaused())
		ps->playBGMusic();

	selectedCraftSlotX = -1;
	selectedCraftSlotY = -1;
	canCraftSelected = false;
}

void CraftingState::pause() {
	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	if (ps->musicIsPaused())
		ps->pauseBGMusic();
}

std::pair<int, int> CraftingState::getItemAt(int x, int y) {
	std::pair<int, int> item;
	int index;
	if (y < 2) {

		std::vector<int> craftableItems = ItemMeta::getCraftableItems();
		int index = x + y * 5 + craftSlotOffset;
		if (index >= craftableItems.size()) {
			return std::pair<int, int>(-1, -1);
		}
		return std::pair<int, int>(craftableItems[index], -1);
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
		int maxStackSize = ItemMeta::getMaxStackSize(itemA.first);
		if (totalAmount > maxStackSize) {
			itemB.second = maxStackSize;
			totalAmount -= maxStackSize;
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

bool CraftingState::playerHasIngredients() {
	std::vector<int> craftableItems = ItemMeta::getCraftableItems();
	int index = selectedCraftSlotX + selectedCraftSlotY * 5 + craftSlotOffset;
	if (index < craftableItems.size()) {
		int itemId = craftableItems[index];

		std::map<int, int> recipe = ItemMeta::getCraftingRecipe(itemId);
		for (std::map<int, int>::iterator iter = recipe.begin(); iter != recipe.end(); ++iter) {

			int key = iter->first;
			int value = iter->second;

			if (handler->player->getInventory()->getAmountOfItem(key) < value) {
				return false;
			}
		}
		return true;
	}
	return false;
}

void CraftingState::craftSelectedItem() {
	if (selectedCraftSlotX != -1 && selectedCraftSlotY != -1) {

		if (canCraftSelected) {

			std::vector<int> craftableItems = ItemMeta::getCraftableItems();
			int index = selectedCraftSlotX + selectedCraftSlotY * 5 + craftSlotOffset;
			if (index < craftableItems.size()) {
				int itemId = craftableItems[index];

				std::map<int, int> recipe = ItemMeta::getCraftingRecipe(itemId);
				for (std::map<int, int>::iterator iter = recipe.begin(); iter != recipe.end(); ++iter) {

					int key = iter->first;
					int value = iter->second;

					handler->player->getInventory()->removeItemFromInv(key, value);

				}

				handler->player->addItemToInv(itemId);

			}



		}

	}
}

void CraftingState::updateJoystick(sf::Int32 dt) {
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

			if (ySlot >= 2) {

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
			} else {
				// Clicked to select a crafting item, or to interact with a button
				if (clickedSlotX == -1 && clickedSlotY == -1) {

					if (xSlot != -1 && ySlot != -1) {
						// Clicked a slot
						selectedCraftSlotX = xSlot;
						selectedCraftSlotY = ySlot;

						if (playerHasIngredients()) {
							canCraftSelected = true;
						} else {
							canCraftSelected = false;
						}

					} else {
						// Clicked a button (maybe)
						if (hoverCraft) {
							// Clicked the craft button
							craftSelectedItem();
						}


						if (hoverUp) {
							if (craftSlotOffset != 0) {
								craftSlotOffset -= 5;
								selectedCraftSlotX = -1;
								selectedCraftSlotY = -1;
							}
						}

						if (hoverDown) {
							if (craftSlotOffset + 10 < ItemMeta::getCraftableItems().size()) {
								craftSlotOffset += 5;
								selectedCraftSlotX = -1;
								selectedCraftSlotY = -1;
							}
						}

					}

				} else {
					canCraftSelected = false;
				}
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