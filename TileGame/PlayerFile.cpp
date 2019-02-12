#include "PlayerFile.h"
#include <string>
#include <fstream>
#include "WorldManager.h"
#include "WorldFile.h"


using namespace tg;

Player* PlayerFile::loadPlayerFile(std::string fileName, Handler* handler) {

	std::ifstream file(handler->saveDirName + "\\" + fileName, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		return nullptr;
	}
	Nullable<int> health;
	Nullable<int> maxHealth;
	Nullable<UUID> id;
	Nullable<float> x;
	Nullable<float> y;
	std::vector<std::pair<int, int>> rawInv;


	health = safeLoad<int>(file);
	maxHealth = safeLoad<int>(file);
	id = safeLoad<UUID>(file);
	x = safeLoad<float>(file);
	y = safeLoad<float>(file);

	for (int i = 0; i < Inventory::INV_SIZE; i++) {
		int first = safeLoad<int>(file);
		int second = safeLoad<int>(file);
		std::pair<int, int> curItem(first, second);
		rawInv.push_back(curItem);
	}

	Inventory* inv = new Inventory(rawInv);


	Nullable<std::string> worldName;
	Nullable<size_t> worldNameSize;

	worldNameSize = safeLoad<size_t>(file);
	worldName.value().resize(worldNameSize);
	file.read((char*)&worldName.value()[0], worldNameSize);

	World* world = handler->worldManager->getWorld(worldName);

	if (world == nullptr) {
		world = WorldFile::loadWorldFile(worldName.value() + ".tgw", handler);
	}

	Nullable<int> stam, maxStam, magic, maxMagic;
	Nullable<bool> regenSlow;

	stam = safeLoad<int>(file);
	maxStam = safeLoad<int>(file);
	magic = safeLoad<int>(file);
	maxMagic = safeLoad<int>(file);
	regenSlow = safeLoad<bool>(file);

	if (stam.getIsNull())
		stam = 100;

	if (maxStam.getIsNull())
		maxStam = 100;

	if (magic.getIsNull())
		magic = 100;

	if (maxMagic.getIsNull())
		maxMagic = 100;

	if (regenSlow.getIsNull())
		regenSlow = false;


	Player* p = new Player(x, y, handler, world);
	p->setHealth(health);
	p->setMaxHealth(maxHealth);
	p->setId(id);
	p->setInventory(inv);
	p->setStamina(stam);
	p->setMaxStamina(maxStam);
	p->setMagic(magic);
	p->setMaxMagic(maxMagic);
	p->setStamIsRegeningSlowly(regenSlow);

	return p;
}



template<typename T> Nullable<T> PlayerFile::safeLoad(std::ifstream& file) {
	T readItem;
	Nullable<T> item;
	file.read((char*)&readItem, sizeof(T));

	if (file.eof()) {
		return item;
	}

	item = readItem;
	return item;
}

PlayerFile::PlayerFile(Player* player, Handler* handler) : handler(handler) {
	this->player = player;
}


PlayerFile::~PlayerFile() {
}

void PlayerFile::saveFile() {

	std::string fileName = "Player.tgp";

	std::ofstream file(handler->saveDirName + "\\" + fileName, std::ios::out | std::ios::trunc | std::ios::binary);

	int health = player->getHealth();
	int maxHealth = player->getMaxHealth();
	UUID id = player->getId();
	float x = player->getX();
	float y = player->getY();
	Inventory* inv = player->getInventory();

	file.write((char*)&health, sizeof(int));
	file.write((char*)&maxHealth, sizeof(int));
	file.write((char*)&id, sizeof(UUID));
	file.write((char*)&x, sizeof(float));
	file.write((char*)&y, sizeof(float));

	for (int i = 0; i < Inventory::INV_SIZE; i++) {
		std::pair<int, int> curItem = inv->getInventory()[i];
		file.write((char*)&curItem.first, sizeof(int));
		file.write((char*)&curItem.second, sizeof(int));
	}

	std::string worldName = player->getWorld()->getNameId();
	size_t worldNameSize = worldName.size();

	file.write((char*)&worldNameSize, sizeof(size_t));
	file.write((char*)&worldName[0], worldNameSize);

	int stam, maxStam, magic, maxMagic;
	bool regenSlow;


	stam = player->getStamina();
	maxStam = player->getMaxStamina();
	magic = player->getMagic();
	maxMagic = player->getMaxMagic();
	regenSlow = player->stamIsRegeningSlowly();

	file.write((char*)&stam, sizeof(int));
	file.write((char*)&maxStam, sizeof(int));
	file.write((char*)&magic, sizeof(int));
	file.write((char*)&maxMagic, sizeof(int));
	file.write((char*)&regenSlow, sizeof(bool));

}
