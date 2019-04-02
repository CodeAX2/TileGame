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
#include "WorldManager.h"
#include "DeathQuotes.h"
#include "Zombie.h"
#include "ItemDesc.h"
#include "ItemMeta.h"

using namespace tg;

sf::Mutex mutex;

Game::Game(sf::RenderWindow* window) : renderThread(&Game::renderLoop, this),
tickThread(&Game::tickLoop, this),
commandThread(&Game::commandLoop, this), debugThread(&Game::debugLoop, this) {

	srand(time(NULL) * rand());

	handler.game = this;

	handler.saveDirName = "Save1";
	handler.assets = new Assets;
	handler.initGameStates();
	handler.setGameState(MAIN_MENU);
	handler.window = window;
	handler.guiView = window->getView();
	handler.inputManager = new InputManager(&handler);

	DeathQuotes::init();
	ItemDesc::init();
	ItemMeta::init();

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

		while (handler.getCurrentState()->getType() == LOADING || handler.getCurrentState()->getType() == MAIN_MENU) {
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

		} else if (cmd == "killzombies") {
			std::vector <Entity*> allEntities = handler.player->getWorld()->getEntityManager()->getAllEntities();
			for (Entity* e : allEntities) {
				if (e->type == ZOMBIE_E) {
					e->damage(e->getHealth(), nullptr);
				}
			}
		} else if (cmd == "spawnzombie") {
			if (args.size() != 2) {
				std::cout << "Invalid arguments!" << std::endl;
			} else {

				int spawnX, spawnY;

				if (args[0].substr(0, 1) == "~") {
					spawnX = handler.player->getX();
					if (args[0].length() > 1) {
						int multiplier = 1;
						if (args[0].substr(1, 1) == "+") {
							multiplier = 1;
						} else if (args[0].substr(1, 1) == "-") {
							multiplier = -1;
						}

						spawnX += multiplier * stoi(args[0].substr(2));

					}

				} else {
					spawnX = stoi(args[0]);
				}

				if (args[1].substr(0, 1) == "~") {
					spawnY = handler.player->getY();
					if (args[1].length() > 1) {
						int multiplier = 1;
						if (args[1].substr(1, 1) == "+") {
							multiplier = 1;
						} else if (args[1].substr(1, 1) == "-") {
							multiplier = -1;
						}

						spawnY += multiplier * stoi(args[1].substr(2));

					}

				} else {
					spawnY = stoi(args[1]);
				}

				Zombie* z = new Zombie((float)spawnX, (float)spawnY, &handler, handler.player->getWorld());
				z->setFollowing(handler.player);
			}

		} else if (cmd == "getpos") {
			std::cout << handler.player->getX() << " " << handler.player->getY() << std::endl;
		} else if (cmd == "mutemusic") {
			PlayingState* ps = dynamic_cast<PlayingState*>(handler.getCustomState(PLAYING));
			ps->pauseBGMusic();

		} else if (cmd == "settime") {
			int time = std::stof(args[0]);
			PlayingState* ps = dynamic_cast<PlayingState*>(handler.getCustomState(PLAYING));
			ps->setTime(time);
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

	while (handler.window->isOpen()) {
		if (!isUpdating && !togglingFullscreen) {

			handler.window->clear();

			handler.getCurrentState()->render();

			handler.window->display();
		}

	}

}

void Game::tickLoop() {
	sf::Int32 msToSleep = 3;


	sf::Clock clock;

	handler.clock = &clock;

	sf::Int32 last = clock.getElapsedTime().asMilliseconds();
	while (handler.window->isOpen()) {

		if (togglingFullscreen) {
			sf::sleep(sf::milliseconds(msToSleep));
		}
		sf::Int32 now = clock.getElapsedTime().asMilliseconds();

		sf::Int32 dt = now - last;
		mutex.lock();
		handler.getCurrentState()->tick(dt);
		mutex.unlock();
		last = now;
		sf::Int32 end = clock.getElapsedTime().asMilliseconds();

		sf::sleep(sf::milliseconds(msToSleep - (end - now)));
	}

	handler.getCurrentState()->exit();

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
		if (!togglingFullscreen) {
			handler.inputManager->updateJoystick(dt);
			handler.inputManager->updateMouse();
			sf::Event event;
			while (handler.window->pollEvent(event)) {

				if (event.type == sf::Event::Closed) {
					handler.window->close();
				} else {
					handler.inputManager->handleEvents(event, dt);
				}


			}
		}
		last = now;
		sf::Int32 end = clock.getElapsedTime().asMilliseconds();

		sf::sleep(sf::milliseconds(msToSleep - (end - now)));

	}




	renderThread.wait();
	tickThread.wait();

	if (handler.player != nullptr) {
		PlayerFile playerFile(handler.player, &handler);
		playerFile.saveFile();
	}

	if (handler.worldManager != nullptr) {
		std::vector<World*> worlds = handler.worldManager->getAllWorlds();
		if (worlds.size() == 0) {
			return;
		}
		for (World* w : worlds) {
			WorldFile curWorldFile(w, &handler);
			curWorldFile.saveFile();
		}
	}

	if (PlayingState* ps = dynamic_cast<PlayingState*>(handler.getCustomState(PLAYING))) {
		ps->saveStateFile();
	}


}

void Game::toggleFullscreen() {

	togglingFullscreen = true;

	sf::RenderWindow* ow = handler.window;

	ow->close();
	renderThread.wait();
	tickThread.wait();

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 4;

	if (isFullscreen) {
		ow->create(sf::VideoMode(1280, 720), "TileGame!", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
	} else {
		ow->create(sf::VideoMode::getDesktopMode(), "TileGame!", sf::Style::None, settings);
	}

	ow->setFramerateLimit(120);

	isFullscreen = !isFullscreen;

	togglingFullscreen = false;

	ow->setActive(false);

	renderThread.launch();
	tickThread.launch();

}

void Game::wait() {

	tickThread.wait();
	renderThread.wait();

}
