#pragma once
#include "Static.h"
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

	void render(Handler* handler);
	void dropItems();


};

