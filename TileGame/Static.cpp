#include "Static.h"
#include "Building.h"
#include "World.h"

using namespace tg;

// Add the static entity to the list of all static entities
// If another static entity already occupies that space, remove
// this new one
void Static::init() {

	if (staticList[world][tY][tX] == false && !Building::buildingOccupies(tX, tY, world, true)) {
		staticList[world][tY][tX] = true;
		world->getEntityManager()->addEntity(this);
		setSuccessfully = true;
	} else {
		delete this;
	}
}

bool Static::tileIsEmpty(int x, int y, World* world) {
	std::map<World*, std::vector<std::vector<bool>>>::iterator it = staticList.find(world);
	if (it == staticList.end()) {
		return true;
	}
	return !staticList[world][y][x];
}

Static::Static(int x, int y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type, float tileOffset, World* world) :
	Entity(x * 96, y * 96 + tileOffset - h, handler, hitBoxX, hitBoxY, hitBoxW, hitBoxH, w, h, needsTicking, type, false, world)
{

	if (staticList[world].size() == 0) {
		staticList[world] = std::vector<std::vector<bool>>(world->getHeight(),
			std::vector<bool>(world->getWidth()));
	}


	tX = x;
	tY = y;

	init();
}


Static::~Static()
{
	if (setSuccessfully) {
		staticList[world][tY][tX] = false;
	}
}

std::map<World*, std::vector<std::vector<bool>>> Static::staticList;
