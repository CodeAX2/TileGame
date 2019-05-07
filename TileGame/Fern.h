#pragma once
#include "Static.h"

namespace tg {

	class Fern :
		public Static {

	public:
		Fern(int x, int y, Handler* handler, World* world);
		~Fern();

	private:
		sf::Int32 timeAlive = 0; // Amount of time the fern has been alive

		float percentToGrow = 0; // The percent chance that the fern will grow when tick() is called

		sf::Int32 timeSinceLastCheck = 0; // The amount of time since the last attempt to grow

		// Minimum amount of time for a fern to grow
		const sf::Int32 initialGrowTime = sf::seconds(180).asMilliseconds();

		// Added to initialGrowTime to calculate the maximum amount of time for a fern to grow
		const sf::Int32 maxGrowTime = sf::seconds(120).asMilliseconds();

		const int triesUntilMax = 15; // Number of attempts to try and grow

	public:
		// Update the fern's state
		void tick(sf::Int32 dt) override;

		// Drop the fern's items
		void dropItems() override;

		// Return the amount of time the fern has been alive
		sf::Int32 getTimeAlive() { return timeAlive; }

		// Return the percent chance to grow
		float getPercentToGrow() { return percentToGrow; }

		// Set the amount of time the fern has been alive
		void setTimeAlive(sf::Int32 time) { timeAlive = time; }

		// Set the percent chance of the fern to grow
		void setPercentToGrow(float percent) { percentToGrow = percent; }

	};

}

