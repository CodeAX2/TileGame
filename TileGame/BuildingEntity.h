#pragma once
#include "Entity.h"


namespace tg {

	class BuildingEntity :
		public Entity
	{
	public:
		BuildingEntity(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
			int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type,
			bool needsInitialization, World* world);
		~BuildingEntity();

	public:
		void renderLighting(Handler* handler) override;

	};

}
