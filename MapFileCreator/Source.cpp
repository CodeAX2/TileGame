
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

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "World.h"

using namespace std;

const string possibleEntities[] = {

	"TREE_E",
	"TALL_GRASS_E",
	"ITEM_E",
	"SAND_CASTLE_E",
	"TREASURE_CHEST_E",
	"PUMPKIN_E",
	"BOAT_E"

};

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

	int numEntities = 10;
	streampos numEntPos = file.tellp();
	file.write((char*)&numEntities, sizeof(int));

	while (true) {

		string type;
		std::cout << "Enter the type of entity, or EXIT to exit: " << flush;
		cin >> type;

		if (type == "EXIT") {
			break;
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
			bool defaultContents;

			std::cout << "Enter the tile x pos: " << flush;
			cin >> tX;

			std::cout << "Enter the tile y pos: " << flush;
			cin >> tY;

			std::cout << "Does it have default contents, 0 for false, 1 for true: " << flush;
			cin >> defaultContents;

			file.write((char*)&type2, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&defaultContents, sizeof(bool));
			vector<int> contents(256, 0);

			if (!defaultContents) {



				while (true) {

					int itemType;
					int amount;

					std::cout << "Enter the item id, or -1 to exit: " << flush;
					cin >> itemType;

					if (itemType == -1) {
						break;
					}

					std::cout << "Enter the amount: " << flush;
					cin >> amount;

					contents[itemType] = amount;
				}



			}

			for (int i = 0; i < 256; i++) {
				int amount = contents[i];
				file.write((char*)&amount, sizeof(int));
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

		}
	}

	file.seekp(numEntPos);
	file.write((char*)&numEntities, sizeof(int));

	cout << "File written!" << endl;
	system("PAUSE");

	return 0;
}