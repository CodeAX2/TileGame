#pragma once
#include "Static.h"

namespace tg {

	class TreasureChest : public Static
	{
	public:
		TreasureChest(int x, int y, Handler* handler, World* world);
		~TreasureChest();

	public:
		void setItemAmount(int itemId, int amount);
		std::vector<int> getContents() { return contains; }
		bool contentsAreDefault() { return defaultContents; }

	private:
		std::vector<int> contains;
		bool defaultContents = true;

	protected:
		void dropItems() override;

	};


}