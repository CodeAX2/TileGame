#pragma once
#include "Interactable.h"
namespace tg {

	class TreasureChest : public Interactable
	{
	public:
		TreasureChest(int x, int y, Handler* handler, World* world);
		~TreasureChest();

	public:
		void setItemAmount(int itemId, int amount);
		void render(Handler* handler) override;
		std::vector<int> getContents() { return contains; }
		bool contentsAreDefault() { return defaultContents; }

	private:
		std::vector<int> contains;
		bool defaultContents = true;
		sf::Int32 timeAlive = 0;

	protected:
		void dropItems() override;
		void tick(sf::Int32 dt) override;
		void onInteract() override;

	};


}