#pragma once

#include <vector>
#include "EntityManager.h"
#include "Handler.h"
#include "Assets.h"
#include "World.h"
#include <fstream>
#include "BuildingEntity.h"

#define OPEN 0
#define OCCUPIED 1
#define ENTRANCE 2

namespace tg {
	class Building
	{
	public:
		Building(int x, int y, int width, int height, int entranceX, Handler* handler, World* outsideWorld, World* insideWorld, std::string nameId, int textureToUse = BUILDING_A);
		~Building();

	protected:
		// Map of worlds to an array of tiles, denoting where buildings are located 
		static std::map<World*, std::vector<std::vector<int>>> buildingsMap;

		// List of all buildings in all worlds
		static std::vector<Building*> allBuildings;

	protected:
		int x, y; // x and y are the top left coordinates

		int width, height; // Width and height (in tiles) of the building

		int entranceX; // Relative x position of the entrance. Entrance
		// is always on the bottom row

		Handler* handler; // Game handler

		BuildingEntity* buildingEntity; // Entity that represents the building

		const sf::Texture* buildingTexture; // Texture used for rendering

		EntityManager entityManager; // Entity manager of the interior world

		int exitX = 0, exitY = 0; // Exit tile coordinates within insideWord

		World* insideWorld; // World to be used when player enters building

		World* outsideWorld; // World to be used when player leaves building

		std::string bldgNameId; // String used as an ID

		int textureToUse; // Texture resource id used to obtain actual texture


	public:
		// Check if a building occupies a given tile
		static bool buildingOccupies(int x, int y, World* w, bool includeEntrance = false);

		// Get a list of all the buildings in every world
		static std::vector<Building*> getAllBuildings() {
			return allBuildings;
		}

		// Get a map of worlds to an array of tiles, denoting where buildings are located
		static std::map<World*, std::vector<std::vector<int>>> getBuildingsMap() {
			return buildingsMap;
		}

	public:
		// Check if a tile in a world is an entrance to a building
		bool positionIsEntrance(int x, int y, World* world);

		// Check if a tile in the insideWorld is an exit
		bool positionIsExit(int x, int y);

		// Get the instance of the interior world
		World* getWorld() { return insideWorld; }

		// Get the instance of the exterior world
		World* getOutWorld() { return outsideWorld; }

		// Set the exit tile within the insideWorld
		void setExit(int eX, int eY) {
			exitX = eX; exitY = eY;
		}

		// Return the top left tile x coordinate of the building
		int getX() { return x; }

		// Return the top left tile y coordinate of the building
		int getY() { return y; }

		// Return the exit tile x coordinate within the insideWorld
		int getExitX() { return exitX; }

		// Return the exit tile y coordinate within the insideWorld
		int getExitY() { return exitY; }

		// Return the tile position the player should appear when exiting a building
		sf::Vector2i getOutPos() {
			return sf::Vector2i(this->x + entranceX, this->y + height - 1);
		}

		// Return the width (in tiles) of the building
		int getWidth() { return width; }

		// Return the height (in tiles) of the building
		int getHeight() { return height; }

		// Return the relative x position of the entrance
		// Entrance is always on the bottom row
		int getEntranceX() { return entranceX; }

		// Return the name ID of the building
		std::string getBuildingNameId() { return bldgNameId; }

		// Return the texture resource id used to obtain actual texture
		int getTextureId() { return textureToUse; }

	};

}