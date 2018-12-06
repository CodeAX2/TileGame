#pragma once
#include "Entity.h"

namespace tg {

	class Static :
		public Entity
	{
	public:
		Static(int x, int y, Handler* handler, int hitBoxX, int hitBoxY,
			int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type, float tileOffset, World* world);
		~Static();

		void init();

		int getTX() { return tX; }
		int getTY() { return tY; }

	public:
		static bool tileIsEmpty(int x, int y, World* world);

	private:
		static std::map<World*, std::vector<std::vector<bool>>> staticList;
		int tX, tY;
		bool setSuccessfully = false;

	};

}