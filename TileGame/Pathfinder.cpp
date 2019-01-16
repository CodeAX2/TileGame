#include "Pathfinder.h"
#include "Building.h"
#include <math.h>
#include "InputManager.h"
#include "Player.h"

using namespace tg;

Pathfinder::Pathfinder(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, int type, World* world, float speed) :
	Entity(x, y, handler, hitBoxX, hitBoxY, hitBoxW, hitBoxH, w, h, true, type, true, world),
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
		float dx = currentPath[spotInpath].x * pathfindSize - (x + hitBoxX);
		float dy = currentPath[spotInpath].y * pathfindSize - (y + hitBoxY);

		float angle = atan2(dy, dx);



		float dist = .2f * dt;

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


		float checkX = currentPath[spotInpath].x * pathfindSize;
		float checkY = currentPath[spotInpath].y * pathfindSize;

		if (ogX + hitBoxX < checkX && x + hitBoxX > checkX || ogX + hitBoxX > checkX && x + hitBoxX < checkX ||
			ogY + hitBoxY < checkY && y + hitBoxY > checkY || ogY + hitBoxY > checkY && y + hitBoxY < checkY) {

			spotInpath++;



		} else if (dx == 0 && dy == 0) {
			spotInpath++;
		} else if (dx <= 1 && dy <= 1 && !moved) {
			spotInpath++;
		}

		if (spotInpath >= currentPath.size()) {
			spotInpath = 0;
		}

		if (pathIsQueued) {
			currentPath = queuedPath;
			pathIsQueued = false;
			spotInpath = 0;
		}

		roundedHitBox = sf::IntRect(sf::Vector2i(std::round(x + (float)hitBoxX), std::round(y + (float)hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));

		world->getEntityManager()->fixEntityMoved(this, ogX, ogY);


	} else if (pathIsQueued) {
		currentPath = queuedPath;
		spotInpath = 0;
		pathIsQueued = false;
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
	sf::Int32 msToWait = 300;
	sf::Clock clock;
	while (true) {

		if (!active || following->getRidingOn() != nullptr) {
			sf::sleep(sf::milliseconds(msToWait));
			continue;
		}
		sf::Int32 begin = clock.getElapsedTime().asMilliseconds();

		int sizeX = ceil((world->getWidth() * 96) / pathfindSize);
		int sizeY = ceil((world->getHeight() * 96) / pathfindSize);

		// Format [x][y]
		std::vector<std::vector<Node>> map(sizeX, std::vector<Node>(sizeY));

		std::vector<sf::Vector2i> openList; // Contain the indicies of the nodes in the map;
		std::vector<sf::Vector2i> closedList;

		sf::Vector2i start = sf::Vector2i(ceil((x + hitBoxX) / pathfindSize), ceil((y + hitBoxY) / pathfindSize));;
		sf::Vector2i target(ceil(following->getCollisionBox().left / pathfindSize), ceil(following->getCollisionBox().top / pathfindSize));

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
				sf::sleep(sf::milliseconds(msToWait));
				continue;
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

				//path.insert(path.begin(), start);

				queuedPath = path;
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




bool Pathfinder::checkForCollision(float nX, float nY, bool collideWithPlayer, bool collideWithPathfinder) {
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

			for (Entity* cur : entitiesAtTile) {

				if (cur == this) {
					continue;
				}
				if (Pathfinder* pathCur = dynamic_cast<Pathfinder*>(cur)) {
					if (!collideWithPathfinder) {
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




