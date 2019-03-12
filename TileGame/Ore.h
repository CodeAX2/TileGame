#pragma once
#include "Static.h"


namespace tg {
	class Ore : public Static {

	public:
		static const int COAL = 0, GEM = 1, GOLD = 2, IRON = 3, PLATINUM = 4,
			SILVER = 5, STEEL = 6, TITANIUM = 7, VIBRANIUM = 8;

	private:
		static const int oreToItem[9];


	public:
		Ore(int x, int y, Handler* handler, World* world, int oreType);
		~Ore();

	private:
		int oreType;
		bool regening = false;
		sf::Int32 cooldown = 0;

	public:
		void dropItems() override;
		void damage(int dmg, Entity* damager) override;
		void tick(sf::Int32 dt) override;
		
		int getOreType() { return oreType; }
		bool isRegening() { return regening; }
		sf::Int32 getCooldown() { return cooldown; }

		void setOreType(int oreType) { this->oreType = oreType; }
		void setIsRegening(bool regening) { this->regening = regening; }
		void setCooldown(sf::Int32 cooldown) { this->cooldown = cooldown; }

	};

}