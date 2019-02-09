#pragma once
#include "GameState.h"
#include "Handler.h"
#include <SFML/Audio.hpp>
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
		void playerDeath(std::string msg) { deathScreen = true; deathMessage = msg; }
		void pause() override;
		void resume() override;

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
		std::string deathMessage = "";
		sf::Music bgMusic;


	};

}