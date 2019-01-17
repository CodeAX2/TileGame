#pragma once
#include "Pathfinder.h"

using namespace tg;

class Zombie :
	public Pathfinder
{

private:
	static const float SPEED;
	int curAnim = 0;
	sf::Int32 timeSinceAttacking = 0;

public:
	Zombie(float x, float y, Handler* handler, World* world);
	~Zombie();

protected:
	void dropItems() override;
	void onCollisionWithFollowing(sf::Int32 dt);

};