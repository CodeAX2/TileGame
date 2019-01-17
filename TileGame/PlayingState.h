#pragma once
#include "GameState.h"
#include "Handler.h"
namespace tg {

	class PlayingState :
		public GameState
	{
	public:
		PlayingState(Handler* handler);
		~PlayingState();

	public:
		void render();
		void tick(sf::Int32 dt);
		void zoom();
		World* getWorld() { return world; }
		void setWorld(World* world) { this->world = world; }
		void setGuiToBottom(bool onBottom) { guiOnBottom = onBottom; }
		void playerDeath() { deathScreen = true; }

	private:
		void renderWorld();
		void renderGUI();
		void renderAllTextures();
		void renderVignette();
		void renderDeathScreen();

	private:
		Handler * handler;
		bool zooming = false;
		float prevZoom;
		World* world;
		bool guiOnBottom = true;
		bool deathScreen = false;
		float deathFade = 0;
		float gameOverFade = 0;



	};

}