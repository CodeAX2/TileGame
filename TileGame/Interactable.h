#pragma once
#include "Handler.h"
#include "World.h"
#include "Static.h"

namespace tg {

	class Interactable : public Static {
	public:
		Interactable(int x, int y, Handler* handler, int hitBoxX, int hitBoxY,
			int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type, float tileOffset, World* world);

		~Interactable();

	public:
		virtual void onInteract();
		void updateState(); // Must call this on every tick
		bool isEnabled() { return enabled; }

	protected:
		bool enabled = false;
	

	};

}