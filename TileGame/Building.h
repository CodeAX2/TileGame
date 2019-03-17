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
		static std::map<World*, std::vector<std::vector<int>>> buildingsMap;
		static std::vector<Building*> allBuildings;

	protected:
		int x, y; // x and y are the top left coordinates
		int width, height;
		int entranceX; // Relative x position of the entrance. Entrance
		// is always on the bottom row
		EntityManager entityManager;
		Handler* handler;
		BuildingEntity* buildingEntity;
		const sf::Texture* buildingTexture;
		int exitX = 0, exitY = 0;
		World* insideWorld;
		World* outsideWorld;
		std::string bldgNameId;
		int textureToUse;


	public:
		static bool buildingOccupies(int x, int y, World* w, bool includeEntrance = false);
		static std::vector<Building*> getAllBuildings() {
			return allBuildings;
		}
	public:
		bool positionIsEntrance(int x, int y, World* world);
		bool positionIsExit(int x, int y);
		World* getWorld() {
			return insideWorld;
		}

		World* getOutWorld() { return outsideWorld; }

		void setExit(int eX, int eY) {
			exitX = eX; exitY = eY;
		}


		int getX() { return x; }
		int getY() { return y; }
		int getExitX() { return exitX; }
		int getExitY() { return exitY; }

		sf::Vector2i getOutPos() {
			return sf::Vector2i(this->x + entranceX, this->y + height - 1);
		}

		int getWidth() { return width; }
		int getHeight() { return height; }
		int getEntranceX() { return entranceX; }

		std::string getBuildingNameId() { return bldgNameId; }

		int getTextureId() { return textureToUse; }

	};

}