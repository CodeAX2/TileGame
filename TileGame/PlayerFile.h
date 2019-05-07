#pragma once

#include "Player.h"
#include "Nullable.h"

namespace tg {
	class PlayerFile
	{

	public:
		PlayerFile(Player* player, Handler* handler);
		~PlayerFile();

	private:
		Player* player; // The player instance used to save to a file
		Handler* handler; // Game handler

	public:
		// Create a player instance loaded from a file
		static Player* loadPlayerFile(std::string fileName, Handler* handler);

	public:
		// Save the player instance to a file
		void saveFile();

	private:
		// Safely load a value from a file
		template<typename T> static Nullable<T> safeLoad(std::ifstream& file);


	};
}
