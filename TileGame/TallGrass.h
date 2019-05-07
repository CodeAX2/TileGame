#pragma once
#include "Entity.h"
#include "Assets.h"
#include "Handler.h"
#include "Static.h"

#define TG_NORMAL 0
#define TG_SNOWY 1

namespace tg {
	class TallGrass :
		public Entity
	{
	public:
		TallGrass(float x, float y, Handler* handler, int type, World* world);
		~TallGrass();

	private:
		int eType; // Denotes what type the grass is

	public:
		// Return what type the grass is
		int getEType() { return eType; }

	};

}