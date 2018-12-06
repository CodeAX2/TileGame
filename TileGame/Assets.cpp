#include "Assets.h"



sf::Image Assets::loadImageFromResource(int name) {
	HRSRC rsrcData = FindResource(NULL, MAKEINTRESOURCE(name), "PNG");
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
	if (rsrcDataSize <= 0)
		throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	sf::Image image;
	if (!image.loadFromMemory(firstByte, rsrcDataSize))
		throw std::runtime_error("Failed to load image from memory.");



	return image;
}

std::string Assets::loadMapFromResource(int name) {
	HRSRC rsrcData = FindResource(NULL, MAKEINTRESOURCE(name), "TEXT");
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
	if (rsrcDataSize <= 0)
		throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	return std::string(static_cast<const char *>(firstByte));
}

sf::Font Assets::loadFontFromResource(int name) {

	HRSRC rsrcData = FindResource(NULL, MAKEINTRESOURCE(name), "TTF");
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
	if (rsrcDataSize <= 0)
		throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	sf::Font font;
	if (!font.loadFromMemory(firstByte, rsrcDataSize))
		throw std::runtime_error("Failed to font image from memory.");

	return font;

}

Assets::Assets() {
	ariali = new sf::Font(loadFontFromResource(ARIALI));

	srand(time(NULL));

	sf::Uint8 red = rand() % (256 - 25) + 25;
	sf::Uint8 green = rand() % (256 - 25) + 25;
	sf::Uint8 blue = rand() % (256 - 25) + 25;


	playerColorP = sf::Color(red, green, blue, 255);
	playerColorS = sf::Color(red - 25, green - 25, blue - 25, 255);
}

void Assets::init() {

	// Load the map
	map = loadMapFromResource(MAP);
	operations++;

	// Load the gui
	guiInv = new sf::Texture();
	guiInv->loadFromImage(loadImageFromResource(GUI_INV));
	operations++;

	// Load other textures
	treeAnimation = new Animation();

	sf::Image treeSheet = loadImageFromResource(TREE_SHEET);
	for (int i = 0; i < 6; i++) {
		sf::Texture* tree = new sf::Texture();
		tree->loadFromImage(treeSheet, sf::IntRect(i * 32, 0, 32, 64));
		treeAnimation->addFrame(tree);
		operations++;
	}

	snowyTreeAnimation = new Animation();

	sf::Image snowyTreeSheet = loadImageFromResource(TREE_SNOW_SHEET);
	for (int i = 0; i < 6; i++) {
		sf::Texture* tree = new sf::Texture();
		tree->loadFromImage(treeSheet, sf::IntRect(i * 32, 0, 32, 64));
		snowyTreeAnimation->addFrame(tree);
		operations++;
	}


	tallGrass = new sf::Texture();
	tallGrass->loadFromImage(loadImageFromResource(TALL_GRASS));
	operations++;

	boat = new sf::Texture();
	boat->loadFromImage(loadImageFromResource(BOAT));

	operations++;

	pumpkin[0] = new sf::Texture();
	pumpkin[0]->loadFromImage(loadImageFromResource(PUMPKIN));

	pumpkin[1] = new sf::Texture();
	pumpkin[1]->loadFromImage(loadImageFromResource(PUMPKIN_LIT));

	sandCastle = new sf::Texture();
	sandCastle->loadFromImage(loadImageFromResource(SAND_CASTLE));

	snowTallGrass = new sf::Texture();
	snowTallGrass->loadFromImage(loadImageFromResource(TALL_GRASS_SNOW));

	treasureChest = new sf::Texture();
	treasureChest->loadFromImage(loadImageFromResource(TREASURE_CHEST));


	for (int i = 0; i < 256; i++) {
		renderPriority[i] = -1; // Declare each spot in the priorities as empty
	}

	// Create all the tiles
	sf::Texture* waterTile = new sf::Texture();

	// Load all the tiles
	loadFull(GRASS_TILE, 0, 6);
	loadFull(ROCK_TILE, 1, 8);
	loadFull(SAND_TILE, 3, 4);
	loadFull(WOOD_TILE, 4, 2);
	loadFull(PATH_TILE, 5, 3);
	loadFull(CAVE_FLOOR_TILE, 6, 5);
	loadFull(SNOW_TILE, 7, 7);

	// Load all the items
	addItemTexture(LOG_ITEM, 0);
	addItemTexture(AXE_ITEM, 2);
	addItemTexture(CANDY_ITEM, 3);
	addItemTexture(CAVE_CANDY_ITEM, 4);

	for (int i = 0; i < 4; i++) {
		sf::Texture* curGem = new sf::Texture();
		curGem->loadFromImage(loadImageFromResource(GEM_SHEET), sf::IntRect(i * 32, 0, 32, 32));
		gemTextures[i] = curGem;
	}

	allItems[1] = gemTextures[0];


	waterTile->loadFromImage(loadImageFromResource(WATER_TILE));





	// Set the id of each tile that does not have a full texture
	setTileInfo(2, waterTile, 1);

	playerAnimation = new Animation();

	sf::Image img = loadImageFromResource(PLAYER_SHEET);

	for (int x = 0; x < img.getSize().x; x++) {
		for (int y = 0; y < img.getSize().y; y++) {

			if (img.getPixel(x, y) == sf::Color(255, 255, 255, 255)) {
				img.setPixel(x, y, playerColorP);
			} else if (img.getPixel(x, y) == sf::Color(230, 230, 230, 255)) {
				img.setPixel(x, y, playerColorS);
			}
		}
	}

	for (int i = 0; i < 18; i++) {
		sf::Texture* curPlayerAnim = new sf::Texture();

		curPlayerAnim->loadFromImage(img,
			sf::IntRect(20 * (i % 12), 39 * (i / 12), 20, 39));
		playerAnimation->addFrame(curPlayerAnim);
		operations++;
	}

	std::cout << "GFX OPERATIONS: " << operations << std::endl;
}

