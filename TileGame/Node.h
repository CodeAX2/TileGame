#pragma once
#include <SFML/System.hpp>



namespace tg {
	class Node {

	private:
		int hValue = 0;
		int gValue = 0;

		int parentX = -1, parentY = -1;

	public:
		Node();



	public:
		int getFValue() { return hValue + gValue; }
		int getHValue() { return hValue; }
		int getGValue() { return gValue; }
		int getParentX() { return parentX; }
		int getParentY() { return parentY; }

		void setGValue(int g) { gValue = g; }
		void setHValue(int h) { hValue = h; }
		void setParent(int parentX, int parentY) { this->parentX = parentX; this->parentY = parentY; }

	};
}
