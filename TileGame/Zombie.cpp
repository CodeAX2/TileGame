#include "Zombie.h"



Zombie::Zombie(float x, float y, Handler* handler, World* world) : 
	Pathfinder(x, y, handler, 16*2, 64*3 - 25, 32, 25, 32 * 3, 64 * 3, ZOMBIE_E, world, speed) {

	texture = handler->assets->getZombieAnimation()->getFrame(curAnim);

}


Zombie::~Zombie()
{
}
