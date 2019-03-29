#include "Fern.h"
#include "Item.h"
#include "Tree.h"

using namespace tg;

Fern::Fern(int x, int y, Handler* handler, World* world) :
	Static(x, y, handler, 14 * 3, 29 * 3, 12, 9, 32 * 3, 32 * 3, true, FERN_E, 58, world) {

	texture = handler->assets->getFernTexture();
	health = 60;
	maxHealth = 60;

}


Fern::~Fern() {
}


void Fern::tick(sf::Int32 dt) {

	timeAlive += dt;

	if (timeAlive > initialGrowTime) {
		percentToGrow += (1.f / maxGrowTime) * dt;

		timeSinceLastCheck += dt;

		if (timeSinceLastCheck >= maxGrowTime / triesUntilMax) {

			timeSinceLastCheck = 0;

			if (rand() % 100 < percentToGrow * 100) {

				int savedTX = tX;
				int savedTY = tY;
				Handler* savedHandler = handler;
				World* savedWorld = world;

				world->getEntityManager()->removeEntity(this, false);

				if (setSuccessfully) {
					staticList[world][tY][tX] = false;
					setSuccessfully = false;
				}

				new Tree(savedTX, savedTY, savedHandler, 0, savedWorld);

				delete this;
			}
		}

	}

}

void Fern::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 33, world);
}
