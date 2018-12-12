#pragma once

#include "Entity.h"
namespace tg {
	class Pathfinder :
		public Entity {
	public:
		Pathfinder(float x, float y, Handler* handler, World* world, float speed);
		~Pathfinder();

	public:
		void setFollowing(Entity* entity) { following = entity; }
		void setDestination(float dx, float dy) { destX = dx; destY = dy; }
		void tick(sf::Int32 dt) override;

	private:
		bool checkForCollision(float nX, float nY);

	private:
		float destX;
		float destY;
		float speed;

		Entity* following;
		bool active = false;


	};
}