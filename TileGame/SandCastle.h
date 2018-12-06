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
		void dropItems() override;

	};

}