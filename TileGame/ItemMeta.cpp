#include "ItemMeta.h"
#include "Entity.h"
#include "Player.h"
#include "TreasureChest.h"

using namespace tg;


std::map<int, ItemMeta*> ItemMeta::allItemMetas;


void ItemMeta::init() {

	for (int i = 0; i <= MAX_ITEM_ID; i++) {
		new ItemMeta(i);
		allItemMetas.at(i)->setEntityDamage(TREE_E, 5);
		allItemMetas.at(i)->setEntityDamage(ZOMBIE_E, 5);
		allItemMetas.at(i)->setEntityDamage(ROCK_E, 5);
		allItemMetas.at(i)->setMaxStackAmount(99);
	}

	// Set damages

	allItemMetas[2]->setEntityDamage(TREE_E, 20);
	allItemMetas[2]->setEntityDamage(ZOMBIE_E, 10);

	allItemMetas[6]->setEntityDamage(ZOMBIE_E, 20);

	allItemMetas[7]->setEntityDamage(ZOMBIE_E, 10);

	allItemMetas[9]->setEntityDamage(ROCK_E, 10);

	// Set place functions
	allItemMetas[5]->setPlaceFunction(
		[](World* world, Handler* handler) {

		sf::Vector2i htp = world->getHighlightedTile();
		if (allItemMetas[5]->tileIsPlacable(world->getTile(htp.x, htp.y))) {

			world->setTile(htp.x, htp.y, 4);
			handler->player->removeItemFromHotbar();
		}

	});

	allItemMetas[8]->setPlaceFunction(
		[](World* world, Handler* handler) {

		sf::Vector2i htp = world->getHighlightedTile();
		if (allItemMetas[8]->tileIsPlacable(world->getTile(htp.x, htp.y))) {

			new TreasureChest(htp.x, htp.y, handler, world);
			handler->player->removeItemFromHotbar();
		}

	});

	// Set placable tiles
	allItemMetas[5]->addPlacableTile(2);

	allItemMetas[8]->addPlacableTile(0);
	allItemMetas[8]->addPlacableTile(3);
	allItemMetas[8]->addPlacableTile(4);
	allItemMetas[8]->addPlacableTile(5);
	allItemMetas[8]->addPlacableTile(6);
	allItemMetas[8]->addPlacableTile(7);

	// Set interact functions
	allItemMetas[3]->setInteractFunction(
		[](World* world, Handler* handler) {
		int amountToIncreasyBy = 10;

		handler->player->removeItemFromHotbar();
		int stam = handler->player->getStamina();
		stam += amountToIncreasyBy;
		if (stam > handler->player->getMaxStamina()) {
			stam = handler->player->getMaxStamina();
		}

		handler->player->setStamina(stam);

	});

	allItemMetas[4]->setInteractFunction(
		[](World* world, Handler* handler) {
		int amountToIncreasyBy = 20;

		handler->player->removeItemFromHotbar();
		int stam = handler->player->getStamina();
		stam += amountToIncreasyBy;
		if (stam > handler->player->getMaxStamina()) {
			stam = handler->player->getMaxStamina();
		}

		handler->player->setStamina(stam);

	});

	// Set recipes

	allItemMetas[2]->craftable = true;
	allItemMetas[2]->setRecipeItem(10, 5);
	allItemMetas[2]->setRecipeItem(1, 2);

	allItemMetas[5]->craftable = true;
	allItemMetas[5]->setRecipeItem(0, 2);
	allItemMetas[5]->setRecipeItem(1, 1);

	allItemMetas[6]->craftable = true;
	allItemMetas[6]->setRecipeItem(10, 5);
	allItemMetas[6]->setRecipeItem(1, 2);

	allItemMetas[7]->craftable = true;
	allItemMetas[7]->setRecipeItem(0, 5);
	allItemMetas[7]->setRecipeItem(1, 2);

	allItemMetas[8]->craftable = true;
	allItemMetas[8]->setRecipeItem(0, 10);
	allItemMetas[8]->setRecipeItem(1, 5);

	allItemMetas[9]->craftable = true;
	allItemMetas[9]->setRecipeItem(0, 5);
	allItemMetas[9]->setRecipeItem(1, 2);

	// Set max stack size
	allItemMetas[2]->setMaxStackAmount(1);

	allItemMetas[6]->setMaxStackAmount(1);

	allItemMetas[7]->setMaxStackAmount(1);

	allItemMetas[9]->setMaxStackAmount(1);

}

bool ItemMeta::itemIsPlacable(int itemId) {
	if (itemId == -1)
		return false;
	return allItemMetas[itemId]->isPlacable();
}

void ItemMeta::placeItem(int itemId, World* world, Handler* handler) {
	if (itemId == -1)
		return;
	if (allItemMetas[itemId]->isPlacable())
		allItemMetas[itemId]->placeFunction(world, handler);
}

bool ItemMeta::canPlaceOnTile(int itemId, int tileId) {
	return allItemMetas[itemId]->tileIsPlacable(tileId);
}

int ItemMeta::getItemDamage(int entityType, int itemId) {
	if (itemId == -1) {
		itemId = 0; // Use the log as the base hitting item
	}
	std::map<int, int> curEntityDamageMap = allItemMetas[itemId]->entityDamageMap;
	if (curEntityDamageMap.find(entityType) != curEntityDamageMap.end()) {

		return curEntityDamageMap[entityType];

	}

	return BASE_DAMAGE;

}


ItemMeta::ItemMeta(int id) {

	this->id = id;
	allItemMetas.insert_or_assign(id, this);
	entityDamageMap = std::map<int, int>();
	recipe = std::map<int, int>();

}

void ItemMeta::setEntityDamage(int entityType, int damage) {
	entityDamageMap.insert_or_assign(entityType, damage);
}

bool ItemMeta::itemIsUsable(int itemId) {
	if (itemId != -1)
		return allItemMetas[itemId]->interactable;
	return false;
}

void ItemMeta::useItem(int itemId, World* world, Handler* handler) {
	if (itemId == -1)
		return;
	if (allItemMetas[itemId]->itemIsUsable)
		allItemMetas[itemId]->interactFunction(world, handler);
}

bool ItemMeta::isCraftable(int itemId) {
	if (itemId != -1)
		return allItemMetas[itemId]->craftable;

	return false;

}
std::map<int, int> ItemMeta::getCraftingRecipe(int itemId) {
	if (itemId != -1 && isCraftable(itemId)) {
		return allItemMetas[itemId]->recipe;
	}

	return std::map<int, int>();
}

std::vector<int> ItemMeta::getCraftableItems() {
	std::vector<int> craftables;
	for (int i = 0; i <= MAX_ITEM_ID; i++) {
		if (allItemMetas[i]->craftable) {
			craftables.push_back(i);
		}
	}

	return craftables;

}

int ItemMeta::getMaxStackSize(int itemId) {
	if (itemId != -1)
		return allItemMetas[itemId]->maxStackAmount;
	return 99;
}