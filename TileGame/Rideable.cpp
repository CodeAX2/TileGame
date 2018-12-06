#include "Rideable.h"
#include "InputManager.h"

using namespace tg;

Rideable::Rideable(float x, float y, Handler* handler, int hitBoxX, int hitBoxY, 
	int hitBoxW, int hitBoxH, int w, int h, int type, World* world) : 
	Entity(x, y, handler, hitBoxX, hitBoxY, hitBoxW, hitBoxH, w, h, true, type, true, world) {

	setRideable(true);
}


Rideable::~Rideable()
{
}

void Rideable::tick(sf::Int32 dt) {

	if (rider != nullptr) {
		if (rider->getId() != riderId) {
			// Change the rider
			rider = world->getEntityManager()->getEntityById(riderId);
			rider->setRiding(this);
		}
		rider->setPos(x - rider->getWidth() / 2 + w / 2, y - rider->getHeight() + h / 2 + 20);
	} else if (riderId != UUID()) {
		// Change the rider
		rider = world->getEntityManager()->getEntityById(riderId);
		rider->setRiding(this);
	}
}

void Rideable::render(Handler* handler) {

	// Draw the entity itself
	sf::RectangleShape shape(sf::Vector2f(w, h));
	shape.setTexture(texture);
	shape.setPosition((int)(x - floor(handler->camera->getXOffset())), (int)(y - floor(handler->camera->getYOffset())));
	handler->window->draw(shape);


	// Draw the health bar, if health is in [0, maxHealth)
	// The health bar changes color based on the health variable
	if (health < maxHealth && health >= 0) {
		sf::Text healthText;
		sf::Font f = *(handler->assets->getArialiFont());
		healthText.setFont(f);

		std::stringstream ss;
		ss << health << "/" << maxHealth;

		healthText.setString(ss.str());
		healthText.setCharacterSize(15);
		healthText.setPosition((int)(x - floor(handler->camera->getXOffset())) - healthText.getLocalBounds().width / 2 + w / 2, (int)(y + 23 - floor(handler->camera->getYOffset())));
		if (health > maxHealth || health < 0 || handler == nullptr) return;

		sf::RectangleShape textBg(sf::Vector2f(w, healthText.getGlobalBounds().height + 4));
		textBg.setPosition(sf::Vector2f(x - floor(handler->camera->getXOffset()), healthText.getGlobalBounds().top - 2));
		textBg.setFillColor(sf::Color(0, 0, 0, 140));
		handler->window->draw(textBg);

		handler->window->draw(healthText);


		sf::RectangleShape healthBar(sf::Vector2f(w * (float)health / maxHealth, 10));
		healthBar.setPosition((int)(x - floor(handler->camera->getXOffset())), (int)(y + 40 - floor(handler->camera->getYOffset())));
		if (health >= maxHealth / 2.f) {
			healthBar.setFillColor(sf::Color(
				255 - (float)health / (maxHealth / 2.f) * 255,
				255,
				(float)health / maxHealth * 50,
				180
			));
		} else {
			healthBar.setFillColor(sf::Color(
				255,
				(float)health / (maxHealth * .6f) * 255,
				(float)health / maxHealth * 50,
				180
			));
		}
		handler->window->draw(healthBar);
	}


	// Draw the hitbox if hitboxes are being shown
	if (handler->inputManager->hitboxIsShown()) {
		sf::IntRect hitBox = getCollisionBox();

		sf::RectangleShape shape2(sf::Vector2f(hitBox.width, hitBox.height));
		shape2.setFillColor(sf::Color(255, 0, 0, 150));
		shape2.setPosition(hitBox.left - handler->camera->getXOffset(), hitBox.top - handler->camera->getYOffset());

		handler->window->draw(shape2);
	}


	if (rider != nullptr) {
		rider->render(handler);
	}
}


bool Rideable::move(float dx, float dy) {

	if (checkForCollision()) {
		if (rider != nullptr)
			rider->setRiding(nullptr);
		rider = nullptr;
		world->getEntityManager()->removeEntity(this);
		return false;
	}

	bool movedX = true, movedY = true;

	x += dx;
	if (checkForCollision()) {
		x -= dx;
		movedX = false;
	}

	y += dy;
	if (checkForCollision()) {
		y -= dy;
		movedY = false;
	}

	world->getEntityManager()->fixEntityMoved(this);

	if (movedX || movedY) {
		return true;
	}


	return false;



}

bool Rideable::checkForCollision() {
	sf::IntRect pBox = getCollisionBox();

	int cX = x + hitBoxX, cY = y + hitBoxY;

	if (cX < 0 || cX + hitBoxW >= world->getWidth() * 96 ||
		cY < 0 || cY + hitBoxH >= world->getHeight() * 96) {
		return true;
	}


	if (world->tileIsSolid(pBox.left, pBox.top) ||
		world->tileIsSolid(pBox.left + pBox.width, pBox.top) ||
		world->tileIsSolid(pBox.left + pBox.width, pBox.top + pBox.height) ||
		world->tileIsSolid(pBox.left, pBox.top + pBox.height)) {
		return true;
	}


	EntityManager* em = world->getEntityManager();

	for (int i = 0; i < em->numEntities(); i++) {

		Entity* cur = em->getEntity(i);

		if (cur == this || cur == rider) {
			continue;
		}

		sf::IntRect eBox = cur->getCollisionBox();

		if (eBox.intersects(pBox)) {
			return true;
		}

	}

	return false;
}

