#pragma once
#include "Static.h"
#include "Interactable.h"

namespace tg {

	class Smelter : public Static, Interactable {
	public:
		Smelter(int x, int y, Handler* handler, World* world);
		~Smelter();
	};
}
