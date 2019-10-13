#include "Pumpkin.h"
#include "Handler.h"
#include "InputManager.h"
#include "Item.h"
#include "PlayingState.h"

using namespace tg;

Pumpkin::Pumpkin(int x, int y, Handler* handler, bool isLit, World* world) : Static(x, y, handler, 18, 25 * 3, 32 * 3 - 36, 7 * 3, 32 * 3, 32 * 3, false, PUMPKIN_E, 78, world) {
	texture = handler->assets->getPumpkinTexture(isLit);
	health = 60;
	maxHealth = 60;
	this->isLit = isLit;
	lightSize = 100;
	extraLight = 40;
	lightX = this->x + hitBoxX + hitBoxW / 2;
	lightY = this->y + hitBoxY;
	world->getEntityManager()->checkMaxLight(this);

}


Pumpkin::~Pumpkin() {
}

void Pumpkin::render(Handler* handler) {

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

		std::stringstream ss;
		ss << health << "/" << maxHealth;

		healthText.setString(ss.str());
		healthText.setCharacterSize(15);
		healthText.setPosition((int)(x - floor(handler->currentCameraXOffset)) - healthText.getLocalBounds().width / 2 + w / 2, (int)(y - 7 - floor(handler->currentCameraYOffset)));
		if (health > maxHealth || health < 0 || handler == nullptr) return;

		sf::RectangleShape textBg(sf::Vector2f(w, healthText.getGlobalBounds().height + 4));
		textBg.setPosition(sf::Vector2f(x - floor(handler->currentCameraXOffset), healthText.getGlobalBounds().top - 2));
		textBg.setFillColor(sf::Color(0, 0, 0, 140));
		handler->window->draw(textBg);

		handler->window->draw(healthText);

		sf::RectangleShape healthBar(sf::Vector2f(w * (float)health / maxHealth, 10));
		healthBar.setPosition((int)(x - floor(handler->currentCameraXOffset)), (int)(y + 10 - floor(handler->currentCameraYOffset)));
		if (health > maxHealth / 2.f) {
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
		shape2.setPosition(hitBox.left - handler->currentCameraXOffset, hitBox.top - handler->currentCameraYOffset);

		handler->window->draw(shape2);
	}


}

void Pumpkin::dropItems() {
	srand(time(NULL));
	rand(); // Throw out the first value, for safety

	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 3, world);
	if (rand() % 2 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 3, world);
		if (rand() % 2 == 0) {
			new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 3, world);
		}
	}
}

void Pumpkin::renderLighting(Handler* handler) {
	// Draw the lighting if it is lit
	if (isLit) {

		PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
		ps->addLightPoint(sf::Vector2f(lightX - handler->currentCameraXOffset, lightY - handler->currentCameraYOffset), lightSize, extraLight);

	}

	// Draw the entity's shadow
	const sf::Texture* shadowTexture = texture;

	sf::VertexArray shadow(sf::Quads, 4);
	sf::Vector2f basicPosition((int)(x - floor(handler->currentCameraXOffset)), (int)(y - floor(handler->currentCameraYOffset) + h));

	shadow[0].position = basicPosition;
	shadow[1].position = sf::Vector2f(basicPosition.x + shadowLength * sin(shadowDegree * M_PI / 180.f), basicPosition.y - shadowLength * cos(shadowDegree * M_PI / 180.f));
	shadow[2].position = sf::Vector2f(basicPosition.x + w + shadowLength * sin(shadowDegree * M_PI / 180.f), basicPosition.y - shadowLength * cos(shadowDegree * M_PI / 180.f));
	shadow[3].position = sf::Vector2f(basicPosition.x + w, basicPosition.y);

	shadow[0].texCoords = sf::Vector2f(0, shadowTexture->getSize().y);
	shadow[1].texCoords = sf::Vector2f(0, 0);
	shadow[2].texCoords = sf::Vector2f(shadowTexture->getSize().x, 0);
	shadow[3].texCoords = sf::Vector2f(shadowTexture->getSize().x, shadowTexture->getSize().y);

	shadow[0].color = sf::Color(0, 0, 0, 150);
	shadow[1].color = sf::Color(0, 0, 0, 0);
	shadow[2].color = sf::Color(0, 0, 0, 0);
	shadow[3].color = sf::Color(0, 0, 0, 150);

	sf::RenderStates state;
	state.texture = shadowTexture;
	handler->window->draw(shadow, state);

}
