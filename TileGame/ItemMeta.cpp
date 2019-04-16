#include "ItemMeta.h"
#include "Entity.h"
#include "Player.h"
#include "TreasureChest.h"
#include "Workbench.h"
#include "Smelter.h"
#include "Torch.h"
#include "Fern.h"

using namespace tg;


std::map<int, ItemMeta*> ItemMeta::allItemMetas;


void ItemMeta::init() {

	for (int i = 0; i <= MAX_ITEM_ID; i++) {
		new ItemMeta(i);
		allItemMetas.at(i)->setEntityDamage(TREE_E, 5);
		allItemMetas.at(i)->setEntityDamage(ZOMBIE_E, 5);
		allItemMetas.at(i)->setEntityDamage(SKELETON_E, 5);
		allItemMetas.at(i)->setEntityDamage(ROCK_E, 0);
		allItemMetas.at(i)->setEntityDamage(ORE_E, 0);
		allItemMetas.at(i)->setEntityDamage(PALM_TREE_E, 5);
	}

	// Set damages

	allItemMetas[2]->setEntityDamage(TREE_E, 20);
	allItemMetas[2]->setEntityDamage(ZOMBIE_E, 10);
	allItemMetas[2]->setEntityDamage(SKELETON_E, 10);
	allItemMetas[2]->setEntityDamage(PALM_TREE_E, 20);

	allItemMetas[6]->setEntityDamage(ZOMBIE_E, 20);
	allItemMetas[6]->setEntityDamage(SKELETON_E, 20);

	allItemMetas[7]->setEntityDamage(ZOMBIE_E, 10);
	allItemMetas[7]->setEntityDamage(SKELETON_E, 10);

	allItemMetas[9]->setEntityDamage(ROCK_E, 10);
	allItemMetas[9]->setEntityDamage(ORE_E, 10);

	allItemMetas[30]->setEntityDamage(TREE_E, 10);
	allItemMetas[30]->setEntityDamage(PALM_TREE_E, 10);

	allItemMetas[31]->setEntityDamage(ROCK_E, 20);
	allItemMetas[31]->setEntityDamage(ORE_E, 20);

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

	allItemMetas[11]->setPlaceFunction(
		[](World* world, Handler* handler) {

		sf::Vector2i htp = world->getHighlightedTile();
		if (allItemMetas[11]->tileIsPlacable(world->getTile(htp.x, htp.y))) {

			new Workbench(htp.x, htp.y, handler, world);
			handler->player->removeItemFromHotbar();
		}

	});

	allItemMetas[13]->setPlaceFunction(
		[](World* world, Handler* handler) {

		sf::Vector2i htp = world->getHighlightedTile();
		if (allItemMetas[11]->tileIsPlacable(world->getTile(htp.x, htp.y))) {

			new Smelter(htp.x, htp.y, handler, world);
			handler->player->removeItemFromHotbar();
		}

	});

	allItemMetas[32]->setPlaceFunction(
		[](World* world, Handler* handler) {

		sf::Vector2i htp = world->getHighlightedTile();
		if (allItemMetas[32]->tileIsPlacable(world->getTile(htp.x, htp.y))) {

			new Torch(htp.x, htp.y, handler, world);
			handler->player->removeItemFromHotbar();
		}

	});

	allItemMetas[33]->setPlaceFunction(
		[](World* world, Handler* handler) {

		sf::Vector2i htp = world->getHighlightedTile();
		if (allItemMetas[33]->tileIsPlacable(world->getTile(htp.x, htp.y))) {

			new Fern(htp.x, htp.y, handler, world);
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

	allItemMetas[11]->addPlacableTile(0);
	allItemMetas[11]->addPlacableTile(3);
	allItemMetas[11]->addPlacableTile(4);
	allItemMetas[11]->addPlacableTile(5);
	allItemMetas[11]->addPlacableTile(6);
	allItemMetas[11]->addPlacableTile(7);

	allItemMetas[13]->addPlacableTile(0);
	allItemMetas[13]->addPlacableTile(3);
	allItemMetas[13]->addPlacableTile(4);
	allItemMetas[13]->addPlacableTile(5);
	allItemMetas[13]->addPlacableTile(6);
	allItemMetas[13]->addPlacableTile(7);

	allItemMetas[32]->addPlacableTile(0);
	allItemMetas[32]->addPlacableTile(3);
	allItemMetas[32]->addPlacableTile(4);
	allItemMetas[32]->addPlacableTile(5);
	allItemMetas[32]->addPlacableTile(6);
	allItemMetas[32]->addPlacableTile(7);

	allItemMetas[33]->addPlacableTile(0);

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

	allItemMetas[15]->setInteractFunction(
		[](World* world, Handler* handler) {
		int amountToIncreasyBy = 30;

		handler->player->removeItemFromHotbar();
		int health = handler->player->getHealth();
		health += amountToIncreasyBy;
		if (health > handler->player->getMaxHealth()) {
			health = handler->player->getMaxHealth();
		}

		handler->player->setHealth(health);

	});

	allItemMetas[34]->setInteractFunction(
		[](World* world, Handler* handler) {
		int amountToIncreasyBy = 10;

		handler->player->removeItemFromHotbar();
		int health = handler->player->getHealth();
		health += amountToIncreasyBy;
		if (health > handler->player->getMaxHealth()) {
			health = handler->player->getMaxHealth();
		}

		handler->player->setHealth(health);

	});

	// Set recipes

	allItemMetas[2]->craftable = true;
	allItemMetas[2]->setRecipeItem(24, 3);
	allItemMetas[2]->setRecipeItem(1, 2);

	allItemMetas[5]->craftable = true;
	allItemMetas[5]->setRecipeItem(0, 2);
	allItemMetas[5]->setRecipeItem(1, 1);

	allItemMetas[6]->craftable = true;
	allItemMetas[6]->setRecipeItem(24, 3);
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

	allItemMetas[11]->craftable = true;
	allItemMetas[11]->setRecipeItem(0, 10);
	allItemMetas[11]->setRecipeItem(1, 10);

	allItemMetas[13]->craftable = true;
	allItemMetas[13]->setRecipeItem(10, 10);
	allItemMetas[13]->setRecipeItem(1, 10);

	allItemMetas[15]->craftable = true;
	allItemMetas[15]->setRecipeItem(14, 3);
	allItemMetas[15]->setRecipeItem(1, 1);

	allItemMetas[30]->craftable = true;
	allItemMetas[30]->setRecipeItem(0, 5);
	allItemMetas[30]->setRecipeItem(1, 2);

	allItemMetas[31]->craftable = true;
	allItemMetas[31]->setRecipeItem(24, 3);
	allItemMetas[31]->setRecipeItem(1, 2);

	allItemMetas[32]->craftable = true;
	allItemMetas[32]->setRecipeItem(0, 1);
	allItemMetas[32]->setRecipeItem(12, 1);
	allItemMetas[32]->setRecipeItem(1, 1);

	// Set max stack size
	allItemMetas[2]->setMaxStackAmount(1);

	allItemMetas[6]->setMaxStackAmount(1);

	allItemMetas[7]->setMaxStackAmount(1);

	allItemMetas[9]->setMaxStackAmount(1);

	allItemMetas[30]->setMaxStackAmount(1);

	allItemMetas[31]->setMaxStackAmount(1);

	// Set fuels
	allItemMetas[0]->burnable = true;
	allItemMetas[0]->burnSpeed = 1;

	allItemMetas[5]->burnable = true;
	allItemMetas[5]->burnSpeed = 1;

	allItemMetas[7]->burnable = true;
	allItemMetas[7]->burnSpeed = 1;

	allItemMetas[8]->burnable = true;
	allItemMetas[8]->burnSpeed = 1;

	allItemMetas[9]->burnable = true;
	allItemMetas[9]->burnSpeed = 1;

	allItemMetas[11]->burnable = true;
	allItemMetas[11]->burnSpeed = 1;

	allItemMetas[12]->burnable = true;
	allItemMetas[12]->burnSpeed = 1.2f;

	allItemMetas[14]->burnable = true;
	allItemMetas[14]->burnSpeed = 0.8f;

	allItemMetas[30]->burnable = true;
	allItemMetas[30]->burnSpeed = 1;

	allItemMetas[32]->burnable = true;
	allItemMetas[32]->burnSpeed = 1;

	allItemMetas[33]->burnable = true;
	allItemMetas[33]->burnSpeed = 0.8f;

	// Set smeltables
	allItemMetas[0]->smeltable = true;
	allItemMetas[0]->smeltResult = std::pair<int, int>(12, 1);

	allItemMetas[16]->smeltable = true;
	allItemMetas[16]->smeltResult = std::pair<int, int>(23, 1);

	allItemMetas[17]->smeltable = true;
	allItemMetas[17]->smeltResult = std::pair<int, int>(24, 1);

	allItemMetas[18]->smeltable = true;
	allItemMetas[18]->smeltResult = std::pair<int, int>(25, 1);

	allItemMetas[19]->smeltable = true;
	allItemMetas[19]->smeltResult = std::pair<int, int>(26, 1);

	allItemMetas[20]->smeltable = true;
	allItemMetas[20]->smeltResult = std::pair<int, int>(27, 1);

	allItemMetas[21]->smeltable = true;
	allItemMetas[21]->smeltResult = std::pair<int, int>(28, 1);

	allItemMetas[22]->smeltable = true;
	allItemMetas[22]->smeltResult = std::pair<int, int>(29, 1);

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

bool ItemMeta::isBurnable(int itemId) {
	if (itemId != -1)
		return allItemMetas[itemId]->burnable;
	return false;
}
float ItemMeta::getBurnSpeed(int itemId) {
	if (itemId != -1)
		return allItemMetas[itemId]->burnSpeed;
	return 0;
}

bool ItemMeta::isSmelatable(int itemId) {
	if (itemId != -1)
		return allItemMetas[itemId]->smeltable;
	return false;
}
std::pair<int, int> ItemMeta::getSmeltResult(int itemId) {
	if (itemId != -1)
		return allItemMetas[itemId]->smeltResult;
	return std::pair<int, int>(-1, -1);
}