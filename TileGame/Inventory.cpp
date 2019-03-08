#include "Inventory.h"
#include <iostream>
#include "ItemMeta.h"

using namespace tg;

Inventory::Inventory() {
	for (int i = 0; i < INV_SIZE; i++) {
		inventory.push_back(std::pair<int, int>(-1, 0));
	}
}

Inventory::Inventory(std::vector<std::pair<int, int>> inventory) {
	this->inventory = inventory;
}


Inventory::~Inventory() {
}

void Inventory::addItemToInv(int item, int amount) {
	int extra = amount;
	int maxStackSize = ItemMeta::getMaxStackSize(item);
	for (int i = 0; i < INV_SIZE; i++) {
		std::pair<int, int> cur = inventory[i];
		if (cur.first == item) {
			int toAdd = maxStackSize - cur.second;
			if (extra - toAdd >= 0) {
				cur.second += toAdd;
				extra -= toAdd;
				inventory[i] = cur;

				if (extra == 0)
					break;

			} else {
				cur.second += extra;
				extra = 0;
				inventory[i] = cur;
				break;
			}
		}
	}

	if (extra != 0) {
		for (int i = 0; i < INV_SIZE; i++) {
			std::pair<int, int> cur = inventory[i];
			if (cur.first == -1) {
				if (extra - maxStackSize >= 0) {

					cur.first = item;
					cur.second = maxStackSize;
					extra -= maxStackSize;
					inventory[i] = cur;

					if (extra == 0)
						break;

				} else {
					cur.first = item;
					cur.second = extra;
					extra = 0;
					inventory[i] = cur;
					break;
				}

			}
		}
	}

}

int Inventory::getAmountOfItem(int item) {
	int total = 0;
	for (int i = 0; i < INV_SIZE; i++) {
		std::pair<int, int> cur = inventory[i];
		if (cur.first == item) {
			total += cur.second;
		}
	}

	return total;
}

void Inventory::swapItems(int spot1, int spot2) {
	std::pair<int, int> a = inventory[spot1];
	std::pair<int, int> b = inventory[spot2];
	inventory[spot1] = b;
	inventory[spot2] = a;
}

void Inventory::combineItems(int spot1, int spot2) {
	std::pair<int, int> a = inventory[spot1];
	std::pair<int, int> b = inventory[spot2];

	int maxStackSize = ItemMeta::getMaxStackSize(a.first);

	if (b.second == maxStackSize) {
		return;
	}

	if (b.second + a.second > maxStackSize) {
		a.second -= (maxStackSize - b.second);
		b.second = maxStackSize;
	} else {
		b.second += a.second;
		a.first = -1;
		a.second = 0;
	}

	inventory[spot1] = a;
	inventory[spot2] = b;

}

void Inventory::removeItemAtIndex(int index, int amnt) {
	int id = inventory[index].first;
	int curAmnt = inventory[index].second;
	curAmnt -= amnt;
	if (curAmnt <= 0) {
		id = -1;
		curAmnt = 0;
	}

	inventory[index] = std::pair<int, int>(id, curAmnt);
}

void Inventory::removeItemFromInv(int item, int amnt) {
	int extra = amnt;
	for (int i = 0; i < INV_SIZE; i++) {
		std::pair<int, int> cur = inventory[i];

		if (cur.first == item) {
			int toRemove = extra;
			if (cur.second - toRemove < 0) {
				toRemove = cur.second;
			}

			cur.second -= toRemove;
			extra -= toRemove;
			if (cur.second == 0) {
				cur.first = -1;
			}
			inventory[i] = cur;
		}


	}



}


void Inventory::setItemAtIndex(int itemId, int amnt, int index) {
	inventory[index] = std::pair<int, int>(itemId, amnt);
}