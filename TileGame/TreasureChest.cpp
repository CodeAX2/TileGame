#include "TreasureChest.h"
#include "Item.h"


TreasureChest::TreasureChest(int x, int y, Handler* handler, World* world) :
	Static(x, y, handler, 9, 32 * 3 - 25, 32 * 3 - 18, 25, 32 * 3, 32 * 3, false, TREASURE_CHEST_E, 73, world) {

	this->texture = handler->assets->getTreasureChestTexture();
	this->maxHealth = 140;
	this->health = 140;

	if (contains.size() == 0) {
		for (int i = 0; i < 256; i++) {
			contains.push_back(0);
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
	if (defaultContents) {
		for (int i = 0; i < 5; i++) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
		}

		if (rand() % 5 == 0) {
			for (int i = 0; i < 3; i++) {
				new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
			}
		}
	} else {
		for (int i = 0; i < contains.size(); i++) {
			for (int j = 0; j < contains[i]; j++) {
				new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, i, world);
			}
		}
	}


}

// Set the amount of a specific item in the chest
void TreasureChest::setItemAmount(int itemId, int amount) {
	if (contains.size() == 0) {
		for (int i = 0; i < 256; i++) {
			contains.push_back(0);
		}
	}

	defaultContents = false;
	contains[itemId] = amount;
}

