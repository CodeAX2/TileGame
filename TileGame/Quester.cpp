#include "Quester.h"
#include "InputManager.h"
#include "Player.h"
#include "GameState.h"

using namespace tg;


Quester::Quester(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, int type, World* world, float speed) :
	Pathfinder(x, y, handler, hitBoxX, hitBoxY, hitBoxW, hitBoxH,
		w, h, type, world, speed),
	Interactable(handler) {


	dialogueList.push_back("Hello, World! I am a test object for the game. My purpose in life is to help make the dialogue game state function correctly! Isn't that super exciting? Also....... Austin sucks.");
	dialogueList.push_back("#blamenoah! He started the zombie pigman apocalypse. If only he never went and tried to kill them all. Stupid noah!");
	dialogueList.push_back("MICROSOFT! FIX YOUR GOSH DARN GAME! IT IS SO BUGGY AND DOESN'T WORK!");
	dialogueList.push_back("Error on line 244. Fix your fr!cking game Jacob!");

	dialogueTexture = handler->assets->getDialogueNPCTexture();

}


Quester::~Quester()
{
}

void Quester::tick(sf::Int32 dt) {

	updateState(this);
	timeAlive += dt;

	if (following == nullptr && followingId != GUID_NULL) {
		following = world->getEntityManager()->getEntityById(followingId);
	}

	bool moved = false;

	// Make sure destination is correct
	if (following != nullptr) {
		if (pow(following->getX() - x, 2) + pow(following->getY() - y, 2) >= 960 * 960) {
			if (pow(following->getX() - x, 2) + pow(following->getY() - y, 2) >= 6144 * 6144) {
				world->getEntityManager()->removeEntity(this, true);
				return;
			}

			active = false;
		} else {
			active = true;
		}
	} else if (destination != sf::Vector2f(-1, -1)) {
		active = true;
	}

	if (!active) {
		// If we are too far from following, don't move. Fix textures

		return;
	}

	std::vector<sf::Vector2i> cP = currentPath;

	if (cP.size() != 0 && spotInpath < cP.size()) {

		float dx = cP[spotInpath].x * pathfindSize - (x + hitBoxX);
		float dy = cP[spotInpath].y * pathfindSize - (y + hitBoxY);

		float angle = atan2(dy, dx);



		float dist = speed * dt;

		float ogX = x;
		float ogY = y;

		bool moved = true;


		if (checkForCollision(x, y)) {
			// Collision before we moved, so disable collision for a sec
			x += dist * cos(angle);
			y += dist * sin(angle);
		} else {

			x += dist * cos(angle);

			if (checkForCollision(x, y)) {
				x = ogX;
			}

			y += dist * sin(angle);
			if (checkForCollision(x, y)) {
				y = ogY;
			}

			if (x == ogX && y == ogY) {
				moved = false;
			}

		}

		if (checkForCollisionWithFollowing(x, y)) {
			onCollisionWithFollowing(dt);
		}

		float checkX = cP[spotInpath].x * pathfindSize;
		float checkY = cP[spotInpath].y * pathfindSize;

		if (linesCross(ogX, ogY, x, y, checkX, checkY, checkX + pathfindSize, checkY)) {
			spotInpath++;
			x = checkX - hitBoxX; y = checkY - hitBoxY;
			dx = 0; dy = 0;
		} else if (linesCross(ogX, ogY, x, y, checkX, checkY, checkX, checkY + pathfindSize)) {
			spotInpath++;
			x = checkX - hitBoxX; y = checkY - hitBoxY;
			dx = 0; dy = 0;
		} else if (abs(dx) <= .25f && abs(dy) <= .25f) {
			spotInpath++;
			x = checkX - hitBoxX; y = checkY - hitBoxY;
			dx = 0; dy = 0;
		} else if (spotInpath == 0 && !moved) {
			spotInpath++;
		}

		if (spotInpath >= cP.size()) {
			spotInpath = 0;
		}

		if (pathIsQueued) {
			while (adjustingPath);
			adjustingPath = true;
			currentPath = queuedPath;
			pathIsQueued = false;
			spotInpath = 0;
			adjustingPath = false;
		}

		roundedHitBox = sf::IntRect(sf::Vector2i(std::round(x + (float)hitBoxX), std::round(y + (float)hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		world->getEntityManager()->fixEntityMoved(this, ogX, ogY);

	} else {

		if (pathIsQueued) {
			while (adjustingPath);
			adjustingPath = true;
			currentPath = queuedPath;
			spotInpath = 0;
			pathIsQueued = false;
			adjustingPath = false;
		}
	}
}

void Quester::render(Handler* handler) {

	if (handler->getCurrentState()->getType() == DIALOGUE) return;

	if (enabled) {
		sf::Texture* interactTexture = nullptr;
		if (handler->inputManager->usingController)
			interactTexture = handler->assets->getControllerInteractPrompt();
		else
			interactTexture = handler->assets->getInteractPrompt();

		sf::RectangleShape interact(sf::Vector2f(interactTexture->getSize().x * 3, interactTexture->getSize().y * 3));
		float yExtra = sin(timeAlive / 200.f) * 5;
		interact.setPosition(x - handler->camera->getXOffset() + w / 2.f - interactTexture->getSize().x * 3.f / 2.f, y - handler->camera->getYOffset() + yExtra - interactTexture->getSize().y * 3.f / 2.f);
		interact.setTexture(interactTexture);
		handler->window->draw(interact);
	} else {
		sf::Texture* questTexture = handler->assets->getAvaliableQuestPrompt();

		sf::RectangleShape interact(sf::Vector2f(questTexture->getSize().x * 3, questTexture->getSize().y * 3));
		float yExtra = sin(timeAlive / 200.f) * 2;
		interact.setPosition(x - handler->camera->getXOffset() + w / 2.f - questTexture->getSize().x * 3.f / 2.f, y - handler->camera->getYOffset() + yExtra - questTexture->getSize().y * 3.f / 2.f);
		interact.setTexture(questTexture);
		handler->window->draw(interact);
	}
}
