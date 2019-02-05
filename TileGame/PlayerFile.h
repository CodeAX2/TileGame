#pragma once

#include "Player.h"
#include "Nullable.h"

namespace tg {
	class PlayerFile
	{

	public:
		static Player* loadPlayerFile(std::string fileName, Handler* handler);

	public:
		PlayerFile(Player* player, Handler* handler);
		~PlayerFile();

	public:
		void saveFile();

	private:
		template<typename T> static Nullable<T> safeLoad(std::ifstream& file);


	private:
		Player * player;
		Handler* handler;

	};
}
