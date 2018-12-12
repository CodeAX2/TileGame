#include "Pathfinder.h"
#include "Building.h"
#include <math.h>
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
		if (pow(following->getX() - x, 2) + pow(following->getY() - y, 2) >= 960 * 960) {
			active = false;
		} else {
			active = true;
		}

		destX = following->getX();
		destY = following->getY();

	}

	if (!active) {
		return;
	}

	// Attempt to move to destination
	float deltaX = destX - x;
	float deltaY = destY - y;
	float deg = atan2(deltaY, deltaX); // atan2 can determine the quadrant! :D 

	float nX = x, nY = y;

	if (abs(x - destX) >= .1f) {
		nX += cos(deg) * speed * dt * .1;
		if (checkForCollision(nX, nY)) {
			nX -= cos(deg) * speed * dt * .1;
		} else {
			moved = true;
		}
	}

	if (abs(y - destY) >= .1f) {
		nY += sin(deg) * speed * dt * .1;
		if (checkForCollision(nX, nY)) {
			nY -= sin(deg) * speed * dt * .1;
		} else {
			moved = true;
		}
	}

	if (x < destX && nX > destX || x > destX && nX < destX) {
		nX = destX;
	}

	if (y < destY && nY > destY || y > destY && nY < destY) {
		nY = destY;
	}

	x = nX; y = nY;

	if (moved) {
		world->getEntityManager()->fixEntityMoved(this);
	}
}




bool Pathfinder::checkForCollision(float nX, float nY) {
	sf::IntRect pBox = sf::IntRect(sf::Vector2i(std::round(nX + hitBoxX), std::round(nY + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
	roundedHitBox = pBox;
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




