#include "Boat.h"
#include "Static.h"
#include "Player.h"

using namespace tg;

Boat::Boat(float x, float y, Handler* handler, World* world) : Rideable(x, y, handler, 4, 4, 32 * 3 - 8, 32 * 3 - 8, 32 * 3, 32 * 3, BOAT_E, world) {
	this->texture = handler->assets->getBoatTexture();
}


Boat::~Boat()
{
}

void Boat::damage(int dmg) {
}

bool Boat::checkForCollision() {
	sf::IntRect pBox = getCollisionBox();

	if (!world->tileIsWater(pBox.left, pBox.top) &&
		!world->tileIsWater(pBox.left + pBox.width, pBox.top) &&
		!world->tileIsWater(pBox.left + pBox.width, pBox.top + pBox.height) &&
		!world->tileIsWater(pBox.left, pBox.top + pBox.height)) {
		if (rider != nullptr) {
			rider->setRiding(nullptr);
			rider = nullptr;
			riderId = UUID();
		}
		world->getEntityManager()->removeEntity(this);
	}

	int cX = x + hitBoxX, cY = y + hitBoxY;

	if (cX < 0 || cX + hitBoxW >= world->getWidth() * 96 ||
		cY < 0 || cY + hitBoxH >= world->getHeight() * 96) {
		return true;
	}


	if (!world->tileIsWater(pBox.left, pBox.top) ||
		!world->tileIsWater(pBox.left + pBox.width, pBox.top) ||
		!world->tileIsWater(pBox.left + pBox.width, pBox.top + pBox.height) ||
		!world->tileIsWater(pBox.left, pBox.top + pBox.height)) {

		if (rider != nullptr) {
			if (!world->tileIsSolid(pBox.left, pBox.top + pBox.height / 2)
				&& Static::tileIsEmpty(pBox.left / 96, (pBox.top + pBox.height / 2) / 96, world)) {

				rider->setPos(((int)pBox.left / 96) * 96 + rider->getWidth() / 2, ((int)(pBox.top + pBox.height / 2) / 96) * 96 - rider->getHeight() + 96 / 2 + 16);
				rider->setRiding(nullptr);
				rider = nullptr;
				riderId = UUID();
				return true;
			}

			if (!world->tileIsSolid(pBox.left + pBox.width / 2, pBox.top) &&
				Static::tileIsEmpty((pBox.left + pBox.width / 2) / 96, pBox.top / 96, world)) {

				rider->setPos(((int)(pBox.left + pBox.width / 2) / 96) * 96 + rider->getWidth() / 2 - 8, ((int)pBox.top / 96) * 96 - rider->getHeight() + 96 / 2 + 24);
				rider->setRiding(nullptr);
				rider = nullptr;
				riderId = UUID();
				return true;
			}

			if (!world->tileIsSolid(pBox.left + pBox.width, pBox.top + pBox.height / 2) &&
				Static::tileIsEmpty((pBox.left + pBox.width) / 96, (pBox.top + pBox.height / 2) / 96, world)) {

				rider->setPos(((int)(pBox.left + pBox.width) / 96) * 96 + rider->getWidth() / 2 - 24, ((int)(pBox.top + pBox.height / 2) / 96) * 96 - rider->getHeight() + 96 / 2 + 16);
				rider->setRiding(nullptr);
				rider = nullptr;
				riderId = UUID();
				return true;
			}

			if (!world->tileIsSolid(pBox.left + pBox.width / 2, pBox.top + pBox.height) &&
				Static::tileIsEmpty((pBox.left + pBox.width / 2) / 96, (pBox.top + pBox.height) / 96, world)) {

				rider->setPos(((int)(pBox.left + pBox.width / 2) / 96) * 96 + rider->getWidth() / 2 - 8, ((int)(pBox.top + pBox.height) / 96) * 96 - rider->getHeight() + 96 / 2 - 8);
				rider->setRiding(nullptr);
				rider = nullptr;
				riderId = UUID();
				return true;
			}
		}



		return true;
	}

	EntityManager* em = world->getEntityManager();

	int sX = pBox.left / 96;
	int sY = pBox.top / 96;
	int eX = (pBox.left + pBox.width) / 96;
	int eY = (pBox.top + pBox.height) / 96;

	for (int y = sY; y <= eY; y++) {
		for (int x = sX; x <= eX; x++) {

			std::vector<Entity*> entitiesAtTile = em->getEntitiesAtTile(x, y);

			for (Entity* cur : entitiesAtTile) {

				if (cur == this || cur == rider) {
					continue;
				}

				sf::IntRect eBox = cur->getCollisionBox();
				if (eBox.intersects(pBox)) {
					return true;
				}
			}
		}
	}

	if (handler->player->getWorld() == world && rider != handler->player) {
		if (handler->player->getCollisionBox().intersects(pBox)) {
			return true;
		}
	}

	return false;
}
