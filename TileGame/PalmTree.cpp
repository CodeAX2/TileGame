#include "PalmTree.h"
#include "InputManager.h"
#include "Item.h"

using namespace tg;

PalmTree::PalmTree(int x, int y, Handler* handler, World* world) :
	Static(x, y, handler, 12 * 3, 96 * 3 - 30, 12 * 3, 30, 64 * 3, 96 * 3, false, PALM_TREE_E, 58, world) {

	this->texture = handler->assets->getPalmTreeTexture();
	this->x -= 16 * 3;

	this->health = 120;
	this->maxHealth = 120;

}


PalmTree::~PalmTree() {
}


void PalmTree::render(Handler* handler) {

	// Draw the entity itself
	sf::RectangleShape shape(sf::Vector2f(w, h));
	shape.setTexture(texture);
	shape.setPosition((int)(x - floor(handler->currentCameraXOffset)), (int)(y - floor(handler->currentCameraYOffset)));
	handler->window->draw(shape);


	// Draw the health bar, if health is in [0, maxHealth)
	// The health bar changes color based on the health variable
	if (health < maxHealth && health >= 0) {
		sf::Text healthText;
		healthText.setFont(handler->assets->getArialiFont());

		healthText.setString(std::to_string(health) + "/" + std::to_string(maxHealth));
		healthText.setCharacterSize(15);
		healthText.setPosition((int)(x - floor(handler->currentCameraXOffset)) - healthText.getLocalBounds().width / 2 + w / 2, (int)(y + 53 - floor(handler->currentCameraYOffset)));
		if (health > maxHealth || health < 0 || handler == nullptr) return;

		sf::RectangleShape textBg(sf::Vector2f(32 * 3, healthText.getGlobalBounds().height + 4));
		textBg.setPosition(sf::Vector2f(x - floor(handler->currentCameraXOffset) + w / 2 - 16 * 3, healthText.getGlobalBounds().top - 2));
		textBg.setFillColor(sf::Color(0, 0, 0, 140));
		handler->window->draw(textBg);

		handler->window->draw(healthText);


		sf::RectangleShape healthBar(sf::Vector2f(32 * 3 * (float)health / maxHealth, 10));
		healthBar.setPosition((int)(x - floor(handler->currentCameraXOffset)) + w / 2 - 16 * 3, (int)(y + 70 - floor(handler->currentCameraYOffset)));
		if (health > maxHealth / 2.f) {
			healthBar.setFillColor(sf::Color(
				255 - (float)health / (maxHealth / 2.f) * 255,
				255,
				(float)health / maxHealth * 50,
				180
			));
		}
		else {
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
		shape2.setPosition(hitBox.left - handler->currentCameraXOffset, hitBox.top - handler->currentCameraYOffset);

		handler->window->draw(shape2);
	}


}

void PalmTree::dropItems() {
	srand(time(NULL));
	rand(); // Throw out the first value, for safety

	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 34, world);

	if (rand() % 3 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 34, world);
	}


	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 0, world);
	for (int i = 0; i < 2; i++) {
		if (rand() % 2 == 0) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 0, world);
		}
	}

}