#include "Pathfinder.h"
#include "Building.h"

using namespace tg;

Pathfinder::Pathfinder(float x, float y, Handler* handler, World* world, float speed) :
	Entity(x, y, handler, 0, 0, 64, 64, 64, 64, true, PATHFINDER, true, world),
	speed(speed) {
}


Pathfinder::~Pathfinder()
{
}

void Pathfinder::tick(sf::Int32 dt) {

	bool moved = false;

	// Make sure destination is correct
	if (following != nullptr) {
		destX = following->getX();
		destY = following->getY();
	}

	// Attempt to move to destination
	float deltaX = destX - x;
	float deltaY = destY - y;


	x += deltaX * speed * dt * .01;
	if (checkForCollision()) {
		x -= deltaX * speed * dt * .01;
	} else {
		moved = true;
	}
	y += deltaY * speed * dt * .01;
	if (checkForCollision()) {
		y -= deltaY * speed * dt * .01;
	} else {
		moved = true;
	}

	if (moved && deltaX > .1f && deltaY > .1f) {
		world->getEntityManager()->fixEntityMoved(this);
	}


}




bool Pathfinder::checkForCollision() {
	sf::IntRect pBox = sf::IntRect(x + hitBoxX, y + hitBoxY, x + hitBoxW + hitBoxX, y + hitBoxH + hitBoxY);
	EntityManager* em = world->getEntityManager();

	for (int i = 0; i < em->numEntities(); i++) {

		Entity* cur = em->getEntity(i);

		if (cur == this) {
			continue;
		}

		sf::IntRect eBox = cur->getCollisionBox();
		if (eBox.intersects(pBox)) {
			return true;
		}

	}

	int cX = x + hitBoxX, cY = y + hitBoxY;

	if (cX < 0 || cX + hitBoxW >= world->getWidth() * 96 ||
		cY < 0 || cY + hitBoxH >= world->getHeight() * 96) {
		return true;
	}


	if (world->tileIsSolid(pBox.left, pBox.top) ||
		world->tileIsSolid(pBox.left + pBox.width, pBox.top) ||
		world->tileIsSolid(pBox.left + pBox.width, pBox.top + pBox.height) ||
		world->tileIsSolid(pBox.left, pBox.top + pBox.height)) {
		return true;
	}

	if (Building::buildingOccupies(pBox.left / 96, pBox.top / 96, world) ||
		Building::buildingOccupies((pBox.left + pBox.width) / 96, pBox.top / 96, world) ||
		Building::buildingOccupies((pBox.left + pBox.width) / 96, (pBox.top + pBox.height) / 96, world) ||
		Building::buildingOccupies(pBox.left / 96, (pBox.top + pBox.height) / 96, world)) {
		return true;
	}



	return false;
}




