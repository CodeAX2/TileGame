#include "Building.h"
#include "Camera.h"
#include "TreasureChest.h"
#include "resource.h"

using namespace tg;

std::vector<Building*> Building::allBuildings;

Building::Building(int x, int y, int width, int height, int entranceX, Handler* handler, World* outsideWorld, World* insideWorld, std::string nameId, int textureToUse) :
	x(x), y(y), width(width), height(height), entranceX(entranceX), handler(handler), entityManager(handler), outsideWorld(outsideWorld), bldgNameId(nameId), textureToUse(textureToUse)
{

	if (this->outsideWorld == nullptr) {
		this->outsideWorld = handler->mainWorld;
	}

	if (buildingsMap[this->outsideWorld].size() == 0) {
		buildingsMap[this->outsideWorld] = std::vector<std::vector<int>>(handler->mainWorld->getHeight(),
			std::vector<int>(handler->mainWorld->getWidth()));
	}

	bool validPosition = true;

	for (int ix = x; ix < x + width; ix++) {
		for (int iy = y; iy < y + height; iy++) {

			if (buildingsMap[this->outsideWorld][iy][ix] != OPEN) {
				validPosition = false;
			}
		}
	}

	if (!validPosition || insideWorld == nullptr) {
		delete this;
		return;
	} else {
		allBuildings.push_back(this);
		for (int ix = x; ix < x + width; ix++) {
			for (int iy = y; iy < y + height; iy++) {

				buildingsMap[this->outsideWorld][iy][ix] = OCCUPIED;
				if (iy == y + height - 1 && ix == x + entranceX) {
					buildingsMap[this->outsideWorld][iy][ix] = ENTRANCE;
				}
			}
		}


		this->insideWorld = insideWorld;




		buildingTexture = handler->assets->getBuildingTexture(textureToUse);
		std::vector<sf::Vector2f> buildingVerticies = handler->assets->getBuildingVerticies(textureToUse);

		buildingEntity = new BuildingEntity(x * 96, (y + height) * 96 - buildingTexture->getSize().y * 3, handler, 0, 0, 0, 0,
			buildingTexture->getSize().x * 3, buildingTexture->getSize().y * 3, true, BUILDING_E, true, this->outsideWorld);

		buildingEntity->setTexture(buildingTexture);
		buildingEntity->setVerticies(buildingVerticies);
	}


}


Building::~Building() {
	delete &insideWorld;
}


bool Building::buildingOccupies(int x, int y, World* w, bool includeEntrance) {
	if (!buildingsMap.count(w)) {
		return false;
	}
	int info = buildingsMap[w][y][x];
	if (info == OCCUPIED) {
		return true;
	} else if (includeEntrance && info == ENTRANCE) {
		return true;
	}
	return false;
}



bool Building::positionIsEntrance(int x, int y, World* world) {
	if (y == this->y + height - 1 && x == this->x + entranceX && world == outsideWorld) {
		return true;
	}

	return false;
}

bool Building::positionIsExit(int x, int y) {
	if (y == exitY && x == exitX) {
		return true;
	}

	return false;
}


std::map<World*, std::vector<std::vector<int>>> Building::buildingsMap;
