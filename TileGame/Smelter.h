#pragma once
#include "Static.h"
#include "Interactable.h"

namespace tg {

	class Smelter : public Static, Interactable {
	public:
		Smelter(int x, int y, Handler* handler, World* world);
		~Smelter();

	public:
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		void onInteract() override;
		void updateState(Entity* caller) override;

		bool getHasFuel() { return hasFuel; }
		float getBurnSpeed() { return burnSpeed; }
		bool getHasSmeltable() { return hasSmeltable; }
		int getSmeltingItem() { return smeltingItem; }
		double getSmeltTimer() { return smeltTimer; }
		bool getIsSmelting() { return smelting; }
		bool getIsFinished() { return finished; }

		void setHasFuel(bool hasFuel) { this->hasFuel = hasFuel; }
		void setBurnSpeed(float burnSpeed) { this->burnSpeed = burnSpeed; }
		void setHasSmeltable(bool hasSmeltable) { this->hasSmeltable = hasSmeltable; }
		void setSmeltingItem(int smeltingItem) { this->smeltingItem = smeltingItem; }
		void setSmeltTimer(double smeltTimer) { this->smeltTimer = smeltTimer; }
		void setIsSmelting(bool smelting) { this->smelting = smelting; }
		void setIsFinished(bool finished) { this->finished = finished; }

		void dropItems() override;

	private:
		sf::Int32 timeAlive = 0;
		bool hasFuel = false;
		float burnSpeed = 0;

		bool hasSmeltable = false;
		int smeltingItem = -1;

		double smeltTimer = 0;
		bool smelting = false;

		bool finished = false;

	private:
		static const int BASE_SMELT_TIME = 30000;

	};
}
