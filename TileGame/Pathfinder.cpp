#include "Pathfinder.h"
#include "Building.h"
#include <math.h>
#include "InputManager.h"
#include "Player.h"

using namespace tg;

Pathfinder::Pathfinder(float x, float y, Handler* handler, World* world, float speed) :
	Entity(x, y, handler, 8, 8, 48, 48, 64, 64, true, PATHFINDER, true, world),
	speed(speed), pathThread(&Pathfinder::generatePath, this) {
	pathThread.launch();

}


Pathfinder::~Pathfinder() {
	pathThread.terminate();
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
	}


	if (!active) {
		// If we are too far from following, don't move.
		return;
	}

	if (currentPath.size() != 0 && spotInpath < currentPath.size()) {
		float dx = currentPath[spotInpath].x * 16 - (x + hitBoxX);
		float dy = currentPath[spotInpath].y * 16 - (y + hitBoxY);

		if (oldDX == 0) {
			oldDX = dx;
		}

		if (oldDY == 0) {
			oldDY = dy;
		}


		float angle = atan2(dy, dx);

		if (dx < 0 && oldDX > 0 || dx > 0 && oldDX < 0 || dy < 0 && oldDY > 0 || dy > 0 && oldDY < 0) {
			spotInpath++;
			dx = currentPath[spotInpath].x * 16 - (x + hitBoxX);
			dy = currentPath[spotInpath].y * 16 - (y + hitBoxY);
			angle = atan2(dy, dx);

			oldDX = dx;
			oldDY = dy;

		}



		float dist = .1f * dt;
		float newX = x, newY = y;

		newX += dist * cos(angle);

		if (checkForCollision(newX, newY)) {
			newX -= dist * cos(angle);
		}

		newY += dist * sin(angle);
		if (checkForCollision(newX, newY)) {
			newY -= dist * sin(angle);
		}


		roundedHitBox.left = newX + hitBoxX;
		roundedHitBox.top = newY + hitBoxY;

		x = newX;
		y = newY;

		world->getEntityManager()->fixEntityMoved(this);

	}






}

void Pathfinder::render(Handler* handler) {
	Entity::render(handler);


	if (handler->inputManager->hitboxIsShown()) {
		std::vector<sf::Vector2i> cP = currentPath;
		for (sf::Vector2i pos : cP) {
			sf::RectangleShape s;
			s.setPosition(sf::Vector2f(pos.x * 16 - handler->camera->getXOffset(), pos.y * 16 - handler->camera->getYOffset()));
			s.setSize(sf::Vector2f(16, 16));
			s.setFillColor(sf::Color(0, 0, 255, 175));
			handler->window->draw(s);
		}
	}

}



void Pathfinder::generatePath() {


	sf::Int32 msToWait = 75;
	sf::Clock clock;
	while (true) {
		if (!active || following->getWorld() != world) {
			sf::sleep(sf::milliseconds(msToWait));
			continue;
		}
		sf::Int32 begin = clock.getElapsedTime().asMilliseconds();

		int sizeX = world->getWidth() * 96 / 16;
		int sizeY = world->getHeight() * 96 / 16;

		// Format [x][y]
		std::vector<std::vector<Node>> map(sizeX, std::vector<Node>(sizeY));

		std::vector<sf::Vector2i> openList; // Contain the indicies of the nodes in the map;
		std::vector<sf::Vector2i> closedList;


		sf::Vector2i start(ceil(x / 16), ceil(y / 16));
		sf::Vector2i target(ceil(following->getCollisionBox().left / 16), ceil(following->getCollisionBox().top / 16));

		sf::Vector2i ogTarget = target;
		int startX = -1;
		int endX = 1;
		int startY = -1;
		int endY = 1;

		int tcx = startX;
		int tcy = startY;

		while (checkForCollision(target.x * 16, target.y * 16, false)) {
			target = ogTarget + sf::Vector2i(tcx, tcy);
			tcx++;
			if (tcx > endX) {
				tcx = startX;
				tcy++;
				if (tcy > endY) {
					startX -= 1;
					endX += 1;
					startY -= 1;
					endY -= 1;
					tcx = startX;
					tcy = startY;
				}
			}
		}



		openList.push_back(start);

		while (openList.size() > 0) {

			sf::Vector2i currentNodePos = openList[0];
			int index = 0;
			for (int i = 1; i < openList.size(); i++) {
				if (map[openList[i].x][openList[i].y].getFValue() < map[currentNodePos.x][currentNodePos.y].getFValue()) {
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
				while (map[curPath.x][curPath.y].getParentX() != -1) {
					path.insert(path.begin(), curPath);
					Node child = map[curPath.x][curPath.y];
					sf::Vector2i newPath(child.getParentX(), child.getParentY());
					curPath = newPath;
				}
				currentPath = path;
				spotInpath = 0;
				break;
			}

			sf::Vector2i neighborDelta[4] = { sf::Vector2i(-1,0), sf::Vector2i(1,0), sf::Vector2i(0,-1), sf::Vector2i(0,1) };
			for (int i = 0; i < 4; i++) {
				sf::Vector2i currentNeighbor = currentNodePos + neighborDelta[i];

				if (std::find(closedList.begin(), closedList.end(), currentNeighbor) != closedList.end()
					|| checkForCollision(currentNeighbor.x * 16, currentNeighbor.y * 16, false)) {
					// Already in closed list, or it would cause a collision
					continue;
				}

				if (std::find(openList.begin(), openList.end(), currentNeighbor) == openList.end()) {
					// Not in open list
					addChild(currentNeighbor.x, currentNeighbor.y, currentNodePos.x, currentNodePos.y, &map, &openList, target.x, target.y);
				} else {
					// Already in open list
					Node& existingNode = map[currentNeighbor.x][currentNeighbor.y];
					if (existingNode.getGValue() > map[currentNodePos.x][currentNodePos.y].getGValue() + 1) {
						existingNode.setGValue(map[currentNodePos.x][currentNodePos.y].getGValue() + 1);
						existingNode.setParent(currentNodePos.x, currentNodePos.y);
					}
				}


			}






		}

		sf::Int32 end = clock.getElapsedTime().asMilliseconds();
		sf::sleep(sf::milliseconds(msToWait - (end - begin)));


	}
}

void Pathfinder::addChild(int x, int y, int parentX, int parentY, std::vector<std::vector<Node>>* map, std::vector<sf::Vector2i>* openList, int targetX, int targetY) {
	sf::Vector2i child = sf::Vector2i(x, y);
	map->at(x).at(y).setParent(parentX, parentY);
	map->at(x).at(y).setGValue(map->at(parentX).at(parentY).getGValue() + 1);
	map->at(x).at(y).setHValue(abs(x - targetX) + abs(y - targetY));
	openList->push_back(sf::Vector2i(x, y));

}




bool Pathfinder::checkForCollision(float nX, float nY, bool collideWithPlayer) {
	sf::IntRect pBox = sf::IntRect(sf::Vector2i(std::round(nX + hitBoxX), std::round(nY + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
	EntityManager* em = world->getEntityManager();

	int sX, sY, eX, eY;

	sX = pBox.left / 96;
	sY = pBox.top / 96;
	eX = (pBox.left + pBox.width) / 96;
	eY = (pBox.top + pBox.height) / 96;

	if (sX < 0)
		sX = 0;
	if (sY < 0)
		sY = 0;

	for (int y = sY; y <= eY; y++) {
		for (int x = sX; x <= eX; x++) {

			std::vector<Entity*> entitiesAtTile = em->getEntitiesAtTile(x, y);

			for (Entity* cur : entitiesAtTile) {

				if (cur == this) {
					continue;
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




