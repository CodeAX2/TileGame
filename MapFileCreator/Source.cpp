
#define NO_TYPE = -1
#define PLAYER_E 0
#define TREE_E 1
#define TALL_GRASS_E 2
#define ITEM_E 3
#define SAND_CASTLE_E 4
#define TREASURE_CHEST_E 5
#define PUMPKIN_E 6
#define BOAT_E 7
#define BUILDING_E 8
#define ZOMBIE_E 9
#define WORKBENCH_E 10
#define ROCK_E 11
#define SMELTER_E 12
#define ORE_E 13

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "World.h"
#include <time.h>

using namespace std;

const string possibleEntities[] = {

	"TREE_E",
	"TALL_GRASS_E",
	"ITEM_E",
	"SAND_CASTLE_E",
	"TREASURE_CHEST_E",
	"PUMPKIN_E",
	"BOAT_E",
	"ZOMBIE_E",
	"WORKBENCH_E",
	"ROCK_E",
	"SMELTER_E",
	"ORE_E"

};

std::vector<std::vector<bool>> staticMap;

int numEntities = 10;
void fillEntities(std::ofstream& file, World& world);

int main() {

	string fileName;
	std::cout << "Enter a name for the world file: " << flush;
	getline(cin, fileName);

	CreateDirectory("Worlds", NULL);
	std::ofstream file("Worlds\\" + fileName + ".tgw", std::ios::out | std::ios::trunc | std::ios::binary);


	string worldFilePath;
	std::cout << "Enter the world .txt to load from: " << flush;
	cin >> worldFilePath;

	std::ifstream worldTxtFile(worldFilePath, std::ios::in);

	stringstream wTxtSS;
	wTxtSS << worldTxtFile.rdbuf();
	string mapData = wTxtSS.str();

	World world(mapData, fileName);

	int w = world.getWidth(); // width
	int h = world.getHeight(); // height
	int spawnX = world.getSpawnX(); //spawn points
	int spawnY = world.getSpawnY();

	file.write((char*)&w, sizeof(int));
	file.write((char*)&h, sizeof(int));
	file.write((char*)&spawnX, sizeof(int));
	file.write((char*)&spawnY, sizeof(int));

	// All the map data
	std::vector<std::vector<uint8_t>> map = world.getMap();
	for (size_t y = 0; y < map.size(); y++) {
		for (size_t x = 0; x < map[y].size(); x++) {
			uint8_t curTile = map[y][x];
			file.write((char*)&curTile, sizeof(uint8_t));
		}
	}

	// All the entity data

	streampos numEntPos = file.tellp();
	file.write((char*)&numEntities, sizeof(int));

	staticMap.resize(world.getHeight());
	for (int i = 0; i < staticMap.size(); i++) {
		staticMap[i].resize(world.getWidth());
	}

	while (true) {

		string type;
		std::cout << "Enter the type of entity, or EXIT to exit, or FILL to fill the world with an entity: " << flush;
		cin >> type;

		if (type == "EXIT") {
			break;
		} else if (type == "FILL") {
			fillEntities(file, world);
			continue;
		} else if (find(begin(possibleEntities), end(possibleEntities), type) == end(possibleEntities)) {
			std::cout << "Invalid entity type!" << std::endl;
			continue;
		}

		numEntities++;

		// Each specific entity

		int health;
		int maxHealth;

		std::cout << "Enter the health: " << flush;
		cin >> health;

		std::cout << "Enter the maximum health: " << flush;
		cin >> maxHealth;

		UUID id;
		UuidCreate(&id);

		// Each entity type has specific information along with it

		if (type == "TALL_GRASS_E") {

			int type2 = TALL_GRASS_E;
			float x;
			float y;
			int eType;

			std::cout << "Enter the x pos: " << flush;
			cin >> x;

			std::cout << "Enter the y pos: " << flush;
			cin >> y;

			std::cout << "Enter 0 for normal, 1 for snowy: " << flush;
			cin >> eType;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));


			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&eType, sizeof(int));

		} else if (type == "TREE_E") {

			int type2 = TREE_E;
			int tX;
			int tY;
			int eType;

			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			staticMap[tY][tX] = true;

			std::cout << "Enter 0 for normal, 1 for snowy: " << flush;
			cin >> eType;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&eType, sizeof(int));

		} else if (type == "ITEM_E") {

			int type2 = ITEM_E;
			float x;
			float y;
			int itemType;

			std::cout << "Enter the x pos: " << flush;
			cin >> x;

			std::cout << "Enter the y pos: " << flush;
			cin >> y;

			std::cout << "Enter the item id: " << flush;
			cin >> itemType;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&itemType, sizeof(int));

		} else if (type == "SAND_CASTLE_E") {

			int type2 = SAND_CASTLE_E;
			int tX;
			int tY;

			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			staticMap[tY][tX] = true;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));

		} else if (type == "TREASURE_CHEST_E") {

			int type2 = TREASURE_CHEST_E;
			int tX;
			int tY;

			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			staticMap[tY][tX] = true;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			std::vector<std::pair<int, int>> contents;
			for (int i = 0; i < 10; i++) {
				contents.push_back(std::pair<int, int>(-1, -1));
			}

			while (true) {

				int itemType;
				int amount;
				int position;

				std::cout << "Enter the item id, or -1 to exit: " << flush;
				cin >> itemType;

				if (itemType == -1) {
					break;
				}

				std::cout << "Enter the amount: " << flush;
				cin >> amount;

				std::cout << "Enter the position in chest: " << flush;
				cin >> position;

				contents[position] = std::pair<int, int>(itemType, amount);
			}

			for (int i = 0; i < contents.size(); i++) {
				int id = contents[i].first;
				int amount = contents[i].second;
				file.write((char*)&id, sizeof(int));
				file.write((char*)&amount, sizeof(int));
				file.write((char*)&i, sizeof(int));
			}

		} else if (type == "PUMPKIN_E") {

			int type2 = PUMPKIN_E;
			int tX;
			int tY;
			bool isLit;


			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			staticMap[tY][tX] = true;

			std::cout << "is it lit, 0 for false, 1 for true: " << flush;
			cin >> isLit;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&isLit, sizeof(bool));

		} else if (type == "BOAT_E") {


			int type2 = BOAT_E;
			float x;
			float y;

			std::cout << "Enter the x pos: " << flush;
			cin >> x;

			std::cout << "Enter the y pos: " << flush;
			cin >> y;

			UUID rId = GUID_NULL;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&rId, sizeof(UUID));

		} else if (type == "ZOMBIE_E") {


			int type2 = ZOMBIE_E;
			float x;
			float y;

			std::cout << "Enter the x pos: " << flush;
			cin >> x;

			std::cout << "Enter the y pos: " << flush;
			cin >> y;

			UUID fId = GUID_NULL;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&fId, sizeof(UUID));
		} else if (type == "WORKBENCH_E") {


			int type2 = WORKBENCH_E;
			int tX;
			int tY;

			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			staticMap[tY][tX] = true;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
		} else if (type == "ROCK_E") {


			int type2 = ROCK_E;
			int tX;
			int tY;

			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			staticMap[tY][tX] = true;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
		} else if (type == "SMELTER_E") {
			int type2 = SMELTER_E;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			int tX, tY;
			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			staticMap[tY][tX] = true;

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));

			file.write((char*)false, sizeof(bool));
			file.write((char*)0, sizeof(float));

			file.write((char*)false, sizeof(bool));
			file.write((char*)-1, sizeof(int));

			file.write((char*)0, sizeof(double));
			file.write((char*)false, sizeof(bool));

			file.write((char*)false, sizeof(bool));

		} else if (type == "ORE_E") {

			int type2 = ORE_E;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));


			int tX, tY, oType;

			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			std::cout << "Enter the ore type: " << flush;
			cin >> oType;

			staticMap[tY][tX] = true;

			bool regening = false;
			int cooldown = 0;

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&oType, sizeof(int));
			file.write((char*)&regening, sizeof(bool));
			file.write((char*)&cooldown, sizeof(signed int));

		}
	}

	file.seekp(numEntPos);
	file.write((char*)&numEntities, sizeof(int));

	std::cout << "File written!" << endl;
	std::system("PAUSE");

	return 0;
}

