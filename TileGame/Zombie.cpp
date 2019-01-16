#include "Zombie.h"
#include "Item.h"

const float Zombie::SPEED = .1f;

Zombie::Zombie(float x, float y, Handler* handler, World* world) :
	Pathfinder(x, y, handler, 16 * 2, 64 * 3 - 25, 32, 25, 32 * 3, 64 * 3, ZOMBIE_E, world, SPEED) {

	texture = handler->assets->getZombieAnimation()->getFrame(curAnim);


}


Zombie::~Zombie()
{
}


void Zombie::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	if (rand() % 2 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	}


}
