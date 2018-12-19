#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <vector>
#include <sstream>
#include "Handler.h"
#include "TileData.h"

namespace tg {
	class World
	{
	private:
		std::vector<std::vector<sf::Uint8>> map;
		int w, h, spawnX, spawnY;
		Handler* handler;
		std::vector<std::vector<TileData>> fullMap;
		int highlightedX = -1, highlightedY = -1;
		bool highlightGood = true;
		EntityManager* em;
		std::string nameId;


	public:
		World(std::string loadFrom, Handler* handler, std::string name, EntityManager* em = nullptr);
		World();
		~World();

	private:
		void readWorldFile(std::string data);
		void loadTileData();

	public:
		int getWidth() { return w; }
		int getHeight() { return h; }
		sf::Uint8 getTile(int x, int y);
		void setTile(int x, int y, sf::Uint8 newId);
		sf::Vector2f getSpawn() {
			return sf::Vector2f((float)spawnX, (float)spawnY);
		}

		sf::Vector2i getSpawnInt() {
			return sf::Vector2i(spawnX, spawnY);
		}

		bool tileIsSolid(int x, int y);
		bool tileIsWater(int x, int y);
		TileData getTileData(int x, int y);
		void setHighlightedTile(int x, int y) {
			highlightedX = x;
			highlightedY = y;
		}

		sf::Vector2i getHighlightedTile() {
			return sf::Vector2i(highlightedX, highlightedY);
		}

		void setHighlightGood(bool h) { highlightGood = h; }
		bool highlightIsGood() { return highlightGood; }

		EntityManager* getEntityManager() { return em; }
		std::string getNameId() { return nameId; }

		std::vector<std::vector<sf::Uint8>> getMap() { return map; }

	};

}