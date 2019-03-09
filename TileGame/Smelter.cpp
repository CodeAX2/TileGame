#include "Smelter.h"

using namespace tg;


Smelter::Smelter(int x, int y, Handler* handler, World* world) :
	Static(x, y, handler, 9, 32 * 3 - 25, 32 * 3 - 18, 25, 32 * 3, 32 * 3, true, SMELTER_E, 73, world),
	Interactable(handler) {

	this->texture = handler->assets->getSmelterTexture(0);
	this->maxHealth = 60;
	this->health = 60;


}


Smelter::~Smelter()
{
}
