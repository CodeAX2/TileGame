#pragma once
#include <utility>
#include <vector>
namespace tg {

	class Inventory {

	public:
		static const int INV_SIZE = 23;

	public:
		Inventory();
		Inventory(std::vector<std::pair<int, int>> inventory);
		~Inventory();

	public:
		std::vector<std::pair<int, int>> getInventory() { return inventory; }
		void addItemToInv(int item, int amount);
		int getAmountOfItem(int item);
		void swapItems(int spot1, int spot2);
		void combineItems(int spot1, int spot2);
		void removeItemAtIndex(int index, int amnt);
		void removeItemFromInv(int item, int amnt);
		void setItemAtIndex(int itemId, int amnt, int index);
		void clear();

	private:
		std::vector<std::pair<int, int>> inventory;

	};

}
