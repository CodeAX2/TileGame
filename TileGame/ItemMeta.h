#pragma once
#include <vector>
#include <map>
#include <functional>
#include "World.h"

namespace tg {
	// Basically, this class contains static data about items
	// I.E. how much damage each item does to what entity (axes do more to trees, swords more to monsters)
	class ItemMeta {

	private:
		int id;
		std::map<int, int> entityDamageMap; // First: entity type to damage, Second: amount of damage
		std::function<void(World*, Handler*)> placeFunction; // Function to run when placed
		bool placable = false; // If the item can be placed in the world
		std::function<void(World*, Handler*)> interactFunction; // Function to run when used
		bool interactable = false; // If the item can be used
		std::vector<int> placableTiles; // What tiles the item can be placed on
		bool craftable = false; // If the item can be crafted
		std::map<int, int> recipe; // First: item id used in crafting, Second: amount of item
		int maxStackAmount = 99; // Maximum amount that can occupy 1 slot in the inventory
		bool burnable = false; // If the item can be used as fuel in a smelter
		float burnSpeed = 0; // Speed multiplier for burning
		bool smeltable = false; // If the item can be smelted
		std::pair<int, int> smeltResult = std::pair<int, int>(-1, -1); // Resulting item from smelting

	private:
		ItemMeta(int id);

	private:
		void setEntityDamage(int entityType, int damage);
		bool isPlacable() { return placable; }
		void setPlaceFunction(std::function<void(World*, Handler*)> function) {
			placeFunction = function;
			placable = true;
		}

		void setInteractFunction(std::function<void(World*, Handler*)> function) {
			interactFunction = function;
			interactable = true;
		}
		void setPlacableTiles(std::vector<int> placableTilesList) { placableTiles = placableTilesList; }
		void addPlacableTile(int tileId) { placableTiles.push_back(tileId); }
		bool tileIsPlacable(int tileId) { return std::find(placableTiles.begin(), placableTiles.end(), tileId) != placableTiles.end(); }
		void setRecipeItem(int item, int amount) { recipe.insert_or_assign(item, amount); }

		void setMaxStackAmount(int amount) { maxStackAmount = amount; }


	private:
		static std::map<int, ItemMeta*> allItemMetas;
		static const int BASE_DAMAGE = 20;
		static const int MAX_ITEM_ID = 34;

	public:
		static void init();
		static int getItemDamage(int entityType, int itemId);
		static void placeItem(int itemId, World* world, Handler* handler);
		static void useItem(int itemId, World* world, Handler* handler);
		static bool itemIsPlacable(int itemId);
		static bool canPlaceOnTile(int itemId, int tileId);
		static bool itemIsUsable(int itemId);
		static bool isCraftable(int itemId);
		static std::map<int, int> getCraftingRecipe(int itemId);
		static std::vector<int> getCraftableItems();
		static int getMaxStackSize(int itemId);
		static bool isBurnable(int itemId);
		static float getBurnSpeed(int itemId);
		static bool isSmelatable(int itemId);
		static std::pair<int, int> getSmeltResult(int itemId);

	};
}