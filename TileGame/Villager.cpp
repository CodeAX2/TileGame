#include "Villager.h"
#include "Player.h"
#include "GameState.h"
#include "DialogueState.h"

using namespace tg;

Villager::Villager(float x, float y, Handler* handler, World* world, std::string pathFileName) :
	Quester(x, y, handler, 14, 117 - 25, 31, 25, 20 * 3, 39 * 3, VILLAGER, world, .1f) {


	texture = handler->assets->getPlayerAnim()->getFrame(0);
	health = 1;
	maxHealth = 1;
	path = Path::loadPathFromFile(pathFileName, handler);

}


Villager::~Villager()
{
}


void Villager::damage(int dmg, Entity* damager) {
	return;
}

void Villager::tick(sf::Int32 dt) {

	if (path.getPathSize() != 0) {

		if (path.getCommand(curPathIndex) == Path::WAIT) {

			waitTime += dt;
			if (waitTime >= path.getWaitTime(curPathIndex)) {
				curPathIndex++;
				if (curPathIndex >= path.getPathSize()) curPathIndex = 0;
				waitTime = 0;
				destination = sf::Vector2f(-1, -1);
				target = sf::Vector2i();
				queuedPath.clear();
				pathIsQueued = false;
				currentPath.clear();
				spotInpath = 0;
			}

			updateState(this);
			return;

		} else if (path.getCommand(curPathIndex) == Path::MOVE_TO && 
			destination != path.getMovePosition(curPathIndex)) {
			destination = path.getMovePosition(curPathIndex);
			queuedPath.clear();
			pathIsQueued = false;
			currentPath.clear();
			spotInpath = 0;
		}
	}

	Quester::tick(dt);
}

void Villager::render(Handler* handler) {
	Pathfinder::render(handler);
	Quester::render(handler);
}

void Villager::onCollisionWithFollowing(sf::Int32 dt) {
	curPathIndex++;
	if (curPathIndex >= path.getPathSize()) curPathIndex = 0;
}

void Villager::onInteract() {
	handler->setGameState(DIALOGUE);

	DialogueState* state = dynamic_cast<DialogueState*>(handler->getCurrentState());
	state->setTalkingQuester(this);

}