#pragma once
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define PLAYING 0
#define LOADING 1
#define MAIN_MENU 2
#define INVENTORY 3
#define CHEST_INVENTORY 4

namespace tg {
	class GameState
	{

	private:
		int type;

	protected:
		sf::Font guiFont;

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
		virtual void updateJoystick(sf::Int32 dt);


	};

}