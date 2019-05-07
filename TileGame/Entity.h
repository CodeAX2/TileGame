#pragma once

#include <SFML/Graphics.hpp>
#include "Handler.h"
#include "EntityManager.h"
#include "Camera.h"
#include <math.h>
#include <Windows.h>

// Definitions of the various types of entities
#define NO_TYPE = -1
#define PLAYER_E 0
#define TREE_E 1
#define TALL_GRASS_E 2
#define ITEM_E 3
#define SAND_CASTLE_E 4
#define TREASURE_CHEST_E 5
#define PUMPKIN_E 6
#define BOAT_E 7
#define BUILDING_E 8
#define ZOMBIE_E 9
#define WORKBENCH_E 10
#define ROCK_E 11
#define SMELTER_E 12
#define ORE_E 13
#define TORCH_E 14
#define FERN_E 15
#define SKELETON_E 16
#define PALM_TREE_E 17

namespace tg {
	// Forward declaration of the Rideable class
	class Rideable;

	class Entity {

	public:
		Entity(float x, float y, Handler* handler, int hitBoxX = 0,
			int hitBoxY = 0, int hitBoxW = 0, int hitBoxH = 0, int w = 64,
			int h = 64, bool needsTicking = true, int type = -1,
			bool needsInitialization = true, World* world = nullptr);
		Entity(const Entity* old_obj);

		virtual ~Entity();

	protected:
		UUID uuid; // The entity's unique ID

		float x, y; // The x and y coordinates of the entity

		World* world; // The World the entity is in

		int w, h; // The width and height of the entity texture

		const sf::Texture* texture; // The texture used for rendering

		Handler* handler; // The game handler

		sf::IntRect roundedHitBox; // Rounded version of the entity's hitbox

		int hitBoxX, hitBoxY, hitBoxW, hitBoxH; // Hit box position and size
		// x and y are relative to the position of entity
		// e.g. hitBoxX = 100 would mean the box is at x + 100

		int health = 100; // The entity's health

		int maxHealth = 100; // The maximum health the entity has

		Rideable* ridingOn; // The Rideable this entity is riding on

		bool isARideable = false; // If the entity is also a Rideable

		float lightSize = 0; // The spread of light the entity emits

		float extraLight = 0; // The additional spread of light the entity emits

		float lightX = 0; // The x coordinate of the entity's light

		float lightY = 0; // The y coordinate of the entity's light

	public:
		const bool needsTicking; // If the entity needs to be updated

		const int type; // What type the entity is, using variables defined earlier

	protected:
		// Drop the entity's items
		virtual void dropItems();

		// Set if the entity is a Rideable
		void setRideable(bool isR) { isARideable = isR; }

	public:
		// Render the entity to the screen
		virtual void render(Handler* handler);

		// Update the entity's state
		virtual void tick(sf::Int32 dt);

		// Initialize the entity and it's variables
		virtual void init();

		// Get the entity's unique id
		UUID getId() { return uuid; }

		// Set the entity's unique id
		void setId(UUID id) { uuid = id; }

		// Get the x coordinate of the entity
		float getX() { return x; }

		// Get the y coordinate of the entity
		float getY() { return y; }

		// Get the world the entity is inside
		World* getWorld() { return world; }

		// Set the x and y coordinates of the entity
		virtual void setPos(float x, float y) { this->x = x; this->y = y; }

		// Set the entity's texture for rendering
		virtual void setTexture(const sf::Texture* t) { texture = t; }

		// Get the width of the entity
		int getWidth() { return w; }

		// Get the height of the entity
		int getHeight() { return h; }

		// Get the entity's rounded collision box
		virtual sf::IntRect getCollisionBox() { return roundedHitBox; }

		// Check if this entity is equivalent to another entity
		virtual bool equals(Entity other);

		// Get the entity's health
		int getHealth() { return health; }

		// Get the entity's maximum health
		int getMaxHealth() { return maxHealth; }

		// Set the entity's health
		void setHealth(int newHealth) { health = newHealth; }

		// Set the entity's maximum health
		void setMaxHealth(int newHealth) { maxHealth = newHealth; }

		// Apply damage to the entity
		virtual void damage(int dmg, Entity* damager);

		// Get if the entity is a Rideable
		bool isRideable() { return isARideable; }

		// Set the Rideable this entity is riding on
		void setRiding(Rideable* rideable) { ridingOn = rideable; }

		// Get the Rideable this entity is riding on
		Rideable* getRidingOn() { return ridingOn; }

		// Render the entity's light
		virtual void renderLighting(Handler* handler);

		// Get the spread of the light this entity emits
		float getLightSize() { return lightSize; }

		// Get the additional spread of the light this entity emits
		float getExtraLightSize() { return extraLight; }

		// Get the x position of the entity's light
		float getLightX() { return lightX; }

		// Get the y position of the entity's light
		float getLightY() { return lightY; }

	};

}