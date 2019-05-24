#pragma once

#include "Entity.h"
#include "Node.h"
#include <SFML/System.hpp>

// Compare two sf::Vector2i's to eachother for use in a sorted map
struct cmpVectorForMap {
	bool operator()(const sf::Vector2i& a, const sf::Vector2i b) const {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	}
};

namespace tg {
	class Pathfinder :
		public Entity {
	public:
		Pathfinder(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
			int hitBoxW, int hitBoxH, int w, int h, int type, World* world, float speed);
		~Pathfinder();

	protected:
		float speed; // The speed the pathfinder walks at

		Entity* following; // The entity the pathfinder is pathfinding to

		sf::Vector2f destination = sf::Vector2f(-1, -1); // The location the pathfinder is pathfinding to

		sf::Vector2i target; // The actual target we are pathfinding to

		UUID followingId = GUID_NULL; // The UUID of the following entity

		bool active = false; // If the pathfinder should move and generate a path

		std::vector<sf::Vector2i> currentPath; // The current generated path being followed

		std::vector<sf::Vector2i> queuedPath; // A new generated path, queued to replace the current path

		bool pathIsQueued = false; // If a new path is queued to replace the current one

		int spotInpath = 0; // The current index within the path the pathfinder is moving to

		sf::Int32 timeSincePathFind = 0; // The time since the previous path was generated

		bool generating = false; // If the pathfinder is generating a path

		int pathfindSize = 16; // The size of each square used for generating a path

		bool adjustingPath = false; // If the current path is being replaced by the queued path

	public:
		// Set the entity the pathfinder is pathfinding to
		void setFollowing(Entity* entity) { following = entity; }

		// Set the id of the entity the pathfinder is pathfinding to
		// If the id does not match following, following will be changed
		void setFollowingId(UUID id) { followingId = id; }

		// Update the state of the pathfinder
		void tick(sf::Int32 dt) override;

		// Render the pathfinder to the screen
		void render(Handler* handler) override;

		// Return the entity the pathfinder is pathfinding to
		Entity* getFollowing() { return following; }

		// Generate a path to the following entity using the A* algorithim
		void generatePath();

	protected:
		// Check for a collision at (nX, nY)
		bool checkForCollision(float nX, float nY, bool collideWithPlayer = true, bool colideWithPathfinder = true);

		// Add a child node to the node map (only for use for the A* algorithim)
		void addChild(int x, int y, int parentX, int parentY, std::map<sf::Vector2i, Node, cmpVectorForMap>* map, std::vector<sf::Vector2i>* openList, int targetX, int targetY);

		// Check if line {(x11, y11), (x12, y12)} crosses with line {(x21, y21), (x22, y22)}
		bool linesCross(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);

		// Check for a collision with the following entity at (nX, nY)
		bool checkForCollisionWithFollowing(float nX, float nY);

		// Runs when the pathfinder collides with the following entity
		virtual void onCollisionWithFollowing(sf::Int32 dt);

	};
}