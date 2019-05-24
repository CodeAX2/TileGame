#pragma once
#include <tuple>
#include <vector>
#include "Handler.h"

namespace tg {

	class Path {
	public:
		Path();
		~Path();

	public:
		const static int MOVE_TO = 0;
		const static int WAIT = 1;

	private:
		std::vector<std::tuple<int, float, float>> path;

	public:
		static Path loadPathFromFile(std::string fileName, Handler* handler);

	public:
		std::vector<std::tuple<int, float, float>> getPath() { return path; }

		int getCommand(int pathIndex) { return std::get<0>(path[pathIndex]); }
		
		sf::Vector2f getMovePosition(int pathIndex) { return sf::Vector2f(std::get<1>(path[pathIndex]), std::get<2>(path[pathIndex])); }
		
		sf::Int32 getWaitTime(int pathIndex) { return sf::Int32(std::get<1>(path[pathIndex])); }

		int getPathSize() { return path.size(); }

	};
}
