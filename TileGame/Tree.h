#pragma once
#include "Entity.h"
#include "Assets.h"
#include "Handler.h"
#include "Static.h"

#define T_NORMAL 0
#define T_SNOWY 1
namespace tg {
	class Tree :
		public Static
	{
		
	public:
		Tree(int x, int y, Handler* handler, int type, World* world);
		~Tree();

	private:
		sf::Int32 timeSinceLastAnim = 0; // Amount of time since the last animation

		int frame = 0; // The current animation index
		
		int eType; // The type of the tree

	public:
		// Return the type of the tree
		int getEType() { return eType; }
		void renderLighting(Handler* handler) override;

	protected:
		// Drop the tree's items
		void dropItems() override;

		// Update the tree's state
		void tick(sf::Int32 dt) override;

	};
}
