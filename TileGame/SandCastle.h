#pragma once
#include "Static.h"

namespace tg {

	class SandCastle :
		public Static
	{
	public:
		SandCastle(int x, int y, Handler* handler, World* world);
		~SandCastle();

	protected:
		// Drop the sand castle's items
		void dropItems() override;

	};

}