#include "BuildingEntity.h"
#include "PlayingState.h"


using namespace tg;

BuildingEntity::BuildingEntity(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type,
	bool needsInitialization, World* world) :

	Entity(
		x, y, handler, hitBoxX, hitBoxY, hitBoxW,
		hitBoxH, w, h, needsTicking, type, needsInitialization, world
	) {

	lightX = x - 1;
	lightY = y - 1;


}


BuildingEntity::~BuildingEntity()
{
}

void BuildingEntity::renderLighting(Handler* handler) {
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));

	float cX = handler->camera->getXOffset();
	float cY = handler->camera->getYOffset();

	ps->addWallLine(sf::Vector2f(x - cX, y + h - cY), sf::Vector2f(x + w - cX, y + h - cY));

	std::vector<sf::Vector2f> camVerticies;
	for (sf::Vector2f vertex : verticies) {
		camVerticies.push_back(sf::Vector2f(vertex.x - cX, vertex.y - cY));
	}

	ps->addBuildingVerticies(camVerticies);
}

void BuildingEntity::setVerticies(std::vector<sf::Vector2f> verticies) {

	for (int i = 0; i < verticies.size(); i++) {

		this->verticies.push_back(sf::Vector2f(x + verticies[i].x * 3, y + verticies[i].y * 3));

	}

}


