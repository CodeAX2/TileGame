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

	private:
		std::vector<std::pair<int, int>> inventory;

	};

}
