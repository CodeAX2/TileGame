#pragma once
#include "Static.h"
#include "Interactable.h"
namespace tg {

	class TreasureChest : public Static, public Interactable
	{
	public:
		TreasureChest(int x, int y, Handler* handler, World* world);
		~TreasureChest();

	public:
		void setItem(int itemId, int amount, int spot);
		void render(Handler* handler) override;
		std::vector<std::pair<int, int>> getContents() { return contents; }

		static const int CONTENTS_SIZE = 10;

	private:
		std::vector<std::pair<int, int>> contents;
		sf::Int32 timeAlive = 0;

	protected:
		void dropItems() override;
		void tick(sf::Int32 dt) override;
		void onInteract() override;

	};


}