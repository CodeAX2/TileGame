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
		void render(Handler* handler) override;
		void dropItems();



	};

}