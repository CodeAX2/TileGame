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
