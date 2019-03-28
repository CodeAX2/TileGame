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
		void playBGMusic() { bgMusic.play(); musicPaused = false; }
		void pauseBGMusic() { bgMusic.pause(); musicPaused = true; }
		void mouseClicked(sf::Event e) override;
		void updateMouse() override;
		void updateJoystick(sf::Int32 dt) override;
		bool musicIsPaused() { return musicPaused; }
		void setDarkness(float darknessPercent) { this->darknessPercent = darknessPercent; }
		sf::RenderTexture* getLightRenderer() { return &renderTexture; }
		float getDarknessPercent() { return darknessPercent; }
		void addLightPoint(sf::Vector2f point, float spread, float extraDist);
		void addWallLine(sf::Vector2f pointA, sf::Vector2f pointB);
		void addBuildingTexture(const sf::Texture* texture, sf::Vector2f pos);

	private:
		void renderWorld();
		void renderGUI();
		void renderAllTextures();
		void renderVignette();
		void renderDeathScreen();
		void renderTime();

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
		sf::Texture* hotBarSlotHighlight;
		bool musicPaused = false;
		sf::Int32 time = 540000 - 5000;
		sf::RenderTexture renderTexture;
		float darknessPercent = 0;
		sf::Texture* lightT;

		std::vector<sf::Glsl::Vec4> lightArr;
		std::vector<sf::Glsl::Vec4> wallArr;
		std::vector<const sf::Texture*> bldgTextureArr;
		std::vector<sf::Vector2f> bldgPosArr;

	};

}