#include "TallGrass.h"

using namespace tg;

TallGrass::TallGrass(float x, float y, Handler* handler, int type, World* world) : Entity(x, y, handler, 0, 0, 0, 0, 32 * 3, 32 * 3, false, TALL_GRASS_E, true, world) {
	this->eType = type;

	if (eType == TG_NORMAL) {
		texture = handler->assets->getTallGrassTexture();
	} else if (eType == TG_SNOWY) {
		texture = handler->assets->getTallSnowGrassTexture();
	}


}


TallGrass::~TallGrass()
{
}

// Nuff said