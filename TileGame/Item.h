#pragma once
#include "Entity.h"

namespace tg {

	class Item :
		public Entity
	{

	public:
		Item(float x, float y, Handler* handler, int itemId, World* world);
		~Item();

	private:
		sf::Int32 timeAlive = 0; // How long the item has been alive

		int offsetHeight = 0; // The height offset of the item for rendering

		sf::Int32 waveOffset; // The offset of time for the bobbing of the item

		int itemId; // The item ID of the item for rendering


	public:
		// Update the state of the item
		void tick(sf::Int32 dt);

		// Render the item to the screen
		void render(Handler* handler);

		// Return the item ID of the item
		int getItemId() { return itemId; }


	};

}