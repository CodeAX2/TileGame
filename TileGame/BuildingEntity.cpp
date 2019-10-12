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


