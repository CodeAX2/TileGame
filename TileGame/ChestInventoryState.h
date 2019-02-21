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

	public:
		void render() override;
		void tick(sf::Int32 dt) override;
		void setOpenChest(TreasureChest* chest) { openChest = chest; }
		void resume() override;
		void pause() override;

	private:
		void renderChestInventory();



	};
}