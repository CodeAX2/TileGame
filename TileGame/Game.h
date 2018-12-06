#pragma once
#include "GameState.h"
#include "Handler.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace tg {

	class Game
	{

	private:
		const bool debugMode = true;


		sf::Thread renderThread;
		sf::Thread tickThread;
		sf::Thread commandThread;
		sf::Thread debugThread;
		Handler handler;


		bool isUpdating = false;
		bool isFullscreen = false;
		bool togglingFullscreen = false;

		std::vector<std::string> debugOutput;

	public:
		Game(sf::RenderWindow* window);
		~Game();

	private:
		void renderLoop();
		void tickLoop();
		void debugLoop();
		void commandLoop();


	public:
		void start();
		void toggleFullscreen();
		void wait();
		void debugLog(std::string info) { debugOutput.push_back(info); }


	};

}