#pragma once
#include "Static.h"


namespace tg {

	class Rock : public Static {
	public:
		Rock(int x, int y, Handler* handler, World* world);
		~Rock();

	public:
		void dropItems() override; 


	};



}