Assets::~Assets() {

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			for (int k = 0; k < 3; k++) {
				delete allTiles[i][j][k];
			}
		}
	}

	delete playerAnimation;

	for (int i = 0; i < 256; i++) {
		delete allItems[i];
	}
	delete treeAnimation;
	delete snowyTreeAnimation;
	delete tallGrass;
	delete guiInv;
	delete ariali;

}

void Assets::setTileInfo(int id, sf::Texture* tile, int priority) {

	// Add each tile to the array of all tiles, at the position equal to the id
	operations++;
	allTiles[id][0][0] = tile;
	renderPriority[priority] = id;

}

void Assets::addOuterTexture(int id, sf::Texture* text, sf::Uint8 position) {
	operations++;
	for (int i = 0; i < 3; i++) {
		if (allTiles[id][position][i] == nullptr) {
			allTiles[id][position][i] = text;
			return;
		}
	}
}

void Assets::addItemTexture(int base, int bId) {

	sf::Texture* item = new sf::Texture();
	sf::IntRect rect;
	rect.width = 32;
	rect.height = 32;
	rect.left = 0;
	rect.top = 0;

	item->loadFromImage(loadImageFromResource(base), rect);
	allItems[bId] = item;
	operations++;

}

void Assets::loadFull(int base, int bId, int priority) {


	// Load the basic tile info
	sf::Texture* tile = new sf::Texture();

	sf::IntRect rect;
	rect.width = 32;
	rect.height = 32;
	rect.left = 32 * 2;
	rect.top = 32 * 3;

	tile->loadFromImage(loadImageFromResource(base), rect);
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
			t->loadFromImage(loadImageFromResource(base), rect2);

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
			sf::Texture* edgeBase = getOuterTexture(bId, edgeId, 0, 0, false);
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
			operations++;

			allTiles[bId][i][0] = nT;

		}

		if (i == 255) {
			break;
		}

	}



}


