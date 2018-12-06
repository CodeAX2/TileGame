#pragma once

#include <vector>
#include "Handler.h"
namespace tg {
	class World;

	class WorldManager
	{
	public:
		WorldManager(Handler* handler);

	public:
		void addWorld(World* world);
		World* getWorld(std::string worldId);
		std::vector<World*> getAllWorlds() { return allWorlds; }


	private:
		Handler * handler;
		std::vector<World*> allWorlds;


	};

}