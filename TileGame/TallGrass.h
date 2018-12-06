#pragma once
#include "Entity.h"
#include "Assets.h"
#include "Handler.h"
#include "Static.h"

#define TG_NORMAL 0
#define TG_SNOWY 1

class TallGrass :
	public Entity
{
public:
	TallGrass(float x, float y, Handler* handler, int type, World* world);
	~TallGrass();

	int getEType() { return eType; }

private:
	int eType;

};

