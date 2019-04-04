#pragma once
#include "Pathfinder.h"

#define STILL 0

namespace tg {

	class Skeleton :
		public Pathfinder
	{

	public:

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
		bool beingKnockbacked = false;
		sf::Int32 knockbackTime = 0;
		float knockbackDegree = 0;
		float knockbackDistance = 200; // Not in pixels, it is a strange value to allow for deceleration
		const sf::Int32 timeToKnockback = 200;

	public:
		Skeleton(float x, float y, Handler* handler, World* world);
		~Skeleton();

	protected:
		void dropItems() override;
		void onCollisionWithFollowing(sf::Int32 dt);
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		void damage(int dmg, Entity* damager) override;

	};

}
