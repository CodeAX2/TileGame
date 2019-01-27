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
	int health;
	int maxHealth;
	UUID id;
	float x;
	float y;
	std::map<sf::Uint8, int> inv;


	file.read((char*)&health, sizeof(int));
	file.read((char*)&maxHealth, sizeof(int));
	file.read((char*)&id, sizeof(UUID));
	file.read((char*)&x, sizeof(float));
	file.read((char*)&y, sizeof(float));

	for (int i = 0; i < 256; i++) {
		int amount;
		file.read((char*)&amount, sizeof(int));
		inv[i] = amount;
	}


	std::string worldName;
	size_t worldNameSize;

	file.read((char*)&worldNameSize, sizeof(size_t));
	worldName.resize(worldNameSize);
	file.read((char*)&worldName[0], worldNameSize);

	World* world = handler->worldManager->getWorld(worldName);

	if (world == nullptr) {
		world = WorldFile::loadWorldFile(worldName + ".tgw", handler);
	}

	int stam, maxStam, magic, maxMagic;
	bool regenSlow;

	stam = safeLoad<int>(file);
	std::cout << stam << std::endl;

	file.read((char*)&maxStam, sizeof(int));
	file.read((char*)&magic, sizeof(int));
	file.read((char*)&maxMagic, sizeof(int));
	file.read((char*)&regenSlow, sizeof(bool));

	Player* p = new Player(x, y, handler, world);
	p->setHealth(health);
	p->setMaxHealth(maxHealth);
	p->setId(id);
	p->setInventory(inv);
	p->setStamina(stam);
	p->setMaxStamina(maxStam);
	p->setMagic(magic);
	p->setMaxMagic(magic);
	p->setStamIsRegeningSlowly(regenSlow);

	return p;
}



template<typename T> T PlayerFile::safeLoad(std::ifstream& file) {
	T item;
	file.read((char*)&item, sizeof(T));

	if (file.eof()) {
		return NULL;
	}

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
	std::map<sf::Uint8, int> inv = player->getInventory();

	file.write((char*)&health, sizeof(int));
	file.write((char*)&maxHealth, sizeof(int));
	file.write((char*)&id, sizeof(UUID));
	file.write((char*)&x, sizeof(float));
	file.write((char*)&y, sizeof(float));

	for (int i = 0; i < 256; i++) {
		int amount = inv[i];
		file.write((char*)&amount, sizeof(int));
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

	/*file.write((char*)&stam, sizeof(int));
	file.write((char*)&maxStam, sizeof(int));
	file.write((char*)&magic, sizeof(int));
	file.write((char*)&maxMagic, sizeof(int));
	file.write((char*)&regenSlow, sizeof(bool));*/

}
