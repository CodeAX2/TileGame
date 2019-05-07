#pragma once
#include "Pathfinder.h"

#define STILL 0

namespace tg {

	class Skeleton :
		public Pathfinder
	{

	public:
		Skeleton(float x, float y, Handler* handler, World* world);
		~Skeleton();

	private:
		static const float SPEED; // Constant walk speed of skeletons

	private:
		int curAnim = 0; // The current animation index used for rendering

		sf::Int32 timeSinceDirChange = 0; // Time dince the directionX and directionY values were updated

		sf::Int32 timeSinceLastAnim = 0; // Time since the animation was updated

		int directionX = STILL; // X direction the skeleton is moving

		int directionY = STILL; // Y direction the skeleton is moving

		int facing = 3; // The current direction the skeleton is facing. SOUTH is default

		bool attacking = false; // If the skeleton is attacking

		sf::Int32 attackCooldown = 0; // Cooldown until the skeleton can attack again

		bool beingKnockbacked = false; // If the skeleton was damaged and is being knocked back

		sf::Int32 knockbackTime = 0; // Time the skeleton has been knocked back for

		float knockbackDegree = 0; // The angle of the knockback trajectory

		float knockbackDistance = 200; // How far the skeleton should be knocked back (not in pixels!)

		const sf::Int32 timeToKnockback = 200; // How long the knockback should take to complete

	protected:
		// Drop the items upon death
		void dropItems() override;

		// Runs when the skeleton collides with the entity it is following
		void onCollisionWithFollowing(sf::Int32 dt);

		// Update the state of the skeleton
		void tick(sf::Int32 dt) override;

		// Render the skeleton
		void render(Handler* handler) override;

		// Apply damage to the skeleton
		void damage(int dmg, Entity* damager) override;

	};

}
