#pragma once
#include "GameState.h"
#include "Handler.h"

namespace tg {
	class InventoryState : public GameState {

	public:
		InventoryState(Handler* handler);
		~InventoryState();


	public:
		void render() override;
		void tick(sf::Int32 dt) override;
		void resume() override;
		void pause() override;
		void updateMouse() override;
		void mouseClicked(sf::Event e) override;
		void updateJoystick(sf::Int32 dt) override;

	private:
		void renderInventory();
		void renderHighlight();
		void swapItems(int fromX, int fromY, int toX, int toY);
		void combineItems(int fromX, int fromY, int toX, int toY);
		int posToId(int posX, int posY);
		void renderItemInfo();

	private:
		sf::Texture* invBg;
		sf::Texture* invHighlight;
		// Represents spots in the GUI where
		// a slot exists
		const int xOffset = 82 * 1.5, yOffset = 176 * 1.5, bottomRowOffset = 563 * 1.5;
		const int size = 108 * 1.5;
		const int xSpace = 18 * 1.5, ySpace = 15 * 1.5;

		int xSlot = -1, ySlot = -1;
		bool grid[4][9] = {
			{0,1,1,1,1,0,0,0,0},
			{0,1,1,1,1,1,1,1,0},
			{0,1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,1,1}
		};

		int clickedSlotX = -1, clickedSlotY = -1;

	private:
		Handler* handler;



	};
}
