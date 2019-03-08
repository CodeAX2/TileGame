#pragma once
#include "GameState.h"
#include "Handler.h"


namespace tg {

	class CraftingState :
		public GameState {

	public:
		CraftingState(Handler* handler);
		~CraftingState();

		void tick(sf::Int32 dt) override;
		void render() override;
		//void exit() override;
		//void pause() override;
		void resume() override;
		void mouseClicked(sf::Event e) override;
		void updateMouse() override;
		//void updateJoystick(sf::Int32 dt) override;

	private:
		void renderBackground();
		void renderHighlight();
		void renderIcons();
		void renderItems();
		void renderCraftables();
		void renderItemInfo();
		bool playerHasIngredients();
		void craftSelectedItem();

		std::pair<int, int> getItemAt(int x, int y);
		void swapItems(int itemAx, int itemAy, int itemBx, int itemBy);
		int getItemIndex(int x, int y);

	private:
		Handler* handler;
		sf::Texture* craftingGui;
		sf::Texture* upArrow;
		sf::Texture* downArrow;
		sf::Texture* deny;
		sf::Texture* allow;
		sf::Texture* invHighlight;

		bool grid[5][9] = {
			{1,1,1,1,1,0,0,0,0},
			{1,1,1,1,1,0,0,0,0},
			{0,1,1,1,1,1,1,1,0},
			{0,1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,1,1}
		};

		const int size = 108;
		const int xSpace = 18, ySpace = 15;
		const int xOffset = 82, yOffsetTop = 25, yOffsetMid = 299, yOffsetBottom = 563;
		const int yOffsetIngredients = 31, xOffsetIngredients = 831, xIngredientSpace = 15, yIngredientSpace = 27;
		int xSlot = -1, ySlot = -1;
		int clickedSlotX = -1, clickedSlotY = -1;
		int selectedCraftSlotX = -1, selectedCraftSlotY = -1;

		bool hoverUp = false, hoverDown = false;
		bool hoverCraft = false;
		bool canCraftSelected = false;
	};

}