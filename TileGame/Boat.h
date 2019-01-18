#pragma once
#include "Rideable.h"

namespace tg {

	class Boat :
		public Rideable
	{
	public:
		Boat(float x, float y, Handler* handler, World* world);
		~Boat();


	public:
		void damage(int dmg, Entity* damager) override;

	protected:
		bool checkForCollision() override;



	};
}
