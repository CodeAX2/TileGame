#pragma once

#include "Building.h"
namespace tg {
	class BuildingFile
	{

	public:
		BuildingFile(Building* building, std::string fileName, Handler* handler);
		~BuildingFile();

	private:
		Building* building; // Building instance (used for saving)
		std::string fileName; // Name of file to save building to
		Handler* handler; // Game handler


	public:
		// Load a building instance from a file
		static Building* loadBuildingFile(std::string fileName, Handler* handler);

	public:
		// Save the building to a file
		void saveFile();

	};
}
