#pragma once
#include "Quester.h"
#include "Path.h"

namespace tg {

	class Villager :
		public Quester {
	public:
		Villager(float x, float y, Handler* handler, World* world, std::string pathFileName);
		~Villager();

	private:
		Path path;
		int curPathIndex = 0;
		sf::Int32 waitTime = 0;


	protected:
		void onCollisionWithFollowing(sf::Int32 dt) override;

	public:
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		void damage(int dmg, Entity* damager) override;
		void onInteract() override;


	};

}

