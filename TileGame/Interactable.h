#pragma once
#include "Handler.h"
#include "World.h"

namespace tg {

	class Interactable {
	public:
		Interactable(Handler* handler);
		~Interactable();

	protected:
		bool enabled = false; // If the interactable is in range to be interacted with

	private:
		Handler* interactHandler; // Interactable version of the game handler

	public:
		// Called when the player interacts with the interactable
		virtual void onInteract();

		// Update the state of the interactable. Must be called from the base entity's tick function
		virtual void updateState(Entity* caller);

		// Return if the interactable is able to be interacted with
		bool isEnabled() { return enabled; }

	};

}