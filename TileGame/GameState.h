#pragma once
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>

#define PLAYING 0
#define LOADING 1
#define MAIN_MENU 2
#define INVENTORY 3

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
		virtual void pause();
		virtual void resume();
		virtual void mouseClicked(sf::Event e);
		virtual void updateMouse();


	};

}