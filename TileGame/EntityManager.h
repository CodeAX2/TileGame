#pragma once

#include <vector>
#include <iostream>
#include <SFML/System.hpp>
#include <Windows.h>

namespace tg {
	class Entity;
	class Handler;
	class World;

	class EntityManager
	{

	public:

		EntityManager(Handler* handler);

		Entity* getEntity(int index);
		void addEntity(Entity* entity);
		int numEntities();
		void deleteEntities();
		void render();
		void updateRenderOrder(Entity* entity);
		void tick(sf::Int32 dt);
		void removeEntity(Entity* entity, bool deleteAfter);
		void fixEntityMoved(Entity* entity, int prevX, int prevY);
		int getNumEntities() { return allEntities.size(); }
		Entity* getEntityById(UUID id);
		void setWorld(World* world);
		std::vector<Entity*> getEntitiesAtTile(int x, int y);
		std::vector<Entity*> getAllEntities() { return allEntities; }

		void addTickAnywhereEntity(Entity* entity) { tickAnywhereList.push_back(entity); }
		void tickExtras(sf::Int32 dt);


	private:

		std::vector<Entity*> allEntities;
		std::vector<Entity*> renderOrder;
		std::vector<Entity*> renderBuffer;
		std::vector<Entity*> tickList;
		Handler* handler;
		std::vector<Entity*> doNotRender;

		int getRenderStartIndex();
		int getRenderEndIndex();

		World* world = nullptr;

		std::vector<std::vector<std::vector<Entity*>>> entityTileMap; // Format of [y][x]

		std::vector<Entity*> tickAnywhereList; // For the entities that need to be ticked, without regard
													  // to the current active world
		

	};

}