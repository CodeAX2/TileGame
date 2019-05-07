#pragma once
#include "Static.h"
#include "Interactable.h"

namespace tg {

	class Smelter : public Static, Interactable {
	public:
		Smelter(int x, int y, Handler* handler, World* world);
		~Smelter();

	private:
		// The base amount of time to smelt an item at burnSpeed = 1.0
		static const int BASE_SMELT_TIME = 30000;

	private:
		sf::Int32 timeAlive = 0; // The amount of time the smelter has been alive

		bool hasFuel = false; // If the smelter has fuel

		float burnSpeed = 0; // The current burn speed, typically based on the fuel

		bool hasSmeltable = false; // If the smelter has a smeltable item

		int smeltingItem = -1; // The current smelting item

		double smeltTimer = 0; // The amount of time left until done smelting

		bool smelting = false; // If the smelter is currently smelting

		bool finished = false; // If the smelter is finished smelting

	public:
		// Update the smelter's state
		void tick(sf::Int32 dt) override;

		// Render the smelter to the screen
		void render(Handler* handler) override;

		// Called when the player interacts with the smelter
		void onInteract() override;

		// Update the smelter's interactable state
		void updateState(Entity* caller) override;

		// Render the smelter's lighting
		void renderLighting(Handler* handler) override;

		// Return if the smelter has fuel
		bool getHasFuel() { return hasFuel; }

		// Return the burn speed of the current fuel
		float getBurnSpeed() { return burnSpeed; }

		// Return if the smelter has a smeltable
		bool getHasSmeltable() { return hasSmeltable; }

		// Return the item being smelted
		int getSmeltingItem() { return smeltingItem; }

		// Return the time left until finished smelting
		double getSmeltTimer() { return smeltTimer; }

		// Return if the smelter is smelting
		bool getIsSmelting() { return smelting; }

		// Return if the smelter is finished smelting
		bool getIsFinished() { return finished; }

		// Set if the smelter has fuel
		void setHasFuel(bool hasFuel) { this->hasFuel = hasFuel; }

		// Set the smelter's burn speed
		void setBurnSpeed(float burnSpeed) { this->burnSpeed = burnSpeed; }

		// Set if the smelter has a smeltable
		void setHasSmeltable(bool hasSmeltable) { this->hasSmeltable = hasSmeltable; }

		// Set the item to be smelted
		void setSmeltingItem(int smeltingItem) { this->smeltingItem = smeltingItem; }
		
		// Set the time until the smelter is done smelting
		void setSmeltTimer(double smeltTimer) { this->smeltTimer = smeltTimer; }
		
		// Set if the smelter is currently smelting		
		void setIsSmelting(bool smelting) { this->smelting = smelting; }
		
		// Set if the smelter is finished smelting
		void setIsFinished(bool finished) { this->finished = finished; }

		// Drop the smelter's items
		void dropItems() override;

	};
}
