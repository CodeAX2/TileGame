#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class World
{
private:
	std::vector<std::vector<uint8_t>> map;
	int w, h, spawnX, spawnY;
	int highlightedX = -1, highlightedY = -1;
	std::string nameId;


public:
	World(std::string loadFrom, std::string name);
	World();
	~World();

private:
	void readWorldFile(std::string data);

public:
	int getWidth() { return w; }
	int getHeight() { return h; }

	int getSpawnX() { return spawnX; }
	int getSpawnY() { return spawnY; }

	std::string getNameId() { return nameId; }

	std::vector<std::vector<uint8_t>> getMap() { return map; }

};

