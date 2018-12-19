#include "WorldManager.h"
#include "World.h"
using namespace tg;

WorldManager::WorldManager(Handler* handler) : handler(handler){
}


void WorldManager::addWorld(World* world) {

	if (getWorld(world->getNameId()) == nullptr) { // The world doesn't already exist
		allWorlds.push_back(world);
	}

}

World* WorldManager::getWorld(std::string worldId) {

	for (World* world : allWorlds) {
		std::string curId = world->getNameId();
		if (worldId == curId) {
			return world;
		}
	}

	return nullptr;
}


World* WorldManager::getWorldFromEntitymanager(EntityManager* em) {
	for (World* world : allWorlds) {
		if (world->getEntityManager() == em) {
			return world;
		}
	}

	return nullptr;

}