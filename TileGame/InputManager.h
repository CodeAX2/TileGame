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
		bool mouseIsPressed = false;
		bool usingController = false;
		Handler* handler;
		float zoom = 1.f;

	public:

		InputManager(Handler* handler);

	public:

		void updateKeys(sf::Event e);
		void mouseClicked(sf::Event e);
		bool getKey(int index) { return keys[index]; }
		bool* getAllKeys();
		bool getToggleKey() { return toggleKey; }
		bool hitboxIsShown() { return hitboxShown; }
		bool getRunningKey() { return runningKey; }
		bool getAttackKey() { return attackKey; }
		void updateJoystick(sf::Uint32 dt);
		float getZoomLevel() { return zoom; }
		void setZoomLevel(float z) { zoom = z; }
		void mouseScrolled(sf::Event e);


	};

}