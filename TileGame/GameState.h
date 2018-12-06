#pragma once
#include <SFML/Main.hpp>

#define PLAYING 0
#define LOADING 1
namespace tg {
	class GameState
	{

	private:
		int type;

	public:
		GameState(int type);
		GameState();
		~GameState();

	public:
		int getType() { return type; }
		virtual void render();
		virtual void tick(sf::Int32 dt);
		virtual void exit();


	};

}