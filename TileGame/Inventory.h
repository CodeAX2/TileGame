#pragma once
#include <utility>
#include <vector>
namespace tg {

	class Inventory {

	public:
		Inventory();
		Inventory(std::vector<std::pair<int, int>> inventory);
		~Inventory();

	public:
		static const int INV_SIZE = 23; // Size of an inventory

	private:
		std::vector<std::pair<int, int>> inventory; // The contents of the inventory

	public:
		// Get the contents of the inventory
		std::vector<std::pair<int, int>> getInventory() { return inventory; }

		// Add an amount of a specific item to the inventory
		void addItemToInv(int item, int amount);

		// Get the total amount of a specific item within the inventory
		int getAmountOfItem(int item);

		// Swap two item's locations within the inventory
		void swapItems(int spot1, int spot2);

		// Combine two stacks of items within the inventory
		void combineItems(int spot1, int spot2);

		// Remove all the items at a location within the inventory
		void removeItemAtIndex(int index, int amnt);

		// Remove a static amount of an item from the inventory
		void removeItemFromInv(int item, int amnt);

		// Set the amount and type of the item at a location within the inventory
		void setItemAtIndex(int itemId, int amnt, int index);

		// Remove all of the items from the inventory
		void clear();

	};

}
