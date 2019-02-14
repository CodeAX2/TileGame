#include "Inventory.h"
#include <iostream>

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
	for (int i = 0; i < INV_SIZE; i++) {
		std::pair<int, int> cur = inventory[i];
		if (cur.first == item) {
			int toAdd = 99 - cur.second;
			if (extra - toAdd >= 0) {
				cur.second += toAdd;
				extra -= toAdd;
				inventory[i] = cur;
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
				if (extra - 99 >= 0) {

					cur.first = item;
					cur.second = 99;
					extra -= 99;
					inventory[i] = cur;

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
	for (int i = 0; i < INV_SIZE; i++) {
		std::pair<int, int> cur = inventory[i];
		if (cur.first == item) {
			return cur.second;
		}
	}

	return 0;
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

	if (b.second == 99) {
		return;
	}

	if (b.second + a.second > 99) {
		a.second -= (99 - b.second);
		b.second = 99;
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
