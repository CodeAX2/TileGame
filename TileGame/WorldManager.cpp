#include "WorldManager.h"
#include "World.h"


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
