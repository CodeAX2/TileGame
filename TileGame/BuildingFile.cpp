#include "BuildingFile.h"
#include <fstream>
#include "Building.h"
#include "WorldManager.h"
#include "WorldFile.h"

using namespace tg;

Building* BuildingFile::loadBuildingFile(std::string fileName, Handler* handler) {

	std::ifstream file(handler->saveDirName + "\\" + fileName, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		return nullptr;
	}


	int x;
	int y;

	int w;
	int h;

	int entranceX;

	int exitX;
	int exitY;

	std::string outsideWorldName;
	size_t outWorldNameSize;

	std::string insideWorldName;
	size_t inWorldNameSize;

	int textureToUse;

	file.read((char*)&x, sizeof(int));
	file.read((char*)&y, sizeof(int));

	file.read((char*)&w, sizeof(int));
	file.read((char*)&h, sizeof(int));

	file.read((char*)&entranceX, sizeof(int));

	file.read((char*)&exitX, sizeof(int));
	file.read((char*)&exitY, sizeof(int));

	file.read((char*)&outWorldNameSize, sizeof(size_t));
	outsideWorldName.resize(outWorldNameSize);
	file.read((char*)&outsideWorldName[0], outWorldNameSize);

	file.read((char*)&inWorldNameSize, sizeof(size_t));
	insideWorldName.resize(inWorldNameSize);
	file.read((char*)&insideWorldName[0], inWorldNameSize);

	file.read((char*)&textureToUse, sizeof(int));

	World* outsideWorld = handler->worldManager->getWorld(outsideWorldName);
	World* insideWorld = handler->worldManager->getWorld(insideWorldName);

	if (outsideWorld == nullptr) {
		outsideWorld = WorldFile::loadWorldFile(outsideWorldName + ".tgw", handler);
	}

	if (insideWorld == nullptr) {
		insideWorld = WorldFile::loadWorldFile(insideWorldName + ".tgw", handler);
	}

	Building* bldg = new Building(x, y, w, h, entranceX, handler, outsideWorld, insideWorld, fileName.substr(0, fileName.size() - 4),textureToUse);
	bldg->setExit(exitX, exitY);

	return bldg;
}

BuildingFile::BuildingFile(Building* building, std::string fileName, Handler* handler) : building(building), fileName(fileName), handler(handler) {
}


BuildingFile::~BuildingFile()
{
}

void BuildingFile::saveFile() {

	std::ofstream file(handler->saveDirName + "\\" + fileName, std::ios::out | std::ios::trunc | std::ios::binary);

	int x = building->getX();
	int y = building->getY();

	int w = building->getWidth();
	int h = building->getHeight();

	int entranceX = building->getEntranceX();

	int exitX = building->getExitX();
	int exitY = building->getExitY();

	std::string outsideWorldName = building->getOutWorld()->getNameId();
	size_t outWorldNameSize = outsideWorldName.size();

	std::string insideWorldName = building->getWorld()->getNameId();
	size_t inWorldNameSize = insideWorldName.size();

	int textureToUse = building->getTextureId();

	file.write((char*)&x, sizeof(int));
	file.write((char*)&y, sizeof(int));

	file.write((char*)&w, sizeof(int));
	file.write((char*)&h, sizeof(int));

	file.write((char*)&entranceX, sizeof(int));

	file.write((char*)&exitX, sizeof(int));
	file.write((char*)&exitY, sizeof(int));

	file.write((char*)&outWorldNameSize, sizeof(size_t));
	file.write((char*)&outsideWorldName[0], outWorldNameSize);

	file.write((char*)&inWorldNameSize, sizeof(size_t));
	file.write((char*)&insideWorldName[0], inWorldNameSize);

	file.write((char*)&textureToUse, sizeof(int));

}
  