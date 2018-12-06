#pragma once

#include "World.h"
#include <string>
#include "Entity.h"

class WorldFile
{

public:
	static World* loadWorldFile(std::string fileName, Handler* handler);

public:
	WorldFile();
	WorldFile(World* world, Handler* handler);
	~WorldFile();

	void saveFile();

private:
	World* world;
	Handler* handler;

};

