#pragma once
#include <SFML/System.hpp>

namespace tg {
	class Node {

	public:
		Node();

	private:
		int hValue = 0; // H value (used for A* pathfinding)

		int gValue = 0; // G value (used for A* pathfinding)

		int parentX = -1, parentY = -1; // Parent x and y coordinates (used for A* pathfinding)

	public:
		// Return the F value (used for A* pathfinding)
		int getFValue() { return hValue + gValue; }

		// Return the H value (used for A* pathfinding)
		int getHValue() { return hValue; }

		// Return the G value (used for A* pathfinding)
		int getGValue() { return gValue; }

		// Return the x coordinate of the parent (used for A* pathfinding)
		int getParentX() { return parentX; }

		// Return the y coordinate of the parent (used for A* pathfinding)
		int getParentY() { return parentY; }

		// Set the G value of the node
		void setGValue(int g) { gValue = g; }

		// Set the H value of the node
		void setHValue(int h) { hValue = h; }

		// Set the parent x and y coordinates
		void setParent(int parentX, int parentY) {
			this->parentX = parentX; this->parentY = parentY;
		}

	};
}
