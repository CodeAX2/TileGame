#pragma once

#include "Entity.h"
#include "Node.h"
#include <SFML/System.hpp>


namespace tg {
	class Pathfinder :
		public Entity {
	public:
		Pathfinder(float x, float y, Handler* handler, World* world, float speed);
		~Pathfinder();

	public:
		void setFollowing(Entity* entity) { following = entity; }
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;

	private:
		bool checkForCollision(float nX, float nY, bool collideWithPlayer = true);
		void generatePath();
		void addChild(int x, int y, int parentX, int parentY, std::vector<std::vector<Node>>* map, std::vector<sf::Vector2i>* openList, int targetX, int targetY);

	private:
		float speed;

		Entity* following;
		bool active = false;

		std::vector<sf::Vector2i> currentPath;
		std::vector<sf::Vector2i> queuedPath;
		bool pathIsQueued = false;
		int spotInpath = 0;

		sf::Int32 timeSincePathFind = 0;

		bool generating = false;

		sf::Thread pathThread;

		float initX = 0, initY = 0;


	};
}