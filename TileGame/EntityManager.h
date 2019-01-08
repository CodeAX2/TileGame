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
		void removeEntity(Entity* entity);
		void fixEntityMoved(Entity* entity, int prevX, int prevY);
		int getNumEntities() { return allEntities.size(); }
		Entity* getEntityById(UUID id);
		void setWorld(World* world);
		std::vector<Entity*> getEntitiesAtTile(int x, int y);


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


	};

}