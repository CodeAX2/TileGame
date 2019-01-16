#pragma once
#include "Pathfinder.h"

using namespace tg;

class Zombie :
	public Pathfinder
{

private:
	const float speed = .25f;
	int curAnim = 0;

public:
	Zombie(float x, float y, Handler* handler, World* world);
	~Zombie();
};

