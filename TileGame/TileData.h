#pragma once

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Handler.h"
#include "Assets.h"
namespace tg {
	class TileData
	{
	public:
		//////////////////
		// surroundingData should be in form of:
		// NW, NE, SE, SW, W, N, E, S
		//////////////////
		TileData(sf::Uint8 baseId, std::vector<sf::Uint8> surroundingData, Handler* handler);
		TileData();

	private:
		sf::Uint8 baseId;
		std::map<sf::Uint8, sf::Uint8> adjacentInfo; // Key is ID of adjacent tile
													 // Value is data of where it surrounds
		Handler* handler;


	public:
		std::map<sf::Uint8, sf::Uint8> getAdjacentInfo() { return adjacentInfo; }

	};

}