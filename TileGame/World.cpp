#include "World.h"
#include "WorldManager.h"
using namespace tg;
World::World(std::string loadFrom, Handler* handler, std::string name, EntityManager* em) : handler(handler), em(em), nameId(name) {

	if (this->em == nullptr) {
		this->em = handler->entityManager;
	}

	readWorldFile(loadFrom);
	loadTileData();


	em->setWorld(this);
	handler->worldManager->addWorld(this);

}

World::World() {

}

World::~World() {
	delete em;
}

// Return a tile at x, y, where both are in terms of the map (i.e. divided by tile size)
sf::Uint8 World::getTile(int x, int y) {

	if (x >= 0 && x < w && y >= 0 && y < h) {
		return map[y][x];
	} else {
		return 2;
	}

}

// Set a tile at x and y to a different tile
// x and y are in terms of the map
void World::setTile(int x, int y, sf::Uint8 newId) {
	if (x >= 0 && x < w && y >= 0 && y < h) {
		// Sets the new tile id
		map[y][x] = newId;

		// Update all surrounding tiles
		for (int tX = x - 1; tX <= x + 1; tX++) {
			for (int tY = y - 1; tY <= y + 1; tY++) {

				sf::Uint8 baseId = getTile(tX, tY);
				std::vector<sf::Uint8> adjData;


				adjData.push_back(getTile(tX - 1, tY - 1));
				adjData.push_back(getTile(tX + 1, tY - 1));
				adjData.push_back(getTile(tX + 1, tY + 1));
				adjData.push_back(getTile(tX - 1, tY + 1));
				adjData.push_back(getTile(tX - 1, tY));
				adjData.push_back(getTile(tX, tY - 1));
				adjData.push_back(getTile(tX + 1, tY));
				adjData.push_back(getTile(tX, tY + 1));

				if (tX >= 0 && tX < w && tY >= 0 && tY < h) {
					fullMap[tY][tX] = TileData(baseId, adjData, handler);
				}


			}
		}
	}




}

// Read the world data from a string containing the contents of a file
void World::readWorldFile(std::string data) {

	std::string line;
	std::vector<int> worldInfo;

	std::istringstream iss(data);

	while (std::getline(iss, line)) {
		// Loop over every line and gather info
		std::istringstream ln(line);
		std::string cur;

		while (std::getline(ln, cur, ' ')) {

			if (cur.c_str()[0] != '\r' && cur.c_str()[0] != NULL) { // Make sure it isnt a new line 
																	// or the string terminator
				try {
					int curInt = std::stoi(cur, nullptr);
					worldInfo.push_back(curInt); // Add the info to an array of all the data

				} catch (std::invalid_argument) {
					std::cout << "Could not read int: " << (int)cur.c_str()[0] << std::endl; // Print out the ascii id
				}
			}


		}
	}

	// Set all the basic world info
	w = worldInfo[0];
	h = worldInfo[1];
	spawnX = worldInfo[2];
	spawnY = worldInfo[3];

	// Set all the map data
	for (unsigned int y = 0; y < h; y++) {

		std::vector <sf::Uint8> rowInfo(w, 2);

		for (unsigned int x = 0; x < w; x++) {
			rowInfo[x] = (worldInfo[x + y * w + 4]);
		}

		// Finally, add the current row to the map
		map.push_back(rowInfo);

	}

}

// Check if a tile is solid. x and y are in terms of the renderer, meaning
// not divided by tileSize
bool World::tileIsSolid(int x, int y) {

	// Divide them here :P
	x /= 96;
	y /= 96;

	if (x < 0 || x >= w || y < 0 || y >= h) {
		return true;
	}

	int tileId = map[y][x];
	if (tileId == 1 || tileId == 2 || (tileId >= 8 && tileId <= 18)) {
		return true;
	}


	return false;
}

bool World::tileIsWater(int x, int y) {

	// Divide them here :P
	x /= 96;
	y /= 96;

	if (x < 0 || x >= w || y < 0 || y >= h) {
		return false;
	}

	int tileId = map[y][x];
	if (tileId == 2) {
		return true;
	}


	return false;
}

// Load all the data about surrounding tiles
// so that we don't have to get it on every frame
void World::loadTileData() {


	for (int y = 0; y < h; y++) {

		std::vector<TileData> curLine;

		for (int x = 0; x < w; x++) {
			sf::Uint8 baseId = map[y][x];
			std::vector<sf::Uint8> adjData;


			adjData.push_back(getTile(x - 1, y - 1));
			adjData.push_back(getTile(x + 1, y - 1));
			adjData.push_back(getTile(x + 1, y + 1));
			adjData.push_back(getTile(x - 1, y + 1));
			adjData.push_back(getTile(x - 1, y));
			adjData.push_back(getTile(x, y - 1));
			adjData.push_back(getTile(x + 1, y));
			adjData.push_back(getTile(x, y + 1));

			curLine.push_back(TileData(baseId, adjData, handler));


		}

		fullMap.push_back(curLine);

	}

}

// Returns the tile data of a certain tile. x and y in terms of the map
TileData World::getTileData(int x, int y) {

	if (x >= 0 && x < w && y >= 0 && y < h) {

		return fullMap[y][x];


	} else {
		return TileData(0,std::vector<sf::Uint8>(),nullptr);
	}


}

float World::getTileDarknessPercent() {

	// Have an array of every tile darkness percent
	// Update it whenever a new entity is created, or when
	// an entity moves (both only if the entity emits light or blocks it)

}
