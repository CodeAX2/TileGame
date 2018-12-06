#pragma once
#include "Static.h"
class SandCastle :
	public Static
{
public:
	SandCastle(int x, int y, Handler* handler, World* world);
	~SandCastle();

protected:
	void dropItems() override;

};

