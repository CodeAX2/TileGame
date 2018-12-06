#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Assets.h"
#include "World.h"
#include "Camera.h"
#include "EntityManager.h"
#include "Player.h"
#include "InputManager.h"
#include "Tree.h"
#include <cstdlib>
#include <time.h>
#include "TallGrass.h"
#include <sstream>
#include "PlayingState.h"
#include "LoadingState.h"
#include "WorldFile.h"
#include "PlayerFile.h"

using namespace tg;

sf::Mutex mutex;

Game::Game(sf::RenderWindow* window) : renderThread(&Game::renderLoop, this),
tickThread(&Game::tickLoop, this),
commandThread(&Game::commandLoop, this), debugThread(&Game::debugLoop, this) {

	srand(time(NULL) * rand());

	handler.game = this;

	handler.assets = new Assets;
	handler.initGameStates();
	handler.setGameState(LOADING);
	handler.window = window;
	handler.saveDirName = "Save1";
	handler.guiView = window->getView();

}

Game::~Game() {

}

void Game::commandLoop() {

	while (handler.window->isOpen() || togglingFullscreen) {


		std::string command;
		std::getline(std::cin, command);

		std::string cmd;
		std::vector<std::string> args;
		int index = 0;
		std::string curArg = "";
		for (char c : command) {
			if (c == ' ') {
				if (index == 0) {
					cmd = curArg;
				} else {
					args.push_back(curArg);
				}
				curArg = "";
				index++;
			} else {
				curArg += c;
			}
		}

		if (curArg != "") {
			if (index == 0) {
				cmd = curArg;
			} else {
				args.push_back(curArg);
			}
		}

		while (handler.getCurrentState()->getType() != PLAYING) {
			sf::sleep(sf::milliseconds(10));
		}


		if (cmd == "tp") {
			if (args.size() != 2) {
				std::cout << "Invalid arguments!" << std::endl;
			} else {
				int x = std::stoi(args[0]);
				int y = std::stoi(args[1]);
				handler.player->setPos(x * 96, y * 96);
				std::cout << "Teleporting to: " << x << ", " << y << std::endl;
			}
		} else if (cmd == "additem" || cmd == "give") {

			if (args.size() != 2) {
				std::cout << "Invalis arguments!" << std::endl;
			} else {
				int item = std::stoi(args[0]);
				int amnt = (int)std::stoll(args[1]);

				if (amnt > 0) {
					for (int i = 0; i < amnt; i++) {
						handler.player->addItemToInv(item);
					}
				} else {
					for (int i = 0; i < -1 * amnt; i++) {
						handler.player->removeItemFromInv(item);
					}
				}


			}

		} else {
			std::cout << "Invalid command!" << std::endl;
		}




	}




}

void Game::debugLoop() {
	sf::Int32 msToSleep = 6;
	sf::Clock clock;


	while (handler.window->isOpen() || togglingFullscreen) {
		sf::Int32 now = clock.getElapsedTime().asMilliseconds();


		while (debugOutput.size() != 0 && (handler.window->isOpen() || togglingFullscreen)) {
			std::cout << debugOutput[0] << std::endl;
			debugOutput.erase(debugOutput.begin());
		}

		sf::Int32 end = clock.getElapsedTime().asMilliseconds();
		sf::sleep(sf::milliseconds(msToSleep - (end - now)));
	}




}

void Game::renderLoop() {
	while (handler.window->isOpen() || togglingFullscreen) {


		mutex.lock();
		if (!isUpdating) {

			handler.window->clear();

			handler.getCurrentState()->render();

			handler.window->display();
		}
		mutex.unlock();


	}


}

void Game::tickLoop() {
	//mutex.lock();

	sf::Int32 msToSleep = 3;


	sf::Clock clock;
	sf::Int32 last = clock.getElapsedTime().asMilliseconds();
	while (handler.window->isOpen() || togglingFullscreen) {
		sf::Int32 now = clock.getElapsedTime().asMilliseconds();
		sf::Int32 dt = now - last;

		//std::cout << dt << std::endl;

		handler.getCurrentState()->tick(dt);

		last = now;
		sf::Int32 end = clock.getElapsedTime().asMilliseconds();

		sf::sleep(sf::milliseconds(msToSleep - (end - now)));
	}

	handler.getCurrentState()->exit();
	mutex.unlock();
}

void Game::start() {


	renderThread.launch();
	tickThread.launch();

	if (debugMode) {
		commandThread.launch();
		debugThread.launch();
	}

	sf::Int32 msToSleep = 4;

	sf::Clock clock;
	sf::Int32 last = clock.getElapsedTime().asMilliseconds();
	while (handler.window->isOpen() || togglingFullscreen) {
		sf::Int32 now = clock.getElapsedTime().asMilliseconds();
		sf::Int32 dt = now - last;
		sf::Event event;
		while (handler.window->pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				handler.window->close();
			} else {
				if (handler.getCurrentState()->getType() == LOADING) continue;
				handler.inputManager->mouseClicked(event);
				handler.inputManager->mouseScrolled(event);
				handler.inputManager->updateKeys(event);
				handler.inputManager->updateJoystick(dt);
			}


		}
		last = now;
		sf::Int32 end = clock.getElapsedTime().asMilliseconds();

		sf::sleep(sf::milliseconds(msToSleep - (end - now)));

	}




	renderThread.wait();
	tickThread.wait();

	WorldFile mainWorldFile(handler.mainWorld, &handler);
	mainWorldFile.saveFile();

	PlayerFile playerFile(handler.player, &handler);
	playerFile.saveFile();


}

void Game::toggleFullscreen() {

	togglingFullscreen = true;

	sf::RenderWindow* ow = handler.window;
	sf::RenderWindow* nw;

	if (isFullscreen) {
		sf::ContextSettings settings;
		settings.depthBits = 24;
		settings.stencilBits = 8;
		settings.antialiasingLevel = 4;
		settings.majorVersion = 4;
		settings.minorVersion = 4;

		nw = new sf::RenderWindow(sf::VideoMode(1280, 720), "TileGame!", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
		nw->setFramerateLimit(120);
		nw->setActive(false);
	} else {
		sf::ContextSettings settings;
		settings.depthBits = 24;
		settings.stencilBits = 8;
		settings.antialiasingLevel = 4;
		settings.majorVersion = 4;
		settings.minorVersion = 4;

		nw = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "TileGame!", sf::Style::None, settings);
		nw->setFramerateLimit(120);
		nw->setActive(false);
	}

	handler.window = nw;
	ow->setActive(false);
	delete ow;

	isFullscreen = !isFullscreen;

	togglingFullscreen = false;

}

void Game::wait() {

	tickThread.wait();
	renderThread.wait();

}
