#include "ItemMeta.h"
#include "Entity.h"
using namespace tg;


std::map<int, ItemMeta*> ItemMeta::allItemMetas;


void ItemMeta::init() {

	for (int i = 0; i <= 6; i++) {
		new ItemMeta(i);
		allItemMetas.at(i)->setEntityDamage(TREE_E, 5);
		allItemMetas.at(i)->setEntityDamage(ZOMBIE_E, 5);
	}

	allItemMetas[2]->setEntityDamage(TREE_E, 20);
	allItemMetas[2]->setEntityDamage(ZOMBIE_E, 10);

	allItemMetas[6]->setEntityDamage(ZOMBIE_E, 20);

}

int ItemMeta::getItemDamage(int entityType, int itemId) {
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

