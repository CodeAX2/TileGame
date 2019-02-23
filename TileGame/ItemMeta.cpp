#include "ItemMeta.h"
#include "Entity.h"
#include "Player.h"
#include "TreasureChest.h"

using namespace tg;


std::map<int, ItemMeta*> ItemMeta::allItemMetas;


void ItemMeta::init() {

	for (int i = 0; i <= 8; i++) {
		new ItemMeta(i);
		allItemMetas.at(i)->setEntityDamage(TREE_E, 5);
		allItemMetas.at(i)->setEntityDamage(ZOMBIE_E, 5);
	}

	// Set damages

	allItemMetas[2]->setEntityDamage(TREE_E, 20);
	allItemMetas[2]->setEntityDamage(ZOMBIE_E, 10);

	allItemMetas[6]->setEntityDamage(ZOMBIE_E, 20);

	allItemMetas[7]->setEntityDamage(ZOMBIE_E, 10);

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