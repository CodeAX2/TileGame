#pragma once
#include "Entity.h"
#include "Assets.h"
#include "Handler.h"
#include <SFML/Graphics/Rect.hpp>
#include <math.h>

#define STILL 0
#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
namespace tg {
	class Player :
		public Entity
	{

	private:
		sf::Int32 timeSinceLastAnim = 0;
		int curAnim = 0;
		bool moving = false;
		double speed = 0.1;
		sf::Int32 timeSinceAttackStart = 0;
		bool attacking = false;
		int vertDirection = STILL;
		int horiDirection = STILL;
		sf::RectangleShape atBox;
		std::map<sf::Uint8, int> inventory;
		bool settingNewPos = false;
		float newX = 0, newY = 0;

	public:
		Player(float x, float y, Handler* handler, World* world);
		~Player();

	public:
		void render(Handler* handler);
		void tick(sf::Int32 dt);
		bool isAttacking() { return attacking; }
		bool previousAttackKey = false;
		void addItemToInv(int itemId);
		void removeItemFromInv(int itemId);
		bool inventoryContains(int itemId);

		void setPos(float x, float y) override {
			this->newX = x; this->newY = y;
			settingNewPos = true;
		}

		void setWorld(World* world) { this->world = world; }

		std::map<sf::Uint8, int> getInventory() { return inventory; }

		sf::IntRect getCollisionBox() {
			return sf::IntRect(sf::Vector2i(std::round(x + hitBoxX), std::round(y + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		}

		void setInventory(std::map<sf::Uint8, int> newInv) { inventory = newInv; }

	private:
		bool checkForCollision();
		void hitEntities();


	};
}
