#pragma once
#pragma once

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <SFML/Audio.hpp>

class Assets
{
private:

	// Collected Textures
	const sf::Texture* allTiles[256][256][3];

	//Tile Priorities
	int renderPriority[256];

public:
	Assets();
	~Assets();

private:
	// Tile/Item Functions
	void setTileInfo(int id, const sf::Texture* tile, int priority);
	void addOuterTexture(int id, const sf::Texture* text, sf::Uint8 position);
	void loadFull(std::string fileLocation, int bId, int priority);

public:

	// Loading Functions
	void init();

	// Tile Getters
	const sf::Texture* getOuterTexture(sf::Uint8 baseTileId, sf::Uint8 outsideData, int x, int y, bool random = true);
	const sf::Texture* getBaseTexture(sf::Uint8 baseTileId) { return allTiles[baseTileId][0][0]; }
	int getTilePriority(sf::Uint8 id);
	bool priorityHasTile(int priority);
	sf::Uint8 getTileWithPriority(int priority) { return renderPriority[priority]; }

};
