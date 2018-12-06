#include "World.h"

World::World(std::string loadFrom, std::string name) : nameId(name) {

	readWorldFile(loadFrom);

}

World::World() {

}

World::~World() {
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

		std::vector <uint8_t> rowInfo(w, 2);

		for (unsigned int x = 0; x < w; x++) {
			rowInfo[x] = (worldInfo[x + y * w + 4]);
		}

		// Finally, add the current row to the map
		map.push_back(rowInfo);

	}

}