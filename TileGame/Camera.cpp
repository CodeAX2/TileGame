#include "Camera.h"
#include "PlayingState.h"

using namespace tg;

Camera::Camera(Handler* handler) : handler(handler) {
}

// Center the camera at a specific x and y pos
void Camera::centerAt(float x, float y, World* world) {


	float newX = x;
	float newY = y;

	if (x < handler->worldView.getSize().x / 2)
		newX = handler->worldView.getSize().x / 2;
	if (x >= world->getWidth() * 96 - handler->worldView.getSize().x / 2)
		newX = world->getWidth() * 96 - handler->worldView.getSize().x / 2 - 1;

	if (y < handler->worldView.getSize().y / 2)
		newY = handler->worldView.getSize().y / 2;
	if (y >= world->getHeight() * 96 - handler->worldView.getSize().y / 2)
		newY = world->getHeight() * 96 - handler->worldView.getSize().y / 2 - 1;

	if (handler->worldView.getSize().x >= world->getWidth() * 96) {
		newX = world->getWidth() * 96 / 2;
	}

	if (handler->worldView.getSize().y >= world->getHeight() * 96) {
		newY = world->getHeight() * 96 / 2;
	}


	cameraX = newX; cameraY = newY;





}

// Center the camera on an entity
void Camera::centerOnEntity(Entity* e) {
	entity = e;
	cameraX = e->getX() + e->getWidth() / 2;
	cameraY = e->getY() + e->getHeight() / 2;
}

// Update the camera, setting its center at the entitie's x and y
void Camera::tick() {

	centerAt(entity->getX() + entity->getWidth() / 2, entity->getY() + entity->getHeight() / 2, entity->getWorld());

}

void Camera::fixForZoom() {
	tick();
}
