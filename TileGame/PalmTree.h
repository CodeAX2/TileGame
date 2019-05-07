#pragma once
#include "Static.h"

namespace tg {

	class PalmTree :
		public Static
	{

	public:
		PalmTree(int x, int y, Handler* handler, World* world);
		~PalmTree();

	public:
		// Render the palm tree to the screen
		void render(Handler* handler) override;

		// Drop the palm tree's items
		void dropItems();

	};

}