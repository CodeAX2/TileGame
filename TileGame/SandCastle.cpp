#include "SandCastle.h"
#include "Item.h"

using namespace tg;

SandCastle::SandCastle(int x, int y, Handler* handler, World* world) : Static(x, y, handler, 12, 32 * 3 - 8 * 3, 32 * 3 - 24, 8 * 3, 32 * 3, 32 * 3, false, SAND_CASTLE_E, 58, world) {
	texture = handler->assets->getSandCastleTexture();
	health = 60;
	maxHealth = 60;
}


SandCastle::~SandCastle()
{
}

// Drop items when the sandcastle is broken
void SandCastle::dropItems() {

	if (rand() % 3 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
		if (rand() % 2 == 0) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
		}
	}

}
