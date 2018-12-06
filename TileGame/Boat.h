#pragma once
#include "Rideable.h"
class Boat :
	public Rideable
{
public:
	Boat(float x, float y, Handler* handler, World* world);
	~Boat();


public:
	void damage(int dmg) override;
	
protected:
	bool checkForCollision() override;



};

