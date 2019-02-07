#pragma once

#include "Entity.h"
#include "Node.h"
#include <SFML/System.hpp>


namespace tg {
	class Pathfinder :
		public Entity {
	public:
		Pathfinder(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
			int hitBoxW, int hitBoxH, int w, int h, int type, World* world, float speed);
		~Pathfinder();

	public:
		void setFollowing(Entity* entity) { following = entity; }
		void setFollowingId(UUID id) { followingId = id; }
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		Entity* getFollowing() { return following; }

	protected:
		bool checkForCollision(float nX, float nY, bool collideWithPlayer = true, bool colideWithPathfinder = true);
		void generatePath();
		void addChild(int x, int y, int parentX, int parentY, std::vector<std::vector<Node>>* map, std::vector<sf::Vector2i>* openList, int targetX, int targetY);
		bool linesCross(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);
		bool checkForCollisionWithFollowing(float nX, float nY);
		virtual void onCollisionWithFollowing(sf::Int32 dt);

	protected:
		float speed;

		Entity* following;
		UUID followingId = GUID_NULL;
		bool active = false;

		std::vector<sf::Vector2i> currentPath;
		std::vector<sf::Vector2i> queuedPath;
		bool pathIsQueued = false;
		int spotInpath = 0;

		sf::Int32 timeSincePathFind = 0;

		bool generating = false;

		sf::Thread pathThread;

		int pathfindSize = 16;


	};
}