#pragma once
#include "Static.h"

namespace tg {

	class Pumpkin :
		public Static
	{

	public:
		Pumpkin(int x, int y, Handler* handler, bool isLit, World* world);
		~Pumpkin();

	private:
		bool isLit; // If the pumpkin is lit up

	public:
		// Return if the pumpkin is lit
		bool getIsLit() { return isLit; }

		// Set if the pumpkin is lit
		void setLit(bool l) {
			isLit = l;
			texture = handler->assets->getPumpkinTexture(isLit);
		}

		// Render the pumpkin to the screen
		void render(Handler* handler) override;

		// Drop the pumpkin's items
		void dropItems();

		// Render the pumpkin's lighting
		void renderLighting(Handler* handler) override;

	};

}