#include "Interactable.h"
#include "Player.h"

using namespace tg;


Interactable::Interactable(Handler* handler) {
	this->interactHandler = handler;
}

Interactable::~Interactable() {
	if (interactHandler->player->getCurrentInteracting() == this) {
		interactHandler->player->setCurrentInteracting(nullptr);
	}
}

void Interactable::onInteract(){}

void Interactable::updateState(Entity* caller) {
	Player* p = interactHandler->player;
	if (p->getWorld() != caller->getWorld()) return;


	float x = caller->getX();
	float y = caller->getY();
	float collisionBoxX = caller->getCollisionBox().left; // (x,y) + (hitBoxX, hitBoxY)
	float collisionBoxY = caller->getCollisionBox().top;
	float hitBoxW = caller->getCollisionBox().width;
	float hitBoxH = caller->getCollisionBox().height;

	sf::Vector2f toPlayer(p->getCollisionBox().left, p->getCollisionBox().top); // Add this to x,y to get player's pos
	toPlayer -= sf::Vector2f(collisionBoxX, collisionBoxY);

	int pFacingHori = p->getFacingHori();
	int pFacingVert = p->getFacingVert();

	int vFacingHori, vFacingVert;

	if (toPlayer.x < -1 * p->getCollisionBox().width + 1) {
		vFacingHori = WEST;
	} else if (toPlayer.x > hitBoxW - 1) {
		vFacingHori = EAST;
	} else {
		vFacingHori = STILL;
	}

	if (toPlayer.y < -1 * p->getCollisionBox().height + 1) {
		vFacingVert = NORTH;
	} else if (toPlayer.y > hitBoxH - 1) {
		vFacingVert = SOUTH;
	} else {
		vFacingVert = STILL;
	}

	bool horiGood = false;
	bool vertGood = false;

	if (
		(vFacingHori == WEST && pFacingHori == EAST) ||
		(vFacingHori == EAST && pFacingHori == WEST) ||
		(vFacingHori == STILL && pFacingHori == STILL)
		) {
		horiGood = true;
	}

	if (
		(vFacingVert == NORTH && pFacingVert == SOUTH) ||
		(vFacingVert == SOUTH && pFacingVert == NORTH) ||
		(vFacingVert == STILL && pFacingVert == STILL)
		) {
		vertGood = true;
	}

	sf::Vector2f toPCenter(
		p->getCollisionBox().left + p->getCollisionBox().width / 2,
		p->getCollisionBox().top + p->getCollisionBox().height / 2
	);

	toPCenter -= sf::Vector2f(collisionBoxX + hitBoxW / 2, collisionBoxY + hitBoxH / 2);

	if (horiGood && vertGood && toPCenter.x * toPCenter.x + toPCenter.y * toPCenter.y <= 75 * 75) {
		if (interactHandler->player->getCurrentInteracting() == nullptr || interactHandler->player->getCurrentInteracting() == this) {
			interactHandler->player->setCurrentInteracting(this);
			enabled = true;
		} else {
			enabled = false;
		}
		// TODO Make it so that the interacting is based on the closest one
	} else {
		if (interactHandler->player->getCurrentInteracting() == this) {
			interactHandler->player->setCurrentInteracting(nullptr);
		}
		enabled = false;
	}

}
