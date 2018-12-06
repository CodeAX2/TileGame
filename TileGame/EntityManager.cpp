#include "EntityManager.h"
#include "Entity.h"
#include "Handler.h"
#include "Player.h"
#include <iostream>
#include <sstream>

using namespace tg;

EntityManager::EntityManager(Handler* handler) {

	this->handler = handler;

}

Entity* EntityManager::getEntity(int index) {
	return allEntities[index];
}

void EntityManager::addEntity(Entity* entity) {


	if (entity->type == PLAYER_E) {
		return;
	}

	allEntities.push_back(entity);
	updateRenderOrder(entity);

	if (entity->needsTicking) {
		tickList.push_back(entity);
	}


}

int EntityManager::numEntities() {
	return allEntities.size();
}

void EntityManager::deleteEntities() {
	for (int i = 0; i < allEntities.size(); i++) {
		delete allEntities[i];
	}

}

void EntityManager::removeEntity(Entity* entity) {

	doNotRender.push_back(entity);

	for (int i = 0; i < renderOrder.size(); i++) {
		if (renderOrder[i] == entity) {
			renderOrder.erase(renderOrder.begin() + i);
			break;
		}
	}

	for (int i = 0; i < allEntities.size(); i++) {
		if (allEntities[i] == entity) {
			allEntities.erase(allEntities.begin() + i);
			break;
		}
	}

	if (entity->needsTicking) {
		for (int i = 0; i < tickList.size(); i++) {
			if (tickList[i] == entity) {
				tickList.erase(tickList.begin() + i);
				break;
			}
		}
	}

	delete entity;

}

void EntityManager::fixEntityMoved(Entity* entity) {
	// The entity moved, so we need to remove it from the render order, and add it back in

	std::vector<Entity*> curRO = renderOrder;

	for (int i = 0; i < curRO.size(); i++) {
		Entity* cur = curRO[i];
		if (cur == nullptr) {
			return;
		}
		if (cur == entity) {
			curRO.erase(curRO.begin() + i);
			break;
		}
	}

	bool isAdded = false;
	for (int i = 0; i < curRO.size(); i++) {

		Entity* cur = curRO[i];
		if (cur == nullptr) continue;

		float curY = cur->getY() + (float)cur->getHeight();

		if (entity->getY() + (float)entity->getHeight() <= curY) {


			curRO.insert(curRO.begin() + i, entity);
			isAdded = true;
			break;
		}

	}

	if (!isAdded) {
		curRO.push_back(entity);
	}

	renderOrder = curRO;

}

Entity* EntityManager::getEntityById(UUID id) {
	if (handler->player->getId() == id) {
		return handler->player;
	}

	for (int i = 0; i < allEntities.size(); i++) {
		Entity* cur = allEntities[i];
		if (cur->getId() == id) {
			return cur;
		}
	}

	return nullptr;

}

void EntityManager::render() {

	for (int i = doNotRender.size() - 1; i >= 0; i--) {
		Entity* cur = doNotRender[i];
		if (std::find(renderOrder.begin(), renderOrder.end(),cur) == renderOrder.end()) {
			doNotRender.erase(doNotRender.begin() + i);
		}
	}

	renderBuffer = renderOrder;

	bool playerIsRendered = false;
	int endIndex = renderBuffer.size() - 1;

	for (int i = getRenderStartIndex(); i <= endIndex; i++) {

		Entity* cur = renderBuffer[i];
		if (cur == nullptr) {
			continue;
		}

		if (std::find(doNotRender.begin(), doNotRender.end(), cur) != doNotRender.end()) {
			continue;
		}

		sf::View v = handler->window->getView();
		sf::Vector2u s = handler->window->getSize();

		if (!(cur->getX() + cur->getWidth() < handler->camera->getXOffset() + (1280/2 - v.getSize().x/2) ||
			cur->getX() > handler->window->getSize().x + handler->camera->getXOffset() - (1280/2 - v.getSize().x/2) ||
			cur->getY() + cur->getHeight() < handler->camera->getYOffset() + (720/2 - v.getSize().y/2) ||
			cur->getY() > handler->window->getSize().y + handler->camera->getYOffset() - (720/2 - v.getSize().y/2))) {

			if (handler->player->getY() + handler->player->getHeight() <= cur->getY() + cur->getHeight() && !playerIsRendered) {
				if (handler->player->getRidingOn() == nullptr) {
					handler->player->render(handler);
					playerIsRendered = true;
				}
			}

			cur->render(handler);
		}

	}

	if (!playerIsRendered && handler->player->getRidingOn() == nullptr) {
		handler->player->render(handler);
	}

}

void EntityManager::tick(sf::Int32 dt) {

	for (int i = 0; i < tickList.size(); i++) {

		Entity* cur = tickList[i];

		if (std::find(doNotRender.begin(), doNotRender.end(), cur) != doNotRender.end()) {
			continue;
		}

		if (cur != nullptr) {
			cur->tick(dt);
		}
	}
}

void EntityManager::updateRenderOrder(Entity* newEntity) {


	for (int i = 0; i < renderOrder.size(); i++) {
		if (renderOrder[i] == nullptr) continue;
		if (newEntity->equals(*renderOrder[i])) {
			// An equal entity already exists, so don't add this one
			allEntities.pop_back();
			return;
		}
	}

	bool isAdded = false;
	for (int i = 0; i < renderOrder.size(); i++) {

		Entity* cur = renderOrder[i];
		if (cur == nullptr) continue;

		float curY = cur->getY() + (float)cur->getHeight();

		if (newEntity->getY() + (float)newEntity->getHeight() <= curY) {


			renderOrder.insert(renderOrder.begin() + i, newEntity);
			isAdded = true;
			break;
		}

	}

	if (!isAdded) {
		renderOrder.push_back(newEntity);
	}


}

int EntityManager::getRenderStartIndex() {
	// Looking for the first item to be rendered
	int left = 0;
	int right = renderOrder.size() - 1;
	sf::View v = handler->window->getView();
	while (left < right) {
		int mIndex = (left + right) / 2;
		Entity* cur = renderOrder[mIndex];
		if (mIndex + 1 == renderOrder.size()) {
			return mIndex;
		}
		Entity* nextCur = renderOrder[mIndex + 1];
		if (cur->getY() + cur->getHeight() < handler->camera->getYOffset() + (720/2 - v.getSize().y/2)) {
			if (nextCur->getY() + nextCur->getHeight() < handler->camera->getYOffset() + (720/2 - v.getSize().y/2)) {
				left = mIndex + 1;
			} else {
				return mIndex;
			}

		} else {
			right = mIndex - 1;
		}
	}
	return 0;
}

int EntityManager::getRenderEndIndex() {
	// Looking for the last item to be rendered
	int left = 0;
	int right = renderOrder.size() - 1;
	sf::View v = handler->window->getView();
	while (left < right) {
		int mIndex = (left + right) / 2;
		Entity* cur = renderOrder[mIndex];
		if (mIndex + 1 == renderOrder.size()) {
			return mIndex;
		}
		Entity* nextCur = renderOrder[mIndex + 1];
		if (cur->getY() <= handler->window->getSize().y + handler->camera->getYOffset() - (720 - v.getSize().y)) {
			if (nextCur->getY() <= handler->window->getSize().y + handler->camera->getYOffset() - (720 - v.getSize().y)) {
				left = mIndex + 1;
			} else {
				return mIndex;
			}

		} else {
			right = mIndex - 1;
		}
	}
	return renderOrder.size() - 1;
}