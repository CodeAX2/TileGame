#pragma once
#include "Static.h"

namespace tg {

	class Fern :
		public Static {
	public:
		Fern(int x, int y, Handler* handler, World* world);
		~Fern();

	public:
		void tick(sf::Int32 dt) override;
		void dropItems() override;

	public:
		sf::Int32 getTimeAlive() { return timeAlive; }
		float getPercentToGrow() { return percentToGrow; }

		void setTimeAlive(sf::Int32 time) { timeAlive = time; }
		void setPercentToGrow(float percent) { percentToGrow = percent; }

	private:
		sf::Int32 timeAlive = 0;
		float percentToGrow = 0;

	};

}

