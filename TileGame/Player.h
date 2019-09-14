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

	public:
		Player(float x, float y, Handler* handler, World* world);
		~Player();

	private:

		sf::Int32 timeSinceLastAnim = 0; // Time since the animation was updated

		int curAnim = 0; // The current animation index used for rendering

		bool moving = false; // If the player is moving

		double speed = 0.1; // The speed the player is moving at

		sf::Int32 timeSinceAttackStart = 0; // Time since the player began attacking

		bool attacking = false; // If the player is currently attacking

		bool previousAttackKey = false; // If the attack key was previously activated

		int vertDirection = STILL; // The direction the player is moving along the y axis

		int horiDirection = STILL; // The direction the player is moving along the x axis

		int facingVert = SOUTH; // The direction the player is facing along the y axis

		int facingHori = STILL; // The direction the player is facing along the x axis

		sf::RectangleShape atBox; // The hitbox used for attacking other entites

		Inventory* inventory; // The player's inventory instance

		bool settingNewPos = false; // If the player's x or y are changing

		float newX = 0, newY = 0; // The new x and y coordinates of the player

		int maxStam = 100; // The maximum stamina the player has

		float stam = (float)maxStam; // The current amount of stamina the player has

		sf::Int32 stamRegenCooldown = 0; // The cooldown for regenerating stamina

		bool slowRegen = false; // If the stamina is regenerating slowly

		int maxMagic = 100; // The maximum magic the player has

		int magic = maxMagic; // The current amount of magic the player has

		float hue = 0; // The hue to render the player at

		int hotBarSlot = 0; // The current hotbar slot the player has selected

		Interactable* curInteracting = nullptr; // If the player is interacting with an entity

	public:
		// Render the player to the screen
		void render(Handler* handler);

		// Render the lighting from the player
		void renderLighting(Handler* handler);

		// Update the state of the player
		void tick(sf::Int32 dt);

		// Return if the player is currently attacking
		bool isAttacking() { return attacking; }
		
		// Add a single item of ID itemId to the inventory
		void addItemToInv(int itemId);

		// Remove a single item of ID itemId from the inventory
		void removeItemFromInv(int itemId);
		
		// Check if the inventory contains an item of ID itemId
		bool inventoryContains(int itemId);

		// Set the position of the player
		void setPos(float x, float y) override {
			this->newX = x; this->newY = y;
			settingNewPos = true;
		}

		// Set the current world the player is in
		void setWorld(World* world) { this->world = world; }

		// Return the instance of the player's inventory
		Inventory* getInventory() { return inventory; }

		// Return the collision box of the player
		sf::IntRect getCollisionBox() {
			return sf::IntRect(sf::Vector2i(std::round(x + hitBoxX), std::round(y + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		}

		// Set the player's inventory to a new inventory instance
		void setInventory(Inventory* newInv) {
			delete inventory;
			inventory = newInv;
		}

		// Apply damage to the player
		void damage(int dmg, Entity* damager) override;

		// Return the current amount of stamina the player has
		int getStamina() { return stam; }

		// Return the maximum amount of stamina the player has
		int getMaxStamina() { return maxStam; }

		// Return the current amount of magic the player has
		int getMagic() { return magic; }

		// Return the maximum amount of stamina the player has
		int getMaxMagic() { return maxMagic; }

		// Return if the player's stamina is regenerating slowly
		bool stamIsRegeningSlowly() { return slowRegen; }

		// Set the current amount of stamina
		void setStamina(int stamina) { stam = stamina; }

		// Set the maximum amount of stamina
		void setMaxStamina(int maxStamina) { maxStam = maxStamina; }

		// Set the current amount of magic
		void setMagic(int magic) { this->magic = magic; }

		// Set the maximum amount of magic
		void setMaxMagic(int maxMagic) { this->maxMagic = maxMagic; }

		// Set if the stamina is regenerating slowly
		void setStamIsRegeningSlowly(bool isRegeningSlow) { slowRegen = isRegeningSlow; }
		
		// Return the currently selected hot bar slot
		int getHotBarSlot() { return hotBarSlot; }

		// Set the currently selected hot bar slot
		void setHotBarSlot(int slot) { hotBarSlot = slot; }

		// Return the item info within the current hotbar slot
		std::pair<int, int> getItemInfoInHotBar() { return inventory->getInventory()[hotBarSlot]; }
		
		// Remove 1 item from the current hot bar slot
		void removeItemFromHotbar();

		// Return the direction the player is facing along the y axis
		int getFacingVert() { return facingVert; }

		// Return the direction the player is facing along the x axis
		int getFacingHori() { return facingHori; }

		// Return the interactable entity the player is currently interacting with
		Interactable* getCurrentInteracting() { return curInteracting; }

		// Set the entity the player is currently interacting with
		void setCurrentInteracting(Interactable* interactable) { curInteracting = interactable; }

		// Call the onInteract() function of the entity the player is currently interacting with
		void interact();

		// Drop the inventory contents
		void dropItems() override;
		
	private:
		// Check for a collision at the current position
		bool checkForCollision();

		// Damage any entities colliding with the player's attack box
		void hitEntities();

	};
}
