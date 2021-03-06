#include "Torch.h"
#include "PlayingState.h"
#include "Item.h"

using namespace tg;

Torch::Torch(int x, int y, Handler* handler, World* world) :
	Static(x, y, handler, 45, 32 * 3 - 6, 6, 6, 32 * 3, 32 * 3, false, TORCH_E, 55, world) {

	texture = handler->assets->getTorchTexture();
	health = 60;
	maxHealth = 60;


	lightSize = 200;
	extraLight = 100;
	lightX = this->x + w / 2;
	lightY = this->y + h / 2;

	world->getEntityManager()->checkMaxLight(this);

}


Torch::~Torch()
{
}


void Torch::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 32, world);
}