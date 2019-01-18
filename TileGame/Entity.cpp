#include "Entity.h"
#include "InputManager.h"
#include <sstream>
#include "Item.h"
#include "Static.h"

using namespace tg;

// Constructors
Entity::Entity(

	float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type,
	bool needsInitialization, World* world

) :

	x(x), y(y),
	hitBoxX(hitBoxX), hitBoxY(hitBoxY), hitBoxW(hitBoxW), hitBoxH(hitBoxH),
	handler(handler), w(w), h(h), needsTicking(needsTicking), type(type), world(world)

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
		textBg.setFillColor(sf::Color(0,0,0,140));
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


}

// The update loop for the entity
void Entity::tick(sf::Int32 dt) {
}

// Check to see if two entities are equal
// They are equal if their xs, ys, and types are the same
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



	return true;

}


// Apply some damage to the entity, and remove it if health
// is below 0
void Entity::damage(int dmg, Entity* damager) {
	health -= dmg;
	if (health <= 0) {
		dropItems();
		world->getEntityManager()->removeEntity(this);
	}

}

// Drop the items when an entity dies
void Entity::dropItems() {

}
