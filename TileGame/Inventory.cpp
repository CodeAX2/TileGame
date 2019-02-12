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
		if (cur.first == item || cur.first == -1 && amount > 0) {
			cur.first = item;
			cur.second += extra;
			if (cur.second == 0) {
				cur.first = -1;
				extra = 0;
			} else if (cur.second > 99) {
				extra = cur.second - 99;
				cur.second = 99;
			} else {
				extra = 0;
			}
			inventory[i] = cur;
			if (extra <= 0)
				return;
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
