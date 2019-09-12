#pragma once
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


// Definitions of the types of playing states
#define PLAYING 0
#define LOADING 1
#define MAIN_MENU 2
#define INVENTORY 3
#define CHEST_INVENTORY 4
#define CRAFTING_INVENTORY 5
#define DIALOGUE 6

namespace tg {
	class GameState {

	public:
		GameState(int type);
		GameState();
		~GameState();

	protected:
		sf::Font guiFont; // The font used for any gui elements

	private:
		int type; // The type of the gamestate

	public:
		// Get the type of the gamestate
		int getType() { return type; }

		// Render the gamestate to the screen
		virtual void render();

		// Update the gamestate
		virtual void tick(sf::Int32 dt);

		// Exit out of the gamestate
		virtual void exit();

		// Momentarily exit out of the gamestate
		virtual void pause();

		// Enter back into the gamestate
		virtual void resume();

		// Trigger events related to a mouse click
		virtual void mouseClicked(sf::Event e);

		// Update the position of the mouse
		virtual void updateMouse();

		// Update the state of the joystick/controller
		virtual void updateJoystick(sf::Int32 dt);

		// Load any info associated with the state
		virtual void load();

		// Save any info associated with the state
		virtual void save();


	};

}