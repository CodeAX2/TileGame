#include "Tree.h"
#include "Item.h"
#include "InputManager.h"
#include "PlayingState.h"
#include "Game.h"

using namespace tg;

Tree::Tree(int x, int y, Handler* handler, int type, World* world) :
	Static(x, y, handler, 23, 64 * 3 - 30, 32 * 3 - 49, 30, 32 * 3, 64 * 3, true, TREE_E, 58, world) {

	frame = rand() % handler->assets->getTreeAnim()->getSize();
	timeSinceLastAnim = rand() % 400;

	this->eType = type;

	if (eType == T_NORMAL) {
		texture = handler->assets->getTreeAnim()->getFrame(frame);
	} else if (eType == T_SNOWY) {
		texture = handler->assets->getTreeSnowAnim()->getFrame(frame);
	}

	lightX = x - 1;
	lightY = y - 1;

}


Tree::~Tree()
{
}

// Drop items when the tree is destroyed
void Tree::dropItems() {
	srand(time(NULL));
	rand(); // Throw out the first value, for safety


	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 0, world);
	for (int i = 0; i < 3; i++) {
		if (rand() % 2 == 0) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 0, world);
		}
	}



	if (rand() % 4 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	}


	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 14, world);

	if (rand() % 3 <= 1) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 14, world);
		if (rand() % 3 < 1) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 14, world);
		}
	}

	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 33, world);

	if (rand() % 3 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 33, world);
	}

}

void Tree::tick(sf::Int32 dt) {
	timeSinceLastAnim += dt;
	if (timeSinceLastAnim >= 400) {
		frame++;
		if (frame >= handler->assets->getTreeAnim()->getSize()) {
			frame = 0;
		}
		timeSinceLastAnim = 0;
		if (eType == T_NORMAL) {
			texture = handler->assets->getTreeAnim()->getFrame(frame);
		} else if (eType == T_SNOWY) {
			texture = handler->assets->getTreeSnowAnim()->getFrame(frame);
		}
	}

}

void Tree::renderLighting(Handler* handler) {
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));

	float cX = handler->camera->getXOffset();
	float cY = handler->camera->getYOffset();

	ps->addWallLine(sf::Vector2f(x - cX + 24, y + h - cY), sf::Vector2f(x + w - cX - 27, y + h - cY));
	ps->addBuildingTexture(texture, sf::Vector2f(x - cX, y - cY));

}





