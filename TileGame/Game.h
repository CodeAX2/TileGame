#pragma once
#include "GameState.h"
#include "Handler.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace tg {

	class Game {

	public:
		Game(sf::RenderWindow* window);
		~Game();

	private:
		const bool debugMode = true; // If the game is running in debug mode

		sf::Thread renderThread; // The thread used for rendering the game

		sf::Thread tickThread; // The thread used for updating the game

		sf::Thread commandThread; // The thread used for recieving commands from the console

		sf::Thread debugThread; // The thread used for outputting debug messages

		Handler handler; // The game handler

		bool isUpdating = false; // If the game is currently updating

		bool isFullscreen = false; // If the game is running in a fullscreen window

		bool togglingFullscreen = false; // If the game is switching to or from a fullscreen window

		std::vector<std::string> debugOutput; // A list containing all of the debug messages to be output

	public:
		// Start all of the game's processes and threads
		void start();

		// Toggle to or from a fullscreen window
		void toggleFullscreen();

		// Wait for the final processes to finish, the stop the game
		void wait();

		// Print a debug message to the screen
		void debugLog(std::string info) { debugOutput.push_back(info); }

	private:
		// Render the entire game
		void renderLoop();

		// Update the entire game
		void tickLoop();

		// Print any queued debug messages
		void debugLoop();

		// Recieve input from the console
		void commandLoop();

	};

}