#include "Entity.h"
#include "InputManager.h"
#include <sstream>
#include "Item.h"
#include "Static.h"
#include "PlayingState.h"
#include <chrono>
#include <sstream>
#include "Game.h"
#include "Pumpkin.h"

using namespace tg;

// Constructors
Entity::Entity(

	float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type,
	bool needsInitialization, World* world

) :

	x(x), y(y),
	hitBoxX(hitBoxX), hitBoxY(hitBoxY), hitBoxW(hitBoxW), hitBoxH(hitBoxH),
	handler(handler), w(w), h(h), needsTicking(needsTicking), type(type), world(world),
	shadowLength(h)

{
	if (this->world == nullptr) {
		this->world = handler->mainWorld;
	}
	roundedHitBox = sf::IntRect(sf::Vector2i(std::round(x + hitBoxX), std::round(y + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
	if (needsInitialization) {
		init();
	}

	UuidCreate(&uuid); // Creates a uuid

}

Entity::Entity(const Entity* old_obj) : needsTicking(old_obj->needsTicking), type(old_obj->type) {
	x = old_obj->x;
	y = old_obj->y;
	w = old_obj->w;
	h = old_obj->h;
	texture = old_obj->texture;
	handler = old_obj->handler;
	hitBoxH = old_obj->hitBoxH;
	hitBoxW = old_obj->hitBoxW;
	hitBoxX = old_obj->hitBoxX;
	hitBoxY = old_obj->hitBoxY;
	health = old_obj->health;
	roundedHitBox = old_obj->roundedHitBox;
}

// Destructor
Entity::~Entity() {
}

// Initializing function, only called if the entity needs initializing
void Entity::init() {
	world->getEntityManager()->addEntity(this);
}

// Render the entity to the screen
void Entity::render(Handler* handler) {

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
		healthText.setPosition((int)(x - floor(handler->currentCameraXOffset)) - healthText.getLocalBounds().width / 2 + w / 2, (int)(y + 23 - floor(handler->currentCameraYOffset)));
		if (health > maxHealth || health < 0 || handler == nullptr) return;

		sf::RectangleShape textBg(sf::Vector2f(32 * 3, healthText.getGlobalBounds().height + 4));
		textBg.setPosition(sf::Vector2f(x - floor(handler->currentCameraXOffset) + w / 2 - 16 * 3, healthText.getGlobalBounds().top - 2));
		textBg.setFillColor(sf::Color(0, 0, 0, 140));
		handler->window->draw(textBg);

		handler->window->draw(healthText);


		sf::RectangleShape healthBar(sf::Vector2f(32 * 3 * (float)health / maxHealth, 10));
		healthBar.setPosition((int)(x - floor(handler->currentCameraXOffset)) + w / 2 - 16 * 3, (int)(y + 40 - floor(handler->currentCameraYOffset)));
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

// The update loop for the entity
void Entity::tick(sf::Int32 dt) {
}

// Check to see if two entities are equal
// They are equal if their xs, ys, types, and ids are the same
bool Entity::equals(Entity other) {
	if (fabs(other.x - x) >= .5f) {
		return false;
	}
	if (fabs(other.y - y) >= .5f) {
		return false;
	}
	if (other.type != type) {
		return false;
	}
	if (other.uuid != uuid) {
		return false;
	}


	return true;

}


// Apply some damage to the entity, and remove it if health
// is below 0
void Entity::damage(int dmg, Entity* damager) {
	health -= dmg;
	if (health <= 0) {
		dropItems();
		world->getEntityManager()->removeEntity(this, true);
	}

}

// Drop the items when an entity dies
void Entity::dropItems() {

}

void Entity::renderLighting(Handler* handler) {

	const sf::Texture* shadowTexture = texture;
	// Draw the entity's shadow

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

	shadow[0].color = sf::Color(0, 0, 0, 125);
	shadow[1].color = sf::Color(0, 0, 0, 50);
	shadow[2].color = sf::Color(0, 0, 0, 50);
	shadow[3].color = sf::Color(0, 0, 0, 125);

	sf::RenderStates state;
	state.texture = shadowTexture;
	handler->window->draw(shadow, state);

	if (lightSize != 0 && extraLight != 0) {

		PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
		ps->addLightPoint(sf::Vector2f(lightX - handler->currentCameraXOffset, lightY - handler->currentCameraYOffset), lightSize, extraLight);
	}
}

bool Entity::shouldRender(Handler* handler) {
	sf::View v = handler->window->getView();
	bool onScreen = !(getX() + getWidth() < handler->currentCameraXOffset + (handler->window->getSize().x / 2 - v.getSize().x / 2) ||
		getX() > handler->window->getSize().x + handler->currentCameraXOffset - (handler->window->getSize().x / 2 - v.getSize().x / 2) ||
		getY() + getHeight() < handler->currentCameraYOffset + (handler->window->getSize().y / 2 - v.getSize().y / 2) ||
		getY() > handler->window->getSize().y + handler->currentCameraYOffset - (handler->window->getSize().y / 2 - v.getSize().y / 2));

	return onScreen;

}

bool Entity::shouldRenderLight(Handler* handler) {
	sf::View v = handler->window->getView();

	float viewModifierX = handler->window->getSize().x / 2 - v.getSize().x / 2;
	float viewModifierY = handler->window->getSize().y / 2 - v.getSize().y / 2;
	float xOffset = handler->currentCameraXOffset;
	float yOffset = handler->currentCameraYOffset;
	int windowWidth = handler->window->getSize().x;
	int windowHeight = handler->window->getSize().y;

	bool lightOnScreen = !(lightX + lightSize + extraLight < xOffset + viewModifierX ||
		lightX - lightSize - extraLight > windowWidth + xOffset - viewModifierX ||
		lightY + lightSize + extraLight < yOffset + viewModifierY ||
		lightY - lightSize - extraLight > windowHeight + yOffset - viewModifierY);

	bool shadowOnScreen = !(x + w + shadowLength < xOffset + viewModifierX ||
		x - shadowLength > windowWidth + xOffset - viewModifierX ||
		y + h + shadowLength < yOffset + viewModifierY ||
		y - shadowLength > windowHeight + yOffset - viewModifierY);

	return lightOnScreen || shadowOnScreen;

}

void Entity::setWorld(World* world) {
	World* prevWorld = this->world;
	this->world = world;

	// Remove the entity from the previous world, but don't delete it (since it's in a new world)
	prevWorld->getEntityManager()->removeEntity(this, false);
	this->world->getEntityManager()->addEntity(this);

}