#pragma once

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "resource.h"
#include <iostream>
#include "Animation.h"
#include <map>
#include "winres.h"

namespace tg {

	class Assets
	{
	private:

		sf::Texture* allTiles[256][256][3];
		sf::Texture* allItems[256];
		sf::Texture* gemTextures[4];
		int renderPriority[256];
		Animation* playerAnimation;
		Animation* zombieAnimation;
		Animation* skeletonAnimation;
		std::string map;
		Animation* treeAnimation;
		Animation* snowyTreeAnimation;
		sf::Texture* tallGrass;
		sf::Texture* boat;
		sf::Texture* pumpkin[2];
		sf::Font ariali;
		sf::Texture* vignette;
		sf::Texture* guiInv;
		int operations = 0;
		sf::Color playerColorP;
		sf::Texture* sandCastle;
		sf::Texture* snowTallGrass;
		sf::Texture* treasureChest;
		sf::Texture* interactPrompt;
		sf::Texture* interactPromptController;
		sf::Texture* workbench;
		sf::Texture* rock;
		sf::Texture* smelter[5];
		sf::Texture* ore[9];
		sf::Texture* lightGFX;
		sf::Texture* torch;
		sf::Shader* shader;
		std::map<int, std::vector<sf::Vector2f>> buildingVerticiesMap;
		sf::Texture* fern;
		std::string version;
		sf::Texture* deathButtons[4];
		sf::Texture* palmTree;
		sf::Texture* questIcon;
		sf::Texture* questIconAvailable;
		sf::Texture* dialogueNPC;

	public:
		Assets();
		~Assets();

	private:
		void setTileInfo(int id, sf::Texture* tile, int priority);
		void addOuterTexture(int id, sf::Texture* text, sf::Uint8 position);
		void loadFull(int base, int bId, int priority);
		void addItemTexture(int base, int bId);
		void addBuildingVerticies(std::pair<int, std::vector<sf::Vector2f>> data);

		std::string loadMapFromResource(int name);
		sf::Font loadFontFromResource(int name);
		sf::Shader* loadShaderFromResource(int name);
		std::string loadVersion(int name);

		void loadWall(int id, int priority, int wallSpot);


	public:

		void init();

		sf::Texture* loadTextureFromResource(int name);
		sf::Image loadImageFromResource(int name);

		float getPercentLoaded() {

			return (float)operations / 1342.f;
		}

		bool priorityHasTile(int priority) {
			if (renderPriority[priority] == -1) {
				return false;
			}

			return true;
		}

		sf::Uint8 getTileWithPriority(int priority) {
			return renderPriority[priority];
		}

		int getTilePriority(sf::Uint8 id) {
			for (int i = 1; i < 256; i++) {
				int cur = renderPriority[i];
				if (cur == id) {
					return i;
				}
			}
			return -1;
		}

		Animation* getTreeAnim() {
			return treeAnimation;
		}

		Animation* getTreeSnowAnim() {
			return snowyTreeAnimation;
		}

		Animation* getPlayerAnim() {
			return playerAnimation;
		}

		std::string getMapData() {
			return map;
		}

		sf::Texture* getTallGrassTexture() {

			return tallGrass;

		}

		sf::Font& getArialiFont() {
			return ariali;
		}

		sf::Texture* getGuiInv() {
			return guiInv;
		}

		sf::Texture* getDeathScreenButton(int buttonId) {
			return deathButtons[buttonId];
		}

		sf::Texture* getVignette() { return vignette; }
		sf::Texture* getInteractPrompt() { return interactPrompt; }
		sf::Texture* getControllerInteractPrompt() { return interactPromptController; }
		sf::Texture* getQuestPrompt() { return questIcon; }
		sf::Texture* getAvaliableQuestPrompt() { return questIconAvailable; }

		sf::Texture* getOuterTexture(sf::Uint8 baseTileId, sf::Uint8 outsideData, int x, int y, bool random = true) {


			// outsideData in form NW,NE,SE,SW,W,N,E,S

			bool needsRendering[8];

			for (int i = 0; i < 8; i++) {
				needsRendering[8 - i - 1] = outsideData >> i & 0x1;
			}

			// Check corners and set properly

			for (int i = 0; i < 4; i++) {
				bool curCorner = needsRendering[i];
				int idA = i + 4;
				int idB = i + 5;
				if (idB >= 8) {
					idB -= 4;
				}

				if (needsRendering[idA] || needsRendering[idB]) {
					needsRendering[i] = false;
				}


			}

			sf::Uint8 pos = needsRendering[0];
			for (int i = 1; i < 8; i++) {
				pos <<= 1;
				pos += needsRendering[i] & 0x1;
			}

			if (pos == 0) return nullptr;

			int diffTextures = 0;
			for (int i = 0; i < 3; i++) {
				if (allTiles[baseTileId][pos][i] != nullptr) {
					diffTextures++;
				}
			}

			srand(x*x + y * y + y * x + 17);

			if (diffTextures == 0 || !random) {
				return allTiles[baseTileId][pos][0];
			}


			return (allTiles[baseTileId][pos][rand() % diffTextures]);

		}

		sf::Texture* getBaseTexture(sf::Uint8 baseTileId) { return allTiles[baseTileId][0][0]; }
		sf::Texture* getItemTexture(sf::Uint8 id) { return allItems[id]; }
		sf::Texture* getRandomGemTexture() {
			return gemTextures[rand() % 4];
		}
		sf::Texture* getBoatTexture() { return boat; }
		sf::Texture* getBuildingTexture(int tId) {
			sf::Texture* bt = loadTextureFromResource(tId);
			return bt;
		}

		sf::Texture* getPumpkinTexture(bool isLit) { return pumpkin[isLit]; }
		sf::Texture* getSandCastleTexture() { return sandCastle; }
		sf::Texture* getTallSnowGrassTexture() { return snowTallGrass; }
		sf::Texture* getTreasureChestTexture() { return treasureChest; }
		sf::Texture* getWorkbenchTexture() { return workbench; }
		sf::Texture* getRockTexture() { return rock; }
		sf::Texture* getSmelterTexture(int state) { return smelter[state]; }
		sf::Texture* getOreTexture(int oreId) { return ore[oreId]; }
		sf::Texture* getTorchTexture() { return torch; }
		sf::Texture* getFernTexture() { return fern; }
		sf::Texture* getPalmTreeTexture() { return palmTree; }
		sf::Texture* getDialogueNPCTexture() { return dialogueNPC; }

		sf::Color getPlayerColor() { return playerColorP; }

		std::string getVersion() { return version; }

		Animation* getZombieAnimation() { return zombieAnimation; }
		Animation* getSkeletonAnimation() { return skeletonAnimation; }

		sf::Color createHSVColor(int hue, float sat, float val);

		sf::Texture* getLightGFX() { return lightGFX; }
		sf::Shader* getShader() { return shader; }
		std::vector<sf::Vector2f> getBuildingVerticies(int buildingTexture) {
			if (buildingVerticiesMap.find(buildingTexture) != buildingVerticiesMap.end())
				return buildingVerticiesMap.at(buildingTexture);
			return std::vector<sf::Vector2f>();
		}


		void setPlayerColor(sf::Color newColor) {
			int red = newColor.r;
			int blue = newColor.b;
			int green = newColor.g;

			red -= 25;
			if (red < 0) red = 0;

			green -= 25;
			if (green < 0) green = 0;

			blue -= 25;
			if (blue < 0) blue = 0;

			playerColorP = newColor;

		}

	};

}