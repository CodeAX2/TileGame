#pragma once
#include "Static.h"

namespace tg {

	class Pumpkin :
		public Static
	{

	private:
		bool isLit;

	public:
		Pumpkin(int x, int y, Handler* handler, bool isLit, World* world);
		~Pumpkin();

	public:
		bool getIsLit() { return isLit; }
		void setLit(bool l) {
			isLit = l;
			texture = handler->assets->getPumpkinTexture(isLit);
		}

		void render(Handler* handler) override;
		void dropItems();
		void renderLighting(Handler* handler) override;


	};

}