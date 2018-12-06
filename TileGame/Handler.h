#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
namespace tg {
	class World;
	class Assets;
	class Game;
	class Camera;
	class EntityManager;
	class Player;
	class GameState;
	class InputManager;
	class WorldManager;


	class Handler
	{

	public:
		World * mainWorld;
		Assets* assets;
		Game* game;
		sf::RenderWindow* window;
		Camera* camera;
		EntityManager* entityManager;
		Player* player;
		InputManager* inputManager;
		sf::View worldView, guiView;
		WorldManager* worldManager;
		std::string saveDirName;

	public:
		Handler();
		~Handler();

	public:
		void setGameState(int stateId);
		void initGameStates();
		GameState* getCurrentState() { return currentState; }
		GameState* getCustomState(int stateId) { return allStates[stateId]; }

	private:
		std::vector<GameState*> allStates;
		GameState* currentState;


	};

}