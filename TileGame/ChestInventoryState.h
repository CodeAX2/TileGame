#pragma once
#include "GameState.h"
#include "Handler.h"
#include "TreasureChest.h"

namespace tg {

	class ChestInventoryState : public GameState {
	public:
		ChestInventoryState(Handler* handler);
		~ChestInventoryState();

	private:
		Handler* handler;
		sf::Texture* invBg;
		sf::Texture* invHighlight;
		TreasureChest* openChest = nullptr;
		bool grid[5][9] = {
			{0,1,1,1,1,1,0,0,0},
			{0,1,1,1,1,1,0,0,0},
			{0,1,1,1,1,1,1,1,0},
			{0,1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,1,1}
		};

		const int size = 108 * 1.5;
		const int xSpace = 18 * 1.5, ySpace = 15 * 1.5;
		const int xOffset = 82 * 1.5, yOffsetTop = 25 * 1.5, yOffsetMid = 299 * 1.5, yOffsetBottom = 563 * 1.5;
		int xSlot = -1, ySlot = -1;
		int clickedSlotX = -1, clickedSlotY = -1;

	public:
		void render() override;
		void tick(sf::Int32 dt) override;
		void setOpenChest(TreasureChest* chest) { openChest = chest; }
		void resume() override;
		void pause() override;
		void updateMouse() override;
		void mouseClicked(sf::Event e) override;
		void updateJoystick(sf::Int32 dt) override;

	private:
		void renderBackground();
		void renderItems();
		void renderHighlight();
		void renderItemInfo();
		void swapItems(int itemAx, int itemAy, int itemBx, int itemBy);
		std::pair<int, int> getItemAt(int x, int y);
		std::pair<int, bool> getItemIndex(int x, int y);



	};
}