void fillEntities(std::ofstream& file, World& world) {

	srand(time(NULL));

	std::string type;
	std::cout << "Enter the entity type: " << flush;
	cin >> type;

	int health;
	int maxHealth;
	double percent;

	std::cout << "Enter the health: " << flush;
	cin >> health;

	std::cout << "Enter the maximum health: " << flush;
	cin >> maxHealth;

	std::cout << "Enter the decimal for percent of entity: " << flush;
	cin >> percent;

	std::cout << percent << endl;

	if (type == "TREE_E") {
		int type2 = TREE_E;
		int eType;

		std::cout << "Enter 0 for normal, 1 for snowy: " << flush;
		cin >> eType;

		for (int i = 0; i < world.getHeight(); i++) {
			for (int j = 0; j < world.getWidth(); j++) {

				if ((world.getMap()[i][j] == 0 && eType == 0) ||
					(world.getMap()[i][j] == 7 && eType == 1)) {

					if ((float)rand() / RAND_MAX > percent) continue;

					if (staticMap[i][j]) continue;
					staticMap[i][j] = true;

					numEntities++;
					UUID id;
					UuidCreate(&id);

					file.write((char*)&type2, sizeof(int));
					file.write((char*)&health, sizeof(int));
					file.write((char*)&maxHealth, sizeof(int));
					file.write((char*)&id, sizeof(UUID));

					file.write((char*)&j, sizeof(int));
					file.write((char*)&i, sizeof(int));
					file.write((char*)&eType, sizeof(int));

				}


			}
		}
	} else if (type == "SAND_CASTLE_E") {
		int type2 = SAND_CASTLE_E;

		for (int i = 0; i < world.getHeight(); i++) {
			for (int j = 0; j < world.getWidth(); j++) {

				if (world.getMap()[i][j] == 3) {

					if ((float)rand() / RAND_MAX > percent) continue;

					if (staticMap[i][j]) continue;
					staticMap[i][j] = true;

					numEntities++;
					UUID id;
					UuidCreate(&id);

					file.write((char*)&type2, sizeof(int));
					file.write((char*)&health, sizeof(int));
					file.write((char*)&maxHealth, sizeof(int));
					file.write((char*)&id, sizeof(UUID));

					file.write((char*)&j, sizeof(int));
					file.write((char*)&i, sizeof(int));

				}
			}
		}


	} else if (type == "PUMPKIN_E") {

		int type2 = PUMPKIN_E;
		bool isLit;

		std::cout << "is it lit, 0 for false, 1 for true: " << flush;
		cin >> isLit;

		for (int i = 0; i < world.getHeight(); i++) {
			for (int j = 0; j < world.getWidth(); j++) {

				if (world.getMap()[i][j] == 0) {

					if ((float)rand() / RAND_MAX > percent) continue;

					if (staticMap[i][j]) continue;
					staticMap[i][j] = true;

					numEntities++;
					UUID id;
					UuidCreate(&id);

					file.write((char*)&type2, sizeof(int));
					file.write((char*)&health, sizeof(int));
					file.write((char*)&maxHealth, sizeof(int));
					file.write((char*)&id, sizeof(UUID));

					file.write((char*)&j, sizeof(int));
					file.write((char*)&i, sizeof(int));
					file.write((char*)&isLit, sizeof(bool));
				}
			}
		}

	} else if (type == "ROCK_E") {

		int type2 = ROCK_E;

		for (int i = 0; i < world.getHeight(); i++) {
			for (int j = 0; j < world.getWidth(); j++) {

				if (world.getMap()[i][j] == 6) {

					if ((float)rand() / RAND_MAX >= percent) continue;

					if (staticMap[i][j]) continue;
					staticMap[i][j] = true;

					numEntities++;
					UUID id;
					UuidCreate(&id);

					file.write((char*)&type2, sizeof(int));
					file.write((char*)&health, sizeof(int));
					file.write((char*)&maxHealth, sizeof(int));
					file.write((char*)&id, sizeof(UUID));

					file.write((char*)&j, sizeof(int));
					file.write((char*)&i, sizeof(int));

				}
			}
		}

	} else if (type == "ORE_E") {
		int type2 = ORE_E;
		int oType;

		std::cout << "Enter the ore type: " << std::flush;
		cin >> oType;

		for (int i = 0; i < world.getHeight(); i++) {
			for (int j = 0; j < world.getWidth(); j++) {

				if (world.getMap()[i][j] == 6) {

					if ((float)rand() / RAND_MAX > percent) continue;

					if (staticMap[i][j]) continue;
					staticMap[i][j] = true;

					numEntities++;
					UUID id;
					UuidCreate(&id);

					bool regening = false;
					int cooldown = 0;

					file.write((char*)&type2, sizeof(int));
					file.write((char*)&health, sizeof(int));
					file.write((char*)&maxHealth, sizeof(int));
					file.write((char*)&id, sizeof(UUID));

					file.write((char*)&j, sizeof(int));
					file.write((char*)&i, sizeof(int));
					file.write((char*)&oType, sizeof(int));
					file.write((char*)&regening, sizeof(bool));
					file.write((char*)&cooldown, sizeof(signed int));

				}
			}
		}
	}

}