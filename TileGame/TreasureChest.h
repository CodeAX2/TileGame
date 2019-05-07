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
		static const int CONTENTS_SIZE = 10; // The amount of slots within a chest

	private:
		std::vector<std::pair<int, int>> contents; // The item contents of the chest

		sf::Int32 timeAlive = 0; // The amount of time alive

	public:
		// Set the item type and amount of a slot in the chest
		void setItem(int itemId, int amount, int spot);

		// Render the chest to the screen
		void render(Handler* handler) override;

		// Return the chest's contents
		std::vector<std::pair<int, int>> getContents() { return contents; }

	protected:
		// Drop the chest's items
		void dropItems() override;

		// Update the chest's state
		void tick(sf::Int32 dt) override;

		// Called when the player interacts with the chest
		void onInteract() override;

	};


}