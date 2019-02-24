#pragma once
#include "Pathfinder.h"


#define STILL 0
using namespace tg;

namespace tg {
	class Zombie :
		public Pathfinder
	{

	private:
		static const float SPEED;
		int curAnim = 0;
		sf::Int32 timeSinceDirChange = 0;
		sf::Int32 timeSinceLastAnim = 0;
		int directionX = STILL;
		int directionY = STILL;
		int facing = 3; // SOUTH
		bool attacking = false;
		sf::Int32 attackCooldown = 0;

	public:
		Zombie(float x, float y, Handler* handler, World* world);
		~Zombie();

	protected:
		void dropItems() override;
		void onCollisionWithFollowing(sf::Int32 dt);
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		void damage(int dmg, Entity* damager) override;

	};
}