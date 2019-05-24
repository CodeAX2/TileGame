#include "Pathfinder.h"
#include "Building.h"
#include <math.h>
#include "InputManager.h"
#include "Player.h"
#include "Game.h"

using namespace tg;


Pathfinder::Pathfinder(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, int type, World* world, float speed) :
	Entity(x, y, handler, hitBoxX, hitBoxY, hitBoxW, hitBoxH, w, h, true, type, true, world),
	speed(speed) {

	world->getEntityManager()->addPathfinder(this);

	if (world->getEntityManager()->getNumPathfinders() > world->getMaxNumPathfinders()) {
		world->getEntityManager()->removeEntity(this, true);
		handler->game->debugLog("Could not add pathfinder! Maximum number of pathfinders already exist!");
		std::cout << world->getEntityManager()->getNumPathfinders() << " " << world->getMaxNumPathfinders() << std::endl;
		return;
	}

}


Pathfinder::~Pathfinder() {
}

void Pathfinder::tick(sf::Int32 dt) {

	if (following == nullptr && followingId != GUID_NULL) {
		following = world->getEntityManager()->getEntityById(followingId);
	}

	bool moved = false;

	// Make sure destination is correct
	if (following != nullptr) {
		if (pow(following->getX() - x, 2) + pow(following->getY() - y, 2) >= 960 * 960) {
			active = false;
		} else {
			active = true;
		}
	} else if (destination != sf::Vector2f(-1, -1)) {
		if (pow(destination.x - x, 2) + pow(destination.y - y, 2) >= 960 * 960) {
			active = false;
		} else {
			active = true;
		}
	}


	if (!active) {
		// If we are too far from following, don't move.
		return;
	}

	if (currentPath.size() != 0 && spotInpath < currentPath.size()) {
		float dx = currentPath[spotInpath].x * pathfindSize - (x + hitBoxX);
		float dy = currentPath[spotInpath].y * pathfindSize - (y + hitBoxY);

		float angle = atan2(dy, dx);



		float dist = speed * dt;

		float ogX = x;
		float ogY = y;

		bool moved = true;

		if (checkForCollision(x, y)) {
			// Collision before we moved, so disable collision for a sec
			x += dist * cos(angle);
			y += dist * sin(angle);
		} else {

			x += dist * cos(angle);

			if (checkForCollision(x, y)) {
				x = ogX;
			}

			y += dist * sin(angle);
			if (checkForCollision(x, y)) {
				y = ogY;
			}

			if (x == ogX && y == ogY) {
				moved = false;
			}

		}


		if (checkForCollisionWithFollowing(x, y)) {
			onCollisionWithFollowing(dt);
		}

		float checkX = currentPath[spotInpath].x * pathfindSize;
		float checkY = currentPath[spotInpath].y * pathfindSize;

		if (linesCross(ogX, ogY, x, y, checkX, checkY, checkX + pathfindSize, checkY)) {
			spotInpath++;
		} else if (linesCross(ogX, ogY, x, y, checkX, checkY, checkX, checkY + pathfindSize)) {
			spotInpath++;
		} else if (abs(dx) <= .25f && abs(dy) <= .25f) {
			spotInpath++;
		} else if (spotInpath == 0 && !moved) {
			spotInpath++;
		}




		if (spotInpath >= currentPath.size()) {
			spotInpath = 0;
		}

		if (pathIsQueued) {
			while (adjustingPath);
			adjustingPath = true;
			currentPath = queuedPath;
			pathIsQueued = false;
			spotInpath = 0;
			adjustingPath = false;
		}

		roundedHitBox = sf::IntRect(sf::Vector2i(std::round(x + (float)hitBoxX), std::round(y + (float)hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));

		world->getEntityManager()->fixEntityMoved(this, ogX, ogY);


	} else if (pathIsQueued) {
		while (adjustingPath);
		adjustingPath = true;
		currentPath = queuedPath;
		spotInpath = 0;
		pathIsQueued = false;
		adjustingPath = false;
	}






}

void Pathfinder::render(Handler* handler) {
	Entity::render(handler);


	if (handler->inputManager->hitboxIsShown()) {
		std::vector<sf::Vector2i> cP = currentPath;
		for (sf::Vector2i pos : cP) {
			sf::RectangleShape s;
			s.setPosition(sf::Vector2f(pos.x * pathfindSize - handler->camera->getXOffset(), pos.y * pathfindSize - handler->camera->getYOffset()));
			s.setSize(sf::Vector2f(pathfindSize, pathfindSize));
			s.setFillColor(sf::Color(0, 0, 255, 175));
			handler->window->draw(s);
		}

		if (currentPath.size() != 0) {
			float tX = currentPath[spotInpath].x * pathfindSize - handler->camera->getXOffset();
			float tY = currentPath[spotInpath].y * pathfindSize - handler->camera->getYOffset();
			float cX = x + hitBoxX - handler->camera->getXOffset();
			float cY = y + hitBoxY - handler->camera->getYOffset();

			sf::VertexArray line(sf::LinesStrip, 2);
			line[0].position = sf::Vector2f(cX, cY);
			line[1].position = sf::Vector2f(tX, tY);

			line[0].color = sf::Color::Red;
			line[1].color = sf::Color::Red;


			handler->window->draw(line);
		}


	}

}



void Pathfinder::generatePath() {
	if (!active || ((following == nullptr || following->getRidingOn() != nullptr) && destination == sf::Vector2f(-1, -1))) {
		while (adjustingPath);
		adjustingPath = true;
		queuedPath.clear();
		//pathIsQueued = true;
		adjustingPath = false;
		return;
	}

	int sizeX = ceil((world->getWidth() * 96) / pathfindSize);
	int sizeY = ceil((world->getHeight() * 96) / pathfindSize);

	// Format [x][y]
	std::map<sf::Vector2i, Node, cmpVectorForMap> nodeMap;

	std::vector<sf::Vector2i> openList; // Contain the indicies of the nodes in the map;
	std::vector<sf::Vector2i> closedList;

	sf::Vector2i start = sf::Vector2i(ceil((x + hitBoxX) / pathfindSize), ceil((y + hitBoxY) / pathfindSize));

	if (following == nullptr) {
		target = sf::Vector2i(ceil(destination.x / pathfindSize), ceil(destination.y / pathfindSize));
	} else {
		target = sf::Vector2i(ceil(following->getCollisionBox().left / pathfindSize), ceil(following->getCollisionBox().top / pathfindSize));
	}

	if (checkForCollision(target.x * pathfindSize - hitBoxX, target.y * pathfindSize - hitBoxY, false, false)) {

		int dist = INT_MAX;
		int tdx = 0;
		int tdy = 0;

		for (int tx = -3; tx <= 3; tx++) {
			for (int ty = -3; ty <= 3; ty++) {
				if (!checkForCollision((target.x + tx) * pathfindSize - hitBoxX, (target.y + ty) * pathfindSize - hitBoxY, false, false)) {
					if (abs(tx) + abs(ty) < dist) {
						tdx = tx;
						tdy = ty;
						dist = abs(tx) + abs(ty);
					}
				}
			}
		}


		target.x += tdx;
		target.y += tdy;


		if (checkForCollision(target.x * pathfindSize - hitBoxX, target.y * pathfindSize - hitBoxY, false, false)) {
			while (adjustingPath);
			adjustingPath = true;
			queuedPath.clear();
			//pathIsQueued = true;
			adjustingPath = false;
			return;
		}

	}



	if (checkForCollision(start.x * pathfindSize - hitBoxX, start.y * pathfindSize - hitBoxY, true, true)) {

		int dist = INT_MAX;
		int tdx = 0;
		int tdy = 0;

		for (int tx = -3; tx <= 3; tx++) {
			for (int ty = -3; ty <= 3; ty++) {
				if (!checkForCollision((start.x + tx) * pathfindSize - hitBoxX, (start.y + ty) * pathfindSize - hitBoxY, true, true)) {
					if (abs(tx) + abs(ty) < dist) {
						tdx = tx;
						tdy = ty;
						dist = abs(tx) + abs(ty);
					}
				}
			}
		}


		start.x += tdx;
		start.y += tdy;


		if (checkForCollision(start.x * pathfindSize - hitBoxX, start.y * pathfindSize - hitBoxY, true, true)) {
			while (adjustingPath);
			adjustingPath = true;
			queuedPath.clear();
			adjustingPath = false;
			return;
		}

	}



	openList.push_back(start);
	nodeMap[start] = Node();
	nodeMap[target] = Node();

	while (openList.size() > 0) {

		sf::Vector2i currentNodePos = openList[0];
		int index = 0;
		for (int i = 1; i < openList.size(); i++) {
			if ((nodeMap[sf::Vector2i(openList[i].x, openList[i].y)].getFValue()) < (nodeMap[sf::Vector2i(currentNodePos.x, currentNodePos.y)].getFValue())) {
				currentNodePos = openList[i];
				index = i;
			}
		}

		openList.erase(openList.begin() + index);
		closedList.push_back(currentNodePos);
		if (currentNodePos == target) {
			// Path found!
			std::vector<sf::Vector2i> path;
			sf::Vector2i curPath = currentNodePos;
			while (nodeMap[curPath].getParentX() != -1) {
				path.insert(path.begin(), curPath);
				Node child = nodeMap[curPath];
				sf::Vector2i newPath(child.getParentX(), child.getParentY());
				std::stringstream ss;
				curPath = newPath;
			}

			while (adjustingPath);
			adjustingPath = true;
			queuedPath = path;
			adjustingPath = false;
			pathIsQueued = true;

			break;
		}

		sf::Vector2i neighborDelta[4] = { sf::Vector2i(-1,0), sf::Vector2i(1,0), sf::Vector2i(0,-1), sf::Vector2i(0,1) };
		for (int i = 0; i < 4; i++) {
			sf::Vector2i currentNeighbor = currentNodePos + neighborDelta[i];

			if (std::find(closedList.begin(), closedList.end(), currentNeighbor) != closedList.end()
				|| checkForCollision(currentNeighbor.x * pathfindSize - hitBoxX, currentNeighbor.y * pathfindSize - hitBoxY, false, false)) {
				// Already in closed list
				continue;
			}

			if (std::find(openList.begin(), openList.end(), currentNeighbor) == openList.end()) {
				// Not in open list

				addChild(currentNeighbor.x, currentNeighbor.y, currentNodePos.x, currentNodePos.y, &nodeMap, &openList, target.x, target.y);

			} else {
				// Already in open list
				Node& existingNode = nodeMap[sf::Vector2i(currentNeighbor.x, currentNeighbor.y)];
				if (existingNode.getGValue() > nodeMap[sf::Vector2i(currentNodePos.x, currentNodePos.y)].getGValue() + 1) {
					existingNode.setGValue(nodeMap[sf::Vector2i(currentNodePos.x, currentNodePos.y)].getGValue() + 1);
					existingNode.setParent(currentNodePos.x, currentNodePos.y);
				}
			}


		}

	}

}


void Pathfinder::addChild(int x, int y, int parentX, int parentY, std::map<sf::Vector2i, Node, cmpVectorForMap>* map, std::vector<sf::Vector2i>* openList, int targetX, int targetY) {
	sf::Vector2i child = sf::Vector2i(x, y);
	Node nd;
	nd.setGValue(map->at(sf::Vector2i(parentX, parentY)).getGValue() + 1);
	nd.setHValue(abs(x - targetX) + abs(y - targetY));
	nd.setParent(parentX, parentY);
	(*map)[child] = nd;

	openList->push_back(child);

}




bool Pathfinder::checkForCollision(float nX, float nY, bool collideWithPlayer, bool collideWithOwnType) {
	sf::IntRect pBox = sf::IntRect(sf::Vector2i(std::round(nX + (float)hitBoxX), std::round(nY + (float)hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
	EntityManager* em = world->getEntityManager();

	int sX, sY, eX, eY;

	sX = pBox.left / 96;
	sY = pBox.top / 96;
	eX = (pBox.left + pBox.width) / 96;
	eY = (pBox.top + pBox.height) / 96;

	for (int y = sY; y <= eY; y++) {
		for (int x = sX; x <= eX; x++) {

			std::vector<Entity*> entitiesAtTile = em->getEntitiesAtTile(x, y);

			for (int i = 0; i < entitiesAtTile.size(); i++) {

				Entity* cur = entitiesAtTile[i];

				if (cur == this || cur == nullptr) {
					continue;
				}
				if (cur->type == type) {

					if (!collideWithOwnType) {
						continue;
					}
				}

				sf::IntRect eBox = cur->getCollisionBox();
				if (eBox.intersects(pBox)) {
					return true;
				}

			}


		}
	}

	if (handler->player->getWorld() == world && collideWithPlayer) {
		if (handler->player->getCollisionBox().intersects(pBox)) {
			return true;
		}
	}


	int cX = nX + hitBoxX, cY = nY + hitBoxY;

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


bool Pathfinder::linesCross(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22) {

	float crossX, crossY = NAN;
	float m1 = NAN, m2 = NAN;

	if (x12 - x11 == 0 && x22 - x21 != 0) {
		crossX = x12;
		m2 = (y22 - y21) / (x22 - x21);
	} else if (x22 - x21 == 0 && x12 - x11 != 0) {
		crossX = x22;
		m1 = (y12 - y11) / (x12 - x11);
	} else {

		if (m1 == m2) {
			// Parallel lines
			return false;
		}

		m1 = (y12 - y11) / (x12 - x11);
		m2 = (y22 - y21) / (x22 - x21);

		crossX = (-m2 * x21 + y21 + m1 * x11 - y11) / (m1 - m2);
		crossY = m1 * crossX - m1 * x11 + y11;
	}

	if (crossY == NAN) {
		if (m1 == NAN) {
			crossY = m2 * crossX - m2 * x21 + y21;
		} else {
			crossY = m1 * crossX - m1 * x11 + y11;
		}
	}



	float lowx1 = x11;
	float highx1 = x12;
	if (x12 < lowx1) {
		lowx1 = x12;
		highx1 = x11;
	}

	float lowx2 = x21;
	float highx2 = x22;
	if (x22 < lowx2) {
		lowx2 = x22;
		highx2 = x21;
	}


	float lowy1 = y11;
	float highy1 = y12;
	if (y12 < lowy1) {
		lowy1 = y12;
		highy1 = y11;
	}

	float lowy2 = y21;
	float highy2 = y22;
	if (y22 < lowy2) {
		lowy2 = y22;
		highy2 = y21;
	}


	if (crossX >= lowx1 && crossX <= highx1 && crossX >= lowx2 && crossX <= highx2 &&
		crossY >= lowy1 && crossY <= highy1 && crossY >= lowy2 && crossY <= highy2) {
		return true;
	}

	return false;
}


bool Pathfinder::checkForCollisionWithFollowing(float nX, float nY) {

	sf::IntRect pBox = sf::IntRect(sf::Vector2i(std::round(nX + (float)hitBoxX) - 6, std::round(nY + (float)hitBoxY) - 6), sf::Vector2i(hitBoxW + 12, hitBoxH + 12));

	if (following == nullptr && destination != sf::Vector2f(-1, -1)) {

		float targetX = target.x * pathfindSize;
		float targetY = target.y * pathfindSize;

		if (pBox.contains(targetX, targetY)) {
			return true;
		} else {
			return false;
		}
	}

	if (following == nullptr) {
		return false;
	}

	if (following->getWorld() == world) {
		if (following->getCollisionBox().intersects(pBox)) {
			return true;
		}
	}

	return false;
}


void Pathfinder::onCollisionWithFollowing(sf::Int32 dt) {}



