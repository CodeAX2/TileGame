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
		// Apply damage to the boat
		void damage(int dmg, Entity* damager) override;

	protected:
		// Check if the current position of the boat causes a collision
		bool checkForCollision() override;

	};
}
