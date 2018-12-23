#include "Pathfinder.h"
#include "Building.h"
#include <math.h>
#include "InputManager.h"
#include "Player.h"

using namespace tg;

Pathfinder::Pathfinder(float x, float y, Handler* handler, World* world, float speed) :
	Entity(x, y, handler, 0, 0, 16, 16, 64, 64, true, PATHFINDER, true, world),
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
		float dx = currentPath[spotInpath].x*hitBoxW - (x + hitBoxX);
		float dy = currentPath[spotInpath].y*hitBoxH - (y + hitBoxY);


		if (initX == 0) {
			initX = x + hitBoxX;
		}

		if (initY == 0) {
			initY = y + hitBoxY;
		}


		float angle = atan2(dy, dx);



		float dist = .2f * dt;


		float ogX = x;
		float ogY = y;
		float newX = x;
		float newY = y;

		newX += dist * cos(angle);

		if (checkForCollision(x, y)) {
			newX = ogX;
		}

		newY += dist * sin(angle);
		if (checkForCollision(x, y)) {
			newY = ogY;
		}

		x = newX;
		y = newY;


		float checkX = currentPath[spotInpath].x*hitBoxW;
		float checkY = currentPath[spotInpath].y*hitBoxH;

		if (initX < checkX && x + hitBoxX > checkX || initX > checkX && x + hitBoxX < checkX ||
			initY < checkY && y + hitBoxY > checkY || initY > checkY && y + hitBoxY < checkY ||
			(dx == 0 && dy == 0)) {

			if (pathIsQueued) {
				currentPath = queuedPath;
				pathIsQueued = false;
				spotInpath = 0;
			} else {
				spotInpath++;
			}
			dx = currentPath[spotInpath].x*hitBoxW - (x + hitBoxX);
			dy = currentPath[spotInpath].y*hitBoxH - (y + hitBoxY);
			angle = atan2(dy, dx);

			initX = x + hitBoxX;
			initY = y + hitBoxY;

		}

		roundedHitBox.left = x + hitBoxX;
		roundedHitBox.top = y + hitBoxY;

		world->getEntityManager()->fixEntityMoved(this);


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
			s.setPosition(sf::Vector2f(pos.x * hitBoxW - handler->camera->getXOffset(), pos.y*hitBoxH - handler->camera->getYOffset()));
			s.setSize(sf::Vector2f(hitBoxW, hitBoxH));
			s.setFillColor(sf::Color(0, 0, 255, 175));
			handler->window->draw(s);
		}

		if (currentPath.size() != 0) {
			float tX = currentPath[spotInpath].x*hitBoxW - handler->camera->getXOffset();
			float tY = currentPath[spotInpath].y*hitBoxH - handler->camera->getYOffset();
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
		if (!active) {
			sf::sleep(sf::milliseconds(msToWait));
		}
		sf::Int32 begin = clock.getElapsedTime().asMilliseconds();

		int sizeX = ceil(x / hitBoxW) + ceil((world->getWidth() * 96 - x) / hitBoxW);
		int sizeY = ceil(y / hitBoxH) + ceil((world->getHeight() * 96 - y) / hitBoxH);

		// Format [x][y]
		std::vector<std::vector<Node>> map(sizeX, std::vector<Node>(sizeY));

		std::vector<sf::Vector2i> openList; // Contain the indicies of the nodes in the map;
		std::vector<sf::Vector2i> closedList;

		sf::Vector2i start;

		if (currentPath.size() != 0) {
			start = sf::Vector2i(currentPath[spotInpath]);
		} else {
			start = sf::Vector2i(ceil((x + hitBoxX) / hitBoxW), ceil((y + hitBoxY) / hitBoxH));
		}
		sf::Vector2i target(ceil(following->getCollisionBox().left / hitBoxW), ceil(following->getCollisionBox().top / hitBoxH));

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
					|| checkForCollision(currentNeighbor.x * hitBoxW, currentNeighbor.y * hitBoxH, false)) {
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




bool Pathfinder::checkForCollision(float nX, float nY, bool collideWithPlayer) {
	sf::IntRect pBox = sf::IntRect(sf::Vector2i(std::round(nX + hitBoxX), std::round(nY + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
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




