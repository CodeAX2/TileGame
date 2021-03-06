#include "Loader.h"
#include "Tree.h"
#include "TallGrass.h"
#include "InputManager.h"
#include "Player.h"
#include "SandCastle.h"
#include "TreasureChest.h"
#include "SFML/Graphics.hpp"
#include "Pumpkin.h"
#include "Animation.h"
#include "Boat.h"
#include "Building.h"
#include <fstream>
#include "WorldFile.h"
#include "PlayerFile.h"
#include "WorldManager.h"
#include "BuildingFile.h"
#include <filesystem>
#include "Zombie.h"
#include <random>
#include "Workbench.h"
#include "Rock.h"
#include "Smelter.h"
#include "Ore.h"
#include "PalmTree.h"
#include "Villager.h"

using namespace tg;
namespace fs = std::filesystem;

Loader::Loader(Handler* handler) : loadThread(&Loader::loadEntities, this), handler(handler) {


}


Loader::~Loader() {
}


// Launch the loading thread
void Loader::beginLoading() {


	loadThread.launch();

}

// Close down the loading thread
void Loader::exit() {


	loadThread.terminate();

}

// The main loading thread
// Loads all the textures, entities, etc.
// TODO: Rename
void Loader::loadEntities() {
	srand(time(NULL));

	{int randomOffset = rand(); }

	loadingMessage = "Loading assets...";
	handler->assets->init();
	handler->entityManager = new EntityManager(handler);
	handler->worldManager = new WorldManager(handler);

	loadingMessage = "Loading world...";

	World* mainWorld = WorldFile::loadWorldFile("Main World.tgw", handler);
	handler->mainWorld = mainWorld;

	bool worldWasNull = false;

	if (mainWorld == nullptr) {
		handler->mainWorld = new World(handler->assets->getMapData(), handler, "Main World", handler->entityManager);
		worldWasNull = true;
		mainWorld = handler->mainWorld;
		mainWorld->setDarknessPercent(-1);
	}

	handler->entityManager->setWorld(mainWorld);


	handler->camera = new Camera(handler);

	loadingMessage = "Loading player...";

	Player* player = PlayerFile::loadPlayerFile("Player.tgp", handler);
	handler->player = player;

	if (handler->player == nullptr) {
		handler->player = new Player(handler->mainWorld->getSpawn().x * 96,
			handler->mainWorld->getSpawn().y * 96 - handler->assets->getPlayerAnim()->getFrame(0)->getSize().y,
			handler, handler->mainWorld);
	}

	handler->camera->centerOnEntity(handler->player);
	//handler->camera->centerAt(48 * 96, 24 * 96, handler->mainWorld);

	srand(time(NULL) * rand());

	loadingMessage = "Loading buildings...";

	std::vector<std::string> buildingFiles;

	for (auto& p : fs::directory_iterator(handler->saveDirName)) {
		std::string curFileName = p.path().filename().generic_string();
		if (curFileName.substr(curFileName.size() - 4, curFileName.size()) == ".tgb") {
			buildingFiles.push_back(curFileName.substr(0, curFileName.size() - 4));
		}
	}

	for (std::string s : buildingFiles) {
		Building* bldg = BuildingFile::loadBuildingFile(s + ".tgb", handler);
	}

	World* world = handler->mainWorld;

	new Villager(50 * 96, 24 * 96, handler, world, "path.tgh");

	if (worldWasNull) {


		new Boat(106 * 96, 56 * 96 - 2, handler, world);
		new Boat(193 * 96, 33 * 96 - 2, handler, world);
		new Boat(60 * 96, 24 * 96 - 2, handler, world);

		loadingMessage = "Loading treasure chests...";
		TreasureChest* chest = new TreasureChest(51, 45, handler, world);
		chest->setItem(5, 10, 0);
		chest->setItem(7, 1, 7);

		(new TreasureChest(83, 53, handler, world))->setItem(5, 10, 2);
		(new TreasureChest(146, 37, handler, world))->setItem(2, 1, 2);
		(new TreasureChest(212, 90, handler, world))->setItem(2, 1, 2);
		(new TreasureChest(107, 189, handler, world))->setItem(5, 10, 2);
		(new TreasureChest(103, 186, handler, world))->setItem(0, 10, 2);
		(new TreasureChest(102, 183, handler, world))->setItem(0, 10, 2);
		(new TreasureChest(109, 193, handler, world))->setItem(0, 10, 2);
		(new TreasureChest(111, 197, handler, world))->setItem(0, 10, 2);
		(new TreasureChest(135, 160, handler, world))->setItem(1, 5, 2);
		(new TreasureChest(120, 232, handler, world))->setItem(1, 5, 2);
		(new TreasureChest(201, 224, handler, world))->setItem(1, 5, 2);
		(new TreasureChest(207, 226, handler, world))->setItem(1, 5, 2);
		(new TreasureChest(202, 222, handler, world))->setItem(1, 5, 2);

		loadingMessage = "Loading trees...";

		for (int i = 0; i < 120000; i++) {

			int x = std::random_device{}() % world->getWidth(), y = std::random_device{}() % world->getHeight();

			if (x == world->getSpawn().x && y == world->getSpawn().y) {
				continue;
			}

			if (world->getTile(x, y) == 0) {

				new Tree(x, y, handler, T_NORMAL, world);


			}


		}


		for (int i = 0; i < 30000; i++) {

			int x = std::random_device{}() % world->getWidth(), y = std::random_device{}() % world->getHeight();

			if (x == world->getSpawn().x && y == world->getSpawn().y) {
				continue;
			}

			if (world->getTile(x, y) == 7) {

				new Tree(x, y, handler, T_SNOWY, world);

			}


		}

		for (int i = 0; i < 30000; i++) {

			int x = std::random_device{}() % world->getWidth(), y = std::random_device{}() % world->getHeight();

			if (x == world->getSpawn().x && y == world->getSpawn().y) {
				continue;
			}

			if (world->getTile(x, y) == 3) {

				new PalmTree(x, y, handler, world);

			}


		}

		loadingMessage = "Loading rocks...";

		for (int i = 0; i < 60000; i++) {

			int x = std::random_device{}() % world->getWidth(), y = std::random_device{}() % world->getHeight();

			if (x == world->getSpawn().x && y == world->getSpawn().y) {
				continue;
			}

			if (world->getTile(x, y) == 6) {

				new Rock(x, y, handler, world);


			}


		}

		loadingMessage = "Loading spooky scary skeletons...";
		for (int i = 0; i < 12000; i++) {

			int x = std::random_device{}() % world->getWidth(), y = std::random_device{}() % world->getHeight();

			if (x == world->getSpawn().x && y == world->getSpawn().y) {
				continue;
			}

			if (world->getTile(x, y) == 0) {

				new Pumpkin(x, y, handler, (rand() % 3) % 2, world);


			}


		}


		loadingMessage = "Loading grass...";

		int grassWidth = handler->assets->getTallGrassTexture()->getSize().x * 3;
		int grassHeight = handler->assets->getTallGrassTexture()->getSize().y * 3;

		for (int i = 0; i < 120000; i++) {
			float x = std::random_device{}() % (world->getWidth() * 96), y = std::random_device{}() % (world->getHeight() * 96);

			if (world->getTile((x + grassWidth) / 96, (y + grassHeight) / 96) == 0 &&
				world->getTile(x / 96, (y + grassHeight) / 96) == 0 &&
				world->getTile((x + grassWidth) / 96, (y + grassHeight - 10) / 96) == 0 &&
				world->getTile(x / 96, (y + grassHeight - 10) / 96) == 0) {

				new TallGrass(x, y, handler, TG_NORMAL, world);


			}




		}

		for (int i = 0; i < 30000; i++) {
			float x = std::random_device{}() % (world->getWidth() * 96), y = std::random_device{}() % (world->getHeight() * 96);

			if (world->getTile((x + grassWidth) / 96, (y + grassHeight) / 96) == 7 &&
				world->getTile(x / 96, (y + grassHeight) / 96) == 7 &&
				world->getTile((x + grassWidth) / 96, (y + grassHeight - 10) / 96) == 7 &&
				world->getTile(x / 96, (y + grassHeight - 10) / 96) == 7) {

				new TallGrass(x, y, handler, TG_SNOWY, world);


			}




		}

		loadingMessage = "Loading sand castles...";

		for (int i = 0; i < 8000; i++) {

			int x = rand() % (world->getWidth()), y = rand() % (world->getHeight());
			if (x == world->getSpawn().x && y == world->getSpawn().y) {
				continue;
			}
			if (world->getTile(x, y) == 3 &&
				world->getTile(x - 1, y) == 3 &&
				world->getTile(x + 1, y) == 3) {

				new SandCastle(x, y, handler, world);


			}


		}
	}

	loadingMessage = "Finished!";
	finished = true;



}