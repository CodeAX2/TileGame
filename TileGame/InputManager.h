#pragma once
#include <SFML/Graphics.hpp>
#include "Handler.h"

namespace tg {

	class InputManager
	{

	private:
		Handler* handler;
		std::vector<sf::Keyboard::Key> numberKeys = {
			sf::Keyboard::Key::Num1,
			sf::Keyboard::Key::Num2,
			sf::Keyboard::Key::Num3,
			sf::Keyboard::Key::Num4,
			sf::Keyboard::Key::Num5,
			sf::Keyboard::Key::Num6,
			sf::Keyboard::Key::Num7,
			sf::Keyboard::Key::Num8,
			sf::Keyboard::Key::Num9
		};

	public:
		bool mouseIsPressed = false;
		bool usingController = false;
		bool runningKey = false;
		bool keys[4] = { false }; // Up, right, down, left
		bool toggleKey = false;
		bool previousToggleKey = false;
		bool hitboxShown = false;
		bool attackKey = false;
		float zoom = 1.f;
		bool joyStickButtons[8] = { false };
		bool dialogueKey = false;

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
		void updateJoystick(sf::Uint32 dt);
		void updateMouse();

	private:
		void updateKeys(sf::Event e);
		void mouseClicked(sf::Event e);
		void mouseScrolled(sf::Event e);



	};

}