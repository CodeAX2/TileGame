#pragma once

#include "World.h"
#include <string>
#include "Entity.h"
#include "Nullable.h"

namespace tg {
	class WorldFile
	{

	public:
		static World* loadWorldFile(std::string fileName, Handler* handler);

	public:
		WorldFile();
		WorldFile(World* world, Handler* handler);
		~WorldFile();

		void saveFile();

	private:
		template<typename T> static Nullable<T> safeLoad(std::ifstream& file);

	private:
		World * world;
		Handler* handler;

	};

}