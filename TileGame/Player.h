#pragma once
#include "Entity.h"
#include "Assets.h"
#include "Handler.h"
#include <SFML/Graphics/Rect.hpp>
#include <math.h>
#include "Inventory.h"
#include "Interactable.h"

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
		int facingVert = SOUTH;
		int facingHori = STILL;
		sf::RectangleShape atBox;
		Inventory* inventory;
		bool settingNewPos = false;
		float newX = 0, newY = 0;
		int maxStam = 100;
		float stam = maxStam;
		sf::Int32 stamRegenCooldown = 0;
		bool slowRegen = false;
		int maxMagic = 100;
		int magic = maxMagic;
		float hue = 0;
		int hotBarSlot = 0;
		Interactable* curInteracting = nullptr;

	public:
		Player(float x, float y, Handler* handler, World* world);
		~Player();

	public:
		void render(Handler* handler);
		void renderLighting(Handler* handler);
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

		Inventory* getInventory() { return inventory; }

		sf::IntRect getCollisionBox() {
			return sf::IntRect(sf::Vector2i(std::round(x + hitBoxX), std::round(y + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		}

		void setInventory(Inventory* newInv) {
			delete inventory;
			inventory = newInv;
		}
		void damage(int dmg, Entity* damager) override;

		int getStamina() { return stam; }
		int getMaxStamina() { return maxStam; }
		int getMagic() { return magic; }
		int getMaxMagic() { return maxMagic; }
		bool stamIsRegeningSlowly() { return slowRegen; }

		void setStamina(int stamina) { stam = stamina; }
		void setMaxStamina(int maxStamina) { maxStam = maxStamina; }
		void setMagic(int magic) { this->magic = magic; }
		void setMaxMagic(int maxMagic) { this->maxMagic = maxMagic; }
		void setStamIsRegeningSlowly(bool isRegeningSlow) { slowRegen = isRegeningSlow; }
		int getHotBarSlot() { return hotBarSlot; }
		void setHotBarSlot(int slot) { hotBarSlot = slot; }
		std::pair<int, int> getItemInfoInHotBar() { return inventory->getInventory()[hotBarSlot]; }
		void removeItemFromHotbar();

		int getFacingVert() { return facingVert; }
		int getFacingHori() { return facingHori; }

		Interactable* getCurrentInteracting() { return curInteracting; }
		void setCurrentInteracting(Interactable* interactable) { curInteracting = interactable; }

		void interact();
		void dropItems() override;
		
	private:
		bool checkForCollision();
		void hitEntities();


	};
}
