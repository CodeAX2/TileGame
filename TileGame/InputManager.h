#pragma once
#include <SFML/Graphics.hpp>
#include "Handler.h"

namespace tg {

	class InputManager
	{

	private:
		bool keys[4] = { false }; // Up, right, down, left
		bool toggleKey = false;
		bool previousToggleKey = false;
		bool hitboxShown = false;
		bool runningKey = false;
		bool attackKey = false;
		Handler* handler;
		float zoom = 1.f;

	public:
		bool mouseIsPressed = false;
		bool usingController = false;

	public:

		InputManager(Handler* handler);

	public:

		void handleEvents(sf::Event e, sf::Uint32 dt);
		bool getKey(int index) { return keys[index]; }
		bool* getAllKeys();
		bool getToggleKey() { return toggleKey; }
		bool hitboxIsShown() { return hitboxShown; }
		bool getRunningKey() { return runningKey; }
		bool getAttackKey() { return attackKey; }
		float getZoomLevel() { return zoom; }
		void setZoomLevel(float z) { zoom = z; }
		void disableCurrentMovement();

	private:
		void updateKeys(sf::Event e);
		void mouseClicked(sf::Event e);
		void mouseScrolled(sf::Event e);

		void updateJoystick(sf::Uint32 dt);
		void updateMouse();

	};

}