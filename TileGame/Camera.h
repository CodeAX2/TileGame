#pragma once

#include <SFML/Graphics.hpp>
#include "Handler.h"
#include "World.h"
#include "Entity.h"

class Camera
{

private:
	float cameraX, cameraY;
	Handler* handler;
	Entity* entity;

public:
	Camera(Handler* handler);

public:
	void centerAt(float x, float y, World* world);
	void centerOnEntity(Entity* e);
	void tick();
	void fixForZoom();


	float getXOffset() { return cameraX - handler->window->getSize().x / 2.f; } // Subtracts half the screen size, since the
	float getYOffset() { return cameraY - handler->window->getSize().y / 2.f; } // camera is in the center of the screen

	// These return the upper left corner of the camera
	sf::Vector2f getCameraPosition() {
		return sf::Vector2f(cameraX, cameraY);
	}


};

