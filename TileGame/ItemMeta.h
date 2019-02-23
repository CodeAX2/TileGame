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
		std::map<int, int> entityDamageMap;
		std::function<void(World*, Handler*)> placeFunction;
		bool placable = false;
		std::function<void(World*, Handler*)> interactFunction;
		bool interactable = false;
		std::vector<int> placableTiles;

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

	private:
		static std::map<int, ItemMeta*> allItemMetas;
		static const int BASE_DAMAGE = 20;

	public:
		static void init();
		static int getItemDamage(int entityType, int itemId);
		static void placeItem(int itemId, World* world, Handler* handler);
		static void useItem(int itemId, World* world, Handler* handler);
		static bool itemIsPlacable(int itemId);
		static bool canPlaceOnTile(int itemId, int tileId);
		static bool itemIsUsable(int itemId);

	};
}
