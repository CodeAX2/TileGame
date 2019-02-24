#pragma once
#include "Handler.h"
#include "World.h"

namespace tg {

	class Interactable {
	public:
		Interactable(Handler* handler);
		~Interactable();

	public:
		virtual void onInteract();
		void updateState(Entity* caller); // Must call this on every tick
		bool isEnabled() { return enabled; }

	protected:
		bool enabled = false;

	private:
		Handler* interactHandler;
	

	};

}