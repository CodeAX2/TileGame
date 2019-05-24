#include "Path.h"
#include <fstream>

using namespace tg;

Path::Path() {
}


Path::~Path() {
}

Path Path::loadPathFromFile(std::string fileName, Handler* handler) {
	std::ifstream file(handler->saveDirName + "\\" + fileName, std::ios::in | std::ios::binary);

	Path p;

	if (!file.is_open()) {
		return p;
	}


	int pathSize;
	file.read((char*)&pathSize, sizeof(int));

	for (int i = 0; i < pathSize; i++) {
		int command;
		file.read((char*)&command, sizeof(int));

		if (command == MOVE_TO) {
			float xPos, yPos;
			file.read((char*)&xPos, sizeof(float));
			file.read((char*)&yPos, sizeof(float));

			std::tuple<int, float, float> curPathAction;
			std::get<0>(curPathAction) = command;
			std::get<1>(curPathAction) = xPos;
			std::get<2>(curPathAction) = yPos;

			p.path.push_back(curPathAction);
		} else if (command == WAIT) {
			sf::Int32 waitTime;
			file.read((char*)&waitTime, sizeof(sf::Int32));

			std::tuple<int, float, float> curPathAction;
			std::get<0>(curPathAction) = command;
			std::get<1>(curPathAction) = waitTime;
			std::get<2>(curPathAction) = 0;

			p.path.push_back(curPathAction);
		}

	}

	return p;

}
