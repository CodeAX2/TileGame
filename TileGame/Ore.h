#pragma once
#include "Static.h"


namespace tg {
	class Ore : public Static {

	public:
		Ore(int x, int y, Handler* handler, World* world, int oreType);
		~Ore();

	public:
		// Ore type constants
		static const int COAL = 0, GEM = 1, GOLD = 2, IRON = 3, PLATINUM = 4,
			SILVER = 5, STEEL = 6, TITANIUM = 7, VIBRANIUM = 8;

	private:
		static const int oreToItem[9]; // Array mapping each ore type constant to the item it drops

	private:
		int oreType; // The type of ore used for rendering and dropping items
		bool regening = false; // If the ore is regenerating its resource
		sf::Int32 cooldown = 0; // Time left until the ore is done regenrating its resource

	public:
		// Drop the ore's respective item
		void dropItems() override;

		// Apply damage to the ore
		void damage(int dmg, Entity* damager) override;

		// Update the ore's state
		void tick(sf::Int32 dt) override;
		
		// Return the type of the ore
		int getOreType() { return oreType; }

		// Return if the ore is regenerating its resource
		bool isRegening() { return regening; }

		// Return the time until the ore is done regenerating its resource
		sf::Int32 getCooldown() { return cooldown; }

		// Set the type of the ore
		void setOreType(int oreType) { this->oreType = oreType; }

		// Set if the ore is regenerating its resource
		void setIsRegening(bool regening) { this->regening = regening; }

		// Set the remaining time until the ore is done regenerating its resource
		void setCooldown(sf::Int32 cooldown) { this->cooldown = cooldown; }

	};

}