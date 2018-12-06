#pragma once

#include <vector>
#include <iostream>
#include <SFML/System.hpp>
#include <Windows.h>

class Entity;
class Handler;

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
	void fixEntityMoved(Entity* entity);
	int getNumEntities() { return allEntities.size(); }
	Entity* getEntityById(UUID id);


private:

	std::vector<Entity*> allEntities;
	std::vector<Entity*> renderOrder;
	std::vector<Entity*> renderBuffer;
	std::vector<Entity*> tickList;
	Handler* handler;
	std::vector<Entity*> doNotRender;

	int getRenderStartIndex();
	int getRenderEndIndex();


};

