#pragma once

#include "Player.h"
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
		Player * player;
		Handler* handler;

	};
}
