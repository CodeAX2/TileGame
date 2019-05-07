#pragma once

#include <SFML/Graphics.hpp>
#include "Handler.h"
#include "World.h"
#include "Entity.h"

namespace tg {

	class Camera {

	public:
		Camera(Handler* handler);

	private:
		float cameraX, cameraY; // The x and y coordinates of the camera
		Handler* handler; // The game handler
		Entity* entity; // The entity the camera is tracking

	public:
		// Center the camera at an x and y position within a world
		void centerAt(float x, float y, World* world);

		// Center the camera on a specific entity
		void centerOnEntity(Entity* e);

		// Update the state of the camera
		void tick();

		// Fix the camera's position to account for zooming in and out
		void fixForZoom();

		// Get the x offset of the camera to properly render to the screen
		float getXOffset() { return cameraX - handler->window->getSize().x / 2.f; } // Subtracts half the screen size, since the
																					// camera is in the center of the screen

		// Get the y offset of the camera to properly render to the screen			
		float getYOffset() { return cameraY - handler->window->getSize().y / 2.f; }

		// Return the upper left corner of the camera
		sf::Vector2f getCameraPosition() { return sf::Vector2f(cameraX, cameraY); }


	};

}