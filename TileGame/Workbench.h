#pragma once
#include "Static.h"
#include "Interactable.h"

namespace tg {

	class Workbench : public Static, Interactable {
	public:
		Workbench(int x, int y, Handler* handler, World* world);
		~Workbench();

	private:
		sf::Int32 timeAlive = 0; // The amount of time the workbench has been alive

	public:
		// Update the workbench's state
		void tick(sf::Int32 dt) override;

		// Render the workbench to the screen
		void render(Handler* handler) override;

		// Called when the player interacts with the workbench
		void onInteract() override;

		// Drop the workbench's items
		void dropItems() override;

	};

}