#include "Ore.h"
#include "Item.h"

using namespace tg;

const int Ore::oreToItem[9] = { 12,1,16,17,18,19,20,21,22 };

Ore::Ore(int x, int y, Handler* handler, World* world, int oreType) :
	Static(x, y, handler, 6, 32 * 3 - 8 * 3, 32 * 3 - 12, 8 * 3, 32 * 3, 32 * 3, true, ORE_E, 73, world) {

	this->oreType = oreType;

	health = 60;
	maxHealth = 60;

	texture = handler->assets->getOreTexture(oreType);

	world->getEntityManager()->addTickAnywhereEntity(this);

}


Ore::~Ore() {
}


void Ore::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, oreToItem[oreType], world);
}

void Ore::damage(int dmg, Entity* damager) {
	if (regening) return;
	health -= dmg;
	if (health <= 0) {
		health = maxHealth;
		dropItems();
		regening = true;
		cooldown = 180000;
	}
}

void Ore::tick(sf::Int32 dt) {
	if (regening) {
		texture = handler->assets->getRockTexture();
		cooldown -= dt;
		if (cooldown <= 0) {
			cooldown = 0;
			regening = false;
			texture = handler->assets->getOreTexture(oreType);
		}
	} else {
		texture = handler->assets->getOreTexture(oreType);
	}
}