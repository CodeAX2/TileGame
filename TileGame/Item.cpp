#include "Item.h"
#include "Player.h"

using namespace tg;

Item::Item(float x, float y, Handler* handler, int itemId, World* world) : Entity(x, y, handler, 0, 0, 0, 0, 64, 64, true, ITEM_E, true, world) {


	if (itemId == 1) {

		texture = handler->assets->getRandomGemTexture();

	} else {

		texture = handler->assets->getItemTexture(itemId);
	}

	timeAlive = 0;
	waveOffset = rand();
	this->itemId = itemId;
}


Item::~Item()
{
}

// Item update loop
void Item::tick(sf::Int32 dt) {

	timeAlive += dt;
	offsetHeight = sin(timeAlive / 200.f + waveOffset) * 6 + 3;
	float dX = handler->player->getX() - x;
	float dY = handler->player->getY() + handler->player->getHeight() - handler->player->getCollisionBox().height - y - 32;

	if (timeAlive < 500) {
		return;
	}

	EntityManager* em = world->getEntityManager();

	if (dX * dX + dY * dY <= 150 * 150) {
		if (dX * dX + dY * dY <= 25 * 25) {
			if (itemId == 2) {
				if (!handler->player->inventoryContains(itemId)) {
					handler->player->addItemToInv(itemId);
					em->removeEntity(this);
					return;
				} else {
					em->removeEntity(this);
					return;
				}
			}
			handler->player->addItemToInv(itemId);
			em->removeEntity(this);
			return;
		} else {
			float cX = dX / 300 * dt * 4000.f / (dX*dX + dY * dY);
			float cY = dY / 300 * dt * 4000.f / (dX*dX + dY * dY);
			x += cX;
			y += cY;
		}

	}

	em->fixEntityMoved(this);
}

// Render the item
void Item::render(Handler* handler) {
	sf::RectangleShape shape(sf::Vector2f(w, h));
	shape.setTexture(texture);
	shape.setPosition((int)(x - floor(handler->camera->getXOffset())), (int)(y - floor(handler->camera->getYOffset()) + offsetHeight));

	handler->window->draw(shape);


}