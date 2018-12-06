#pragma once
#include "Entity.h"

namespace tg {

	class Rideable :
		public Entity
	{

	protected:
		Entity * rider;
		UUID riderId;


	public:
		Rideable(float x, float y, Handler* handler, int hitBoxX, int hitBoxY, int hitBoxW, int hitBoxH, int w, int h, int type, World* world);
		~Rideable();

	public:
		bool move(float dx, float dy);
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		void setRider(Entity* entity) { rider = entity; entity->setRiding(this); riderId = rider->getId(); }
		sf::IntRect getCollisionBox() override {
			return sf::IntRect(sf::Vector2i(std::round(x + hitBoxX), std::round(y + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		}
		Entity* getRider() { return rider; }
		void setRiderId(UUID id) { riderId = id; }

	protected:
		virtual bool checkForCollision();

	};

}