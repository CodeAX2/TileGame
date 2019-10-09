#pragma once
#include "Entity.h"

namespace tg {

	class Rideable :
		public Entity
	{

	public:
		Rideable(float x, float y, Handler* handler, int hitBoxX, int hitBoxY, int hitBoxW, int hitBoxH, int w, int h, int type, World* world);
		~Rideable();

	protected:
		Entity* rider; // The entity riding the rideable
		UUID riderId; // The ID of the entity riding

	public:
		// Move the rideable by dx in the x direction, and dy in the y direction
		bool move(float dx, float dy);

		// Update the state of the rideable
		void tick(sf::Int32 dt) override;

		// Render the rideable to the screen
		void render(Handler* handler) override;

		void renderLighting(Handler* handler) override;

		// Set the current rider
		void setRider(Entity* entity) {
			rider = entity; entity->setRiding(this); riderId = rider->getId();
		}

		// Set the ID of the rider
		// If riderId does not match rider, rider will be changed
		void setRiderId(UUID id) { riderId = id; }

		// Return the collision box of the rideable
		sf::IntRect getCollisionBox() override {
			return sf::IntRect(sf::Vector2i(std::round(x + hitBoxX), std::round(y + hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		}

		// Return the entity riding the rideable
		Entity* getRider() { return rider; }

	protected:
		// Check for a collision with the rideable at the current position
		virtual bool checkForCollision();
		bool shouldRender(Handler* handler);
		bool shouldRenderLight(Handler* handler);

	};

}