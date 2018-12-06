#include "Assets.h"



// Creates the playerColor
Assets::Assets() {
}

// Initialize EVERYTHING!
void Assets::init() {

	for (int i = 0; i < 256; i++) {
		renderPriority[i] = -1; // Declare each spot in the priorities as empty
	}

	///// Create all the tiles /////
	const std::string baseFileDir = "F:\\Coding\\C++\\Projects\\TileGame\\TileGame\\Resources\\Textures\\";

	// Load all the tiles
	loadFull(baseFileDir + "Grass24.png", 0, 7);
	loadFull(baseFileDir + "Rock24.png", 1, 8);
	loadFull(baseFileDir + "Sand24.png", 3, 5);
	loadFull(baseFileDir + "Wood24.png", 4, 2);
	loadFull(baseFileDir + "Path24.png", 5, 3);
	loadFull(baseFileDir + "CaveFloor24.png", 6, 4);
	loadFull(baseFileDir + "Snow24.png", 7, 6);

	// Set the id of each tile that does not have a full texture
	sf::Texture* waterTile = new sf::Texture();
	waterTile->loadFromFile(baseFileDir + "WaterTile.png");
	setTileInfo(2, waterTile, 1);

}

// Check if a certain tile priority has a tile assigned to it
bool Assets::priorityHasTile(int priority) {
	if (renderPriority[priority] == -1) {
		return false;
	}

	return true;
}

// Get the priority of a tile id
int Assets::getTilePriority(sf::Uint8 id) {
	for (int i = 0; i < 256; i++) {
		int cur = renderPriority[i];
		if (cur == id) {
			return i;
		}
	}

	return -1;

}

// Get the proper texture based on the surrounding data, and the base tile
const sf::Texture * Assets::getOuterTexture(sf::Uint8 baseTileId, sf::Uint8 outsideData, int x, int y, bool random) {


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

// Delete EVERYTHING!
Assets::~Assets() {

	delete allTiles[256][256][3];
}

// Set the id, texture, and priority of a tile
void Assets::setTileInfo(int id, const sf::Texture* tile, int priority) {

	// Add each tile to the array of all tiles, at the position equal to the id
	allTiles[id][0][0] = tile;
	renderPriority[priority] = id;

}

// Create a specific borderig texture, of the possible 256
void Assets::addOuterTexture(int id, const sf::Texture* text, sf::Uint8 position) {
	for (int i = 0; i < 3; i++) {
		if (allTiles[id][position][i] == nullptr) {
			allTiles[id][position][i] = text;
			return;
		}
	}
}

// Load the entirety of a tile, given its id
void Assets::loadFull(std::string fileLocation, int bId, int priority) {


	// Load the basic tile info
	sf::Texture* tile = new sf::Texture();

	sf::IntRect rect;
	rect.width = 32;
	rect.height = 32;
	rect.left = 32 * 2;
	rect.top = 32 * 3;

	tile->loadFromFile(fileLocation, rect);
	setTileInfo(bId, tile, priority);

	// Load the given textures

	sf::Uint8 order[24] = {
		0b00001000,0b00000100,0b00000010,0b00000001,
		0b00001100,0b00000110,0b00000011,0b00001001,
		0b00001110,0b00000111,0b00001011,0b00001101,
		0b00001111,0b11110000,0b00000000,0b00000000,
		0b00001000,0b00000100,0b00000010,0b00000001,
		0b00001000,0b00000100,0b00000010,0b00000001
	};
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			sf::Uint8 curSpot = order[i * 4 + j];
			if (i * 4 + j >= 24 || curSpot == 0) continue;

			sf::IntRect rect2;
			rect2.width = 32;
			rect2.height = 32;
			rect2.left = 32 * j;
			rect2.top = 32 * i;

			sf::Texture* t = new sf::Texture();
			t->loadFromFile(fileLocation, rect2);

			addOuterTexture(bId, t, curSpot);


		}
	}

	// Load the opposing sides textures

	sf::Image o1 = allTiles[bId][0b00001000][0]->copyToImage();
	o1.copy(allTiles[bId][0b00000010][0]->copyToImage(), 16, 0, sf::IntRect(16, 0, 16, 32));
	sf::Texture* ot1 = new sf::Texture();
	ot1->loadFromImage(o1);
	addOuterTexture(bId, ot1, 0b00001010);

	sf::Image o2 = allTiles[bId][0b00000100][0]->copyToImage();
	o2.copy(allTiles[bId][0b00000001][0]->copyToImage(), 0, 16, sf::IntRect(0, 16, 32, 16));
	sf::Texture* ot2 = new sf::Texture();
	ot2->loadFromImage(o2);
	addOuterTexture(bId, ot2, 0b00000101);




	// Load all the corner stuff
	for (sf::Uint8 i = 0; i <= 255; i++) {
		if (allTiles[bId][i][0] == nullptr) {
			sf::Texture* nT = new sf::Texture();
			sf::Uint8 edgeId = i;
			edgeId <<= 4;
			edgeId >>= 4;

			sf::Uint8 cornerId = i;
			cornerId >>= 4;

			sf::Image nI;
			const sf::Texture* edgeBase = getOuterTexture(bId, edgeId, 0, 0, false);
			if (edgeBase != nullptr) {
				nI = edgeBase->copyToImage();
			} else {
				nI.create(32, 32, sf::Color(0, 0, 0, 0));
			}


			bool cornerData[4]; // NW, NE, SE, SW
			for (int i = 0; i < 4; i++) {
				cornerData[3 - i] = 0x1 & cornerId >> i;
			}

			sf::Image cornerImageBase = getOuterTexture(bId, 0b11110000, 0, 0, false)->copyToImage();
			if (cornerData[0]) {
				nI.copy(cornerImageBase, 0, 0, sf::IntRect(0, 0, 16, 16));
			}
			if (cornerData[1]) {
				nI.copy(cornerImageBase, 16, 0, sf::IntRect(16, 0, 16, 16));
			}
			if (cornerData[2]) {
				nI.copy(cornerImageBase, 16, 16, sf::IntRect(16, 16, 16, 16));
			}
			if (cornerData[3]) {
				nI.copy(cornerImageBase, 0, 16, sf::IntRect(0, 16, 16, 16));
			}

			nT->loadFromImage(nI);

			allTiles[bId][i][0] = nT;

		}

		if (i == 255) {
			break;
		}

	}



}

