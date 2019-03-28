#include "WorldFile.h"
#include "TallGrass.h"
#include "Tree.h"
#include "Item.h"
#include "SandCastle.h"
#include "TreasureChest.h"
#include "Pumpkin.h"
#include "Boat.h"
#include "Zombie.h"
#include "Workbench.h"
#include "Rock.h"
#include "Smelter.h"
#include "Ore.h"
#include "Torch.h"
#include "Fern.h"

using namespace tg;
World* WorldFile::loadWorldFile(std::string fileName, Handler * handler) {
	std::ifstream file(handler->saveDirName + "\\" + fileName, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return nullptr;


	int w; // width
	int h; // height
	int spawnX; //spawn points
	int spawnY;

	file.read((char*)&w, sizeof(int));
	file.read((char*)&h, sizeof(int));
	file.read((char*)&spawnX, sizeof(int));
	file.read((char*)&spawnY, sizeof(int));

	std::stringstream worldData;
	worldData << w << " " << h << "\n";
	worldData << spawnX << " " << spawnY << "\n";

	// All the map data
	for (size_t y = 0; y < h; y++) {
		for (size_t x = 0; x < w; x++) {
			uint8_t curTile;
			file.read((char*)&curTile, sizeof(uint8_t));
			worldData << (int)curTile;
			if (x != w - 1) {
				worldData << " ";
			}
		}
		worldData << "\n";
	}

	// All the entity data

	EntityManager* em = new EntityManager(handler);

	World* world = new World(worldData.str(), handler, fileName.substr(0, fileName.size() - 4), em);

	int numEntities;
	file.read((char*)&numEntities, sizeof(int));
	for (int i = 0; i < numEntities; i++) {

		// Each specific entity

		int health;
		int maxHealth;

		UUID id;
		int type;

		file.read((char*)&type, sizeof(int));
		file.read((char*)&health, sizeof(int));
		file.read((char*)&maxHealth, sizeof(int));
		file.read((char*)&id, sizeof(UUID));

		// Each entity type has specific information along with it

		if (type == TALL_GRASS_E) {
			float x;
			float y;
			int eType;

			file.read((char*)&x, sizeof(float));
			file.read((char*)&y, sizeof(float));
			file.read((char*)&eType, sizeof(int));

			TallGrass* tg = new TallGrass(x, y, handler, eType, world);
			tg->setHealth(health);
			tg->setMaxHealth(maxHealth);
			tg->setId(id);

		} else if (type == TREE_E) {

			int tX;
			int tY;
			int eType;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));
			file.read((char*)&eType, sizeof(int));

			Tree* t = new Tree(tX, tY, handler, eType, world);
			t->setHealth(health);
			t->setMaxHealth(maxHealth);
			t->setId(id);

		} else if (type == ITEM_E) {

			float x;
			float y;
			int itemType;

			file.read((char*)&x, sizeof(float));
			file.read((char*)&y, sizeof(float));
			file.read((char*)&itemType, sizeof(int));

			Item* it = new Item(x, y, handler, itemType, world);
			it->setHealth(health);
			it->setMaxHealth(maxHealth);
			it->setId(id);

		} else if (type == SAND_CASTLE_E) {

			int tX;
			int tY;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));

			SandCastle* sc = new SandCastle(tX, tY, handler, world);
			sc->setHealth(health);
			sc->setMaxHealth(health);
			sc->setId(id);

		} else if (type == TREASURE_CHEST_E) {

			int tX;
			int tY;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));

			std::vector<std::pair<int, int>> contents;
			for (int i = 0; i < TreasureChest::CONTENTS_SIZE; i++) {
				contents.push_back(std::pair<int, int>(-1, -1));
			}

			for (int i = 0; i < TreasureChest::CONTENTS_SIZE; i++) {
				int id, amount, pos;
				file.read((char*)&id, sizeof(int));
				file.read((char*)&amount, sizeof(int));
				file.read((char*)&pos, sizeof(int));
				contents[pos] = std::pair<int, int>(id, amount);
			}

			TreasureChest* tc = new TreasureChest(tX, tY, handler, world);

			for (int i = 0; i < contents.size(); i++) {
				if (contents[i].first != -1)
					tc->setItem(contents[i].first, contents[i].second, i);
			}

			tc->setHealth(health);
			tc->setMaxHealth(maxHealth);
			tc->setId(id);

		} else if (type == PUMPKIN_E) {

			int tX;
			int tY;
			bool isLit;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));
			file.read((char*)&isLit, sizeof(bool));

			Pumpkin* p = new Pumpkin(tX, tY, handler, isLit, world);
			p->setHealth(health);
			p->setMaxHealth(maxHealth);
			p->setId(id);

		} else if (type == BOAT_E) {

			float x;
			float y;
			UUID rId;

			file.read((char*)&x, sizeof(float));
			file.read((char*)&y, sizeof(float));
			file.read((char*)&rId, sizeof(UUID));

			Boat* b = new Boat(x, y, handler, world);
			b->setHealth(health);
			b->setMaxHealth(maxHealth);
			b->setId(id);
			b->setRiderId(rId);

		} else if (type == ZOMBIE_E) {
			float x, y;
			UUID fId;
			file.read((char*)&x, sizeof(float));
			file.read((char*)&y, sizeof(float));
			file.read((char*)&fId, sizeof(UUID));

			Zombie* z = new Zombie(x, y, handler, world);
			z->setFollowingId(fId);
			z->setHealth(health);
			z->setMaxHealth(maxHealth);
			z->setId(id);
		} else if (type == WORKBENCH_E) {

			int tX;
			int tY;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));

			Workbench* w = new Workbench(tX, tY, handler, world);
			w->setHealth(health);
			w->setMaxHealth(maxHealth);
			w->setId(id);

		} else if (type == ROCK_E) {

			int tX;
			int tY;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));

			Rock* r = new Rock(tX, tY, handler, world);
			r->setHealth(health);
			r->setMaxHealth(maxHealth);
			r->setId(id);

		} else if (type == SMELTER_E) {
			int tX;
			int tY;
			bool hasFuel;
			float burnSpeed;

			bool hasSmeltable;
			int smeltingItem;

			double smeltTimer;
			bool smelting;

			bool finished;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));

			file.read((char*)&hasFuel, sizeof(bool));
			file.read((char*)&burnSpeed, sizeof(float));

			file.read((char*)&hasSmeltable, sizeof(bool));
			file.read((char*)&smeltingItem, sizeof(int));

			file.read((char*)&smeltTimer, sizeof(double));
			file.read((char*)&smelting, sizeof(bool));

			file.read((char*)&finished, sizeof(bool));

			Smelter* s = new Smelter(tX, tY, handler, world);
			s->setHasFuel(hasFuel);
			s->setBurnSpeed(burnSpeed);
			s->setHasSmeltable(hasSmeltable);
			s->setSmeltingItem(smeltingItem);
			s->setSmeltTimer(smeltTimer);
			s->setIsSmelting(smelting);
			s->setIsFinished(finished);

		} else if (type == ORE_E) {

			int tX;
			int tY;
			int oType;
			bool regening;
			sf::Int32 cooldown;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));
			file.read((char*)&oType, sizeof(int));
			file.read((char*)&regening, sizeof(bool));
			file.read((char*)&cooldown, sizeof(sf::Int32));

			Ore* o = new Ore(tX, tY, handler, world, oType);
			o->setHealth(health);
			o->setMaxHealth(maxHealth);
			o->setId(id);
			o->setIsRegening(regening);
			o->setCooldown(cooldown);


		} else if (type == TORCH_E) {
			int tX;
			int tY;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));

			Torch* t = new Torch(tX, tY, handler, world);

		} else if (type == FERN_E) {
			int tX;
			int tY;
			sf::Int32 timeAlive;
			float growthPercent;

			file.read((char*)&tX, sizeof(int));
			file.read((char*)&tY, sizeof(int));
			file.read((char*)&timeAlive, sizeof(sf::Int32));
			file.read((char*)&growthPercent, sizeof(float));

			Fern* f = new Fern(tX, tY, handler, world);
			f->setTimeAlive(timeAlive);
			f->setPercentToGrow(growthPercent);

		}
	}

	return world;
}

