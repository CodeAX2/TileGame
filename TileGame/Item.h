#pragma once
#include "Entity.h"

namespace tg {

	class Item :
		public Entity
	{

	private:
		sf::Int32 timeAlive = 0;
		int offsetHeight = 0;
		sf::Int32 waveOffset;
		int itemId;

	public:
		Item(float x, float y, Handler* handler, int itemId, World* world);
		~Item();

	public:
		void tick(sf::Int32 dt);
		void render(Handler* handler);

		int getItemId() { return itemId; }


	};

}