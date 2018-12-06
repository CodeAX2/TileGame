#pragma once

#include "Building.h"
namespace tg {
	class BuildingFile
	{

	public:
		static Building* loadBuildingFile(std::string fileName, Handler* handler);


	public:
		BuildingFile(Building* building, std::string fileName, Handler* handler);
		~BuildingFile();

	public:
		void saveFile();


	private:
		Building * building;
		std::string fileName;
		Handler* handler;

	};
}
