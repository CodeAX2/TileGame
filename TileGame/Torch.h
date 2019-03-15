#pragma once
#include "Static.h"

namespace tg {
	class Torch : public Static {
	public:
		Torch(int x, int y, Handler* handler, World* world);
		~Torch();

	public:
		void dropItems() override;

	};

}