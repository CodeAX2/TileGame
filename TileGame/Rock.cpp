#include "Rock.h"
#include "Item.h"

using namespace tg;

Rock::Rock(int x, int y, Handler* handler, World* world) : 
	Static(x, y, handler, 6, 32 * 3 - 8 * 3, 32 * 3 - 12, 8 * 3, 32 * 3, 32 * 3, false, ROCK_E, 73, world) {

	texture = handler->assets->getRockTexture();
	health = 60;
	maxHealth = 60;

}


Rock::~Rock()
{
}



void Rock::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 10, world);

	if (rand() % 3 <= 1) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 10, world);
		if (rand() % 2 == 0) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 10, world);
		}
	}

}