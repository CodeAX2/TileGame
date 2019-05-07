#pragma once
#include "Pathfinder.h"


#define STILL 0

namespace tg {
	class Zombie :
		public Pathfinder
	{

	public:
		Zombie(float x, float y, Handler* handler, World* world);
		~Zombie();


	private:
		static const float SPEED; // Constant walk speed of zombies

	private:
		int curAnim = 0; // The current animation index used for rendering

		sf::Int32 timeSinceDirChange = 0; // Time dince the directionX and directionY values were updated

		sf::Int32 timeSinceLastAnim = 0; // Time since the animation was updated

		int directionX = STILL; // X direction the zombie is moving

		int directionY = STILL; // Y direction the zombie is moving

		int facing = 3; // The current direction the zombie is facing. SOUTH is default

		bool attacking = false; // If the zombie is attacking

		sf::Int32 attackCooldown = 0; // Cooldown until the zombie can attack again

		bool beingKnockbacked = false; // If the zombie was damaged and is being knocked back

		sf::Int32 knockbackTime = 0; // Time the zombie has been knocked back for

		float knockbackDegree = 0; // The angle of the knockback trajectory

		float knockbackDistance = 200; // How far the zombie should be knocked back (not in pixels!)

		const sf::Int32 timeToKnockback = 200; // How long the knockback should take to complete

	protected:
		// Drop the items upon death
		void dropItems() override;

		// Runs when the zombie collides with the entity it is following
		void onCollisionWithFollowing(sf::Int32 dt);

		// Update the state of the zombie
		void tick(sf::Int32 dt) override;

		// Render the zombie
		void render(Handler* handler) override;

		// Apply damage to the zombie
		void damage(int dmg, Entity* damager) override;

	};
}