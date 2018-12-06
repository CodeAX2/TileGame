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

		int getEType() { return eType; }

	protected:
		void dropItems() override;
		void tick(sf::Int32 dt) override;

	private:
		sf::Int32 timeSinceLastAnim = 0;
		int frame = 0;
		int eType;



	};
}
