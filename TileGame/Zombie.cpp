#include "Zombie.h"


const float Zombie::SPEED = .1f;

Zombie::Zombie(float x, float y, Handler* handler, World* world) : 
	Pathfinder(x, y, handler, 16*2, 64*3 - 25, 32, 25, 32 * 3, 64 * 3, ZOMBIE_E, world, SPEED) {

	texture = handler->assets->getZombieAnimation()->getFrame(curAnim);


}


Zombie::~Zombie()
{
}
