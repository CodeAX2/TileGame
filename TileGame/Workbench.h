#pragma once
#include "Static.h"
#include "Interactable.h"

namespace tg {

	class Workbench : public Static, Interactable {
	public:
		Workbench(int x, int y, Handler* handler, World* world);
		~Workbench();

	public:
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		void onInteract() override;
	private:
		sf::Int32 timeAlive = 0;

	};

}