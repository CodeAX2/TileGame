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

	protected:
		// Map of world to an array denoting if each tile contains a static
		static std::map<World*, std::vector<std::vector<bool>>> staticList;

	protected:
		int tX, tY; // The tile x and y coordinates of the static

		bool setSuccessfully = false; // If the static was created successfully

	public:
		// Return if a tile x and y coordinate does not contain a static
		static bool tileIsEmpty(int x, int y, World* world);

	public:
		// Initialize the static, making sure everything is setup correctly
		void init();

		// Get the tile x coordinate
		int getTX() { return tX; }

		// Get the tile y coordinate
		int getTY() { return tY; }

	};

}