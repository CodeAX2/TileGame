#pragma once

#include <SFML/Graphics.hpp>
#include "Handler.h"
#include "EntityManager.h"
#include "Camera.h"
#include <math.h>
#include <Windows.h>


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

namespace tg {
	class Rideable;

	class Entity
	{
	protected:
		float x, y;
		int w, h;
		const sf::Texture* texture;
		Handler* handler;
		sf::IntRect roundedHitBox;

		int hitBoxX, hitBoxY, hitBoxW, hitBoxH; // x and y are relative to the position of entity
												// e.g. hitBoxX = 100 would mean the box is at x + 100
		int health = 100;
		int maxHealth = 100;
		Rideable* ridingOn;

		World* world;

		bool isARideable = false;
		UUID uuid;

		float lightSize = 0;
		float extraLight = 0;
		float lightX = 0;
		float lightY = 0;

	public:
		const bool needsTicking;
		const int type;


	public:
		Entity(float x, float y, Handler* handler, int hitBoxX = 0,
			int hitBoxY = 0, int hitBoxW = 0, int hitBoxH = 0, int w = 64,
			int h = 64, bool needsTicking = true, int type = -1,
			bool needsInitialization = true, World* world = nullptr);
		Entity(const Entity* old_obj);
		virtual ~Entity();


	public:
		virtual void render(Handler* handler);
		virtual void tick(sf::Int32 dt);
		virtual void init();
		float getX() { return x; }
		float getY() { return y; }
		virtual void setPos(float x, float y) { this->x = x; this->y = y; }
		void setRiding(Rideable* rideable) { ridingOn = rideable; }
		virtual void setTexture(const sf::Texture* t) { texture = t; }
		int getWidth() { return w; }
		int getHeight() { return h; }
		virtual sf::IntRect getCollisionBox() {
			return roundedHitBox;
		}
		virtual bool equals(Entity other);
		int getHealth() { return health; }
		int getMaxHealth() { return maxHealth; }
		virtual void damage(int dmg, Entity* damager);
		bool isRideable() { return isARideable; }
		UUID getId() { return uuid; }
		void setId(UUID id) { uuid = id; }
		void setHealth(int newHealth) { health = newHealth; }
		void setMaxHealth(int newHealth) { maxHealth = newHealth; }
		World* getWorld() { return world; }
		Rideable* getRidingOn() { return ridingOn; }
		virtual void renderLighting(Handler* handler);
		float getLightSize() { return lightSize; }
		float getLightX() { return lightX; }
		float getLightY() { return lightY; }
		float getExtraLightSize() { return extraLight; }

	protected:
		virtual void dropItems();
		void setRideable(bool isR) { isARideable = isR; }

	};

}