WorldFile::WorldFile() {
}

WorldFile::WorldFile(World* world, Handler* handler) {
	this->handler = handler;
	this->world = world;
}

WorldFile::~WorldFile() {
}

void WorldFile::saveFile() {

	std::ofstream file(handler->saveDirName + "\\" + world->getNameId() + ".tgw", std::ios::out | std::ios::trunc | std::ios::binary);

	int w = world->getWidth(); // width
	int h = world->getHeight(); // height
	int spawnX = world->getSpawn().x; //spawn points
	int spawnY = world->getSpawn().y;

	file.write((char*)&w, sizeof(int));
	file.write((char*)&h, sizeof(int));
	file.write((char*)&spawnX, sizeof(int));
	file.write((char*)&spawnY, sizeof(int));

	// All the map data
	std::vector<std::vector<sf::Uint8>> map = world->getMap();
	for (size_t y = 0; y < map.size(); y++) {
		for (size_t x = 0; x < map[y].size(); x++) {
			uint8_t curTile = map[y][x];
			file.write((char*)&curTile, sizeof(uint8_t));
		}
	}

	// All the entity data

	EntityManager* em = world->getEntityManager();

	int numEntities = em->getNumEntities();
	std::streampos numEntPos = file.tellp();
	file.write((char*)&numEntities, sizeof(int));

	int totalWrittenEntities = 0;

	for (int i = 0; i < numEntities; i++) {
		Entity* cur = em->getEntity(i);

		// Each specific entity

		int health = cur->getHealth();
		int maxHealth = cur->getMaxHealth();

		UUID id = cur->getId();
		int type = cur->type;

		// Each entity type has specific information along with it

		if (type == TALL_GRASS_E) {

			TallGrass* curTg = dynamic_cast<TallGrass*>(cur);

			float x = curTg->getX();
			float y = curTg->getY();
			int eType = curTg->getEType();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));


			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&eType, sizeof(int));

			totalWrittenEntities++;

		} else if (type == TREE_E) {


			Tree* curT = dynamic_cast<Tree*>(cur);

			int tX = curT->getTX();
			int tY = curT->getTY();
			int eType = curT->getEType();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&eType, sizeof(int));

			totalWrittenEntities++;

		} else if (type == ITEM_E) {

			Item* curI = dynamic_cast<Item*>(cur);

			float x = curI->getX();
			float y = curI->getY();
			int itemType = curI->getItemId();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&itemType, sizeof(int));

			totalWrittenEntities++;

		} else if (type == SAND_CASTLE_E) {

			SandCastle* curSc = dynamic_cast<SandCastle*>(cur);

			int tX = curSc->getTX();
			int tY = curSc->getTY();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));

			totalWrittenEntities++;

		} else if (type == TREASURE_CHEST_E) {

			TreasureChest* curTc = dynamic_cast<TreasureChest*>(cur);


			int tX = curTc->getTX();
			int tY = curTc->getTY();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			std::vector<std::pair<int, int>> contents = curTc->getContents();

			for (int i = 0; i < contents.size(); i++) {
				int id = contents[i].first;
				int amount = contents[i].second;
				file.write((char*)&id, sizeof(int));
				file.write((char*)&amount, sizeof(int));
				file.write((char*)&i, sizeof(int));
			}

			totalWrittenEntities++;

		} else if (type == PUMPKIN_E) {

			Pumpkin* curP = dynamic_cast<Pumpkin*>(cur);

			int tX = curP->getTX();
			int tY = curP->getTY();
			bool isLit = curP->getIsLit();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&isLit, sizeof(bool));

			totalWrittenEntities++;

		} else if (type == BOAT_E) {

			Boat* curB = dynamic_cast<Boat*>(cur);

			float x = curB->getX();
			float y = curB->getY();

			Entity* rider = curB->getRider();

			UUID rId = GUID_NULL;

			if (rider != nullptr)
				rId = rider->getId();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&rId, sizeof(UUID));

			totalWrittenEntities++;

		} else if (type == ZOMBIE_E) {
			Zombie* curZ = dynamic_cast<Zombie*>(cur);
			float x = curZ->getX();
			float y = curZ->getY();

			Entity* following = curZ->getFollowing();
			UUID fId = GUID_NULL;

			if (following != nullptr) {
				fId = following->getId();
			}

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&x, sizeof(float));
			file.write((char*)&y, sizeof(float));
			file.write((char*)&fId, sizeof(UUID));

			totalWrittenEntities++;

		} else if (type == WORKBENCH_E) {

			Workbench* curP = dynamic_cast<Workbench*>(cur);

			int tX = curP->getTX();
			int tY = curP->getTY();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));

			totalWrittenEntities++;

		} else if (type == ROCK_E) {

			Rock* curP = dynamic_cast<Rock*>(cur);

			int tX = curP->getTX();
			int tY = curP->getTY();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));

			totalWrittenEntities++;

		} else if (type == SMELTER_E) {
			Smelter* curP = dynamic_cast<Smelter*>(cur);

			int tX = curP->getTX();
			int tY = curP->getTY();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));

			bool hasFuel = curP->getHasFuel();
			float burnSpeed = curP->getBurnSpeed();

			bool hasSmeltable = curP->getHasSmeltable();
			int smeltingItem = curP->getSmeltingItem();

			double smeltTimer = curP->getSmeltTimer();
			bool smelting = curP->getIsSmelting();

			bool finished = curP->getIsFinished();

			file.write((char*)&hasFuel, sizeof(bool));
			file.write((char*)&burnSpeed, sizeof(float));

			file.write((char*)&hasSmeltable, sizeof(bool));
			file.write((char*)&smeltingItem, sizeof(int));

			file.write((char*)&smeltTimer, sizeof(double));
			file.write((char*)&smelting, sizeof(bool));

			file.write((char*)&finished, sizeof(bool));

			totalWrittenEntities++;

		} else if (type == ORE_E) {

			Ore* curP = dynamic_cast<Ore*>(cur);

			int tX = curP->getTX();
			int tY = curP->getTY();
			int oType = curP->getOreType();
			bool regening = curP->isRegening();
			sf::Int32 cooldown = curP->getCooldown();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&oType, sizeof(int));
			file.write((char*)&regening, sizeof(bool));
			file.write((char*)&cooldown, sizeof(sf::Int32));

			totalWrittenEntities++;
		} else if (type == TORCH_E) {

			Torch* curP = dynamic_cast<Torch*>(cur);

			int tX = curP->getTX();
			int tY = curP->getTY();

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));

			totalWrittenEntities++;
		} else if (type == FERN_E) {

			Fern* curP = dynamic_cast<Fern*>(cur);

			file.write((char*)&type, sizeof(int));
			file.write((char*)&health, sizeof(int));
			file.write((char*)&maxHealth, sizeof(int));
			file.write((char*)&id, sizeof(UUID));

			int tX = curP->getTX();
			int tY = curP->getTY();
			sf::Int32 timeAlive = curP->getTimeAlive();
			float growthPercent = curP->getPercentToGrow();

			file.write((char*)&tX, sizeof(int));
			file.write((char*)&tY, sizeof(int));
			file.write((char*)&timeAlive, sizeof(sf::Int32));
			file.write((char*)&growthPercent, sizeof(float));

			totalWrittenEntities++;

		}
	}

	file.seekp(numEntPos);
	file.write((char*)&totalWrittenEntities, sizeof(int));
}
