#include "Assets.h"

using namespace tg;

sf::Texture* Assets::loadTextureFromResource(int name) {
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

	sf::Texture* image = new sf::Texture();
	if (!image->loadFromMemory(firstByte, rsrcDataSize))
		throw std::runtime_error("Failed to load image from memory.");



	return image;
}


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
	ariali = sf::Font(loadFontFromResource(ARIALI));

	srand(time(NULL));

	sf::Uint8 red = rand() % (256 - 25) + 25;
	sf::Uint8 green = rand() % (256 - 25) + 25;
	sf::Uint8 blue = rand() % (256 - 25) + 25;


	playerColorP = sf::Color(red, green, blue, 255);
}

void Assets::init() {

	// Load the map
	map = loadMapFromResource(MAP);
	operations++;

	// Load the gui
	guiInv = loadTextureFromResource(GUI_INV);
	operations++;

	// Load other textures
	vignette = loadTextureFromResource(VIGNETTE);
	operations++;

	lightGFX = loadTextureFromResource(LIGHT_GFX);

	interactPrompt = loadTextureFromResource(INTERACT_PROMPT);
	operations++;

	interactPromptController = loadTextureFromResource(INTERACT_PROMPT_CONTROLLER);
	operations++;

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
		tree->loadFromImage(snowyTreeSheet, sf::IntRect(i * 32, 0, 32, 64));
		snowyTreeAnimation->addFrame(tree);
		operations++;
	}


	tallGrass = loadTextureFromResource(TALL_GRASS);
	operations++;

	boat = loadTextureFromResource(BOAT);
	operations++;

	pumpkin[0] = loadTextureFromResource(PUMPKIN);

	pumpkin[1] = loadTextureFromResource(PUMPKIN_LIT);

	sandCastle = loadTextureFromResource(SAND_CASTLE);

	snowTallGrass = loadTextureFromResource(TALL_GRASS_SNOW);

	treasureChest = loadTextureFromResource(TREASURE_CHEST);

	workbench = loadTextureFromResource(WORKBENCH);

	rock = loadTextureFromResource(ROCK);

	sf::Image smelterSheet = loadImageFromResource(SMELTER);
	for (int i = 0; i < 5; i++) {
		smelter[i] = new sf::Texture();
		smelter[i]->loadFromImage(smelterSheet, sf::IntRect(i * 32, 0, 32, 32));
	}

	for (int i = 0; i < 9; i++) {
		ore[i] = loadTextureFromResource(ROCK_COAL + i);
	}


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
	// ID 8 - 15 is used for walls
	for (int i = 8; i < 18; i++) {
		loadWall(i, i + 9, i - 8);
	}

	// ID 18 is used for blank tiles

	// Load all the items
	addItemTexture(LOG_ITEM, 0);
	addItemTexture(AXE_ITEM, 2);
	addItemTexture(CANDY_ITEM, 3);
	addItemTexture(CAVE_CANDY_ITEM, 4);
	addItemTexture(WOOD_BRIDGE_ITEM, 5);
	addItemTexture(METAL_SWORD_ITEM, 6);
	addItemTexture(WOOD_SWORD_ITEM, 7);
	addItemTexture(TREASURE_CHEST_ITEM, 8);
	addItemTexture(WOOD_PICKAXE_ITEM, 9);
	addItemTexture(STONE_ITEM, 10);
	addItemTexture(WORKBENCH_ITEM, 11);
	addItemTexture(COAL_ITEM, 12);
	addItemTexture(PINE_NEEDLES_ITEM, 14);
	addItemTexture(PINE_SALVE_ITEM, 15);
	addItemTexture(WOOD_AXE_ITEM, 30);
	addItemTexture(METAL_PICKAXE_ITEM, 31);

	for (int i = 0; i < 4; i++) {
		sf::Texture* curGem = new sf::Texture();
		curGem->loadFromImage(loadImageFromResource(GEM_SHEET), sf::IntRect(i * 32, 0, 32, 32));
		gemTextures[i] = curGem;
	}

	allItems[1] = gemTextures[0];
	allItems[13] = smelter[0];

	sf::Image oreItemSheet = loadImageFromResource(ORE_SHEET);
	for (int i = 0; i < 7; i++) {
		allItems[16 + i] = new sf::Texture();
		allItems[16 + i]->loadFromImage(oreItemSheet, sf::IntRect(32 * i, 0, 32, 32));
	}

	sf::Image ingotItemSheet = loadImageFromResource(INGOT_SHEET);
	for (int i = 0; i < 7; i++) {
		allItems[23 + i] = new sf::Texture();
		allItems[23 + i]->loadFromImage(ingotItemSheet, sf::IntRect(32 * i, 0, 32, 32));
	}


	waterTile->loadFromImage(loadImageFromResource(WATER_TILE));





	// Set the id of each tile that does not have a full texture
	setTileInfo(2, waterTile, 1);

	playerAnimation = new Animation();

	sf::Image img = loadImageFromResource(PLAYER_SHEET);

	for (int i = 0; i < 18; i++) {
		sf::Texture* curPlayerAnim = new sf::Texture();

		curPlayerAnim->loadFromImage(img,
			sf::IntRect(20 * (i % 12), 39 * (i / 12), 20, 39));
		playerAnimation->addFrame(curPlayerAnim);
		operations++;
	}

	zombieAnimation = new Animation();
	sf::Image zombieSheet;
	zombieSheet = loadImageFromResource(ZOMBIE_SHEET);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++) {
			sf::Texture* cur = new sf::Texture();
			cur->loadFromImage(zombieSheet, sf::IntRect(32 * j, 32 * i, 32, 32));
			zombieAnimation->addFrame(cur);
			operations++;
		}
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

	item = loadTextureFromResource(base);
	allItems[bId] = item;
	operations++;

}

void Assets::loadWall(int id, int priority, int wallSpot) {

	sf::Image fullWallSheet = loadImageFromResource(WALL_TILE_SHEET);
	sf::Texture* curWallText = new sf::Texture();

	if (wallSpot == 0) {
		curWallText->loadFromImage(fullWallSheet, sf::IntRect(0, 0, 32, 64));
	} else if (wallSpot >= 1 && wallSpot <= 3) {
		curWallText->loadFromImage(fullWallSheet, sf::IntRect(32 * wallSpot, 0, 32, 32));
	} else if (wallSpot >= 4 && wallSpot <= 7) {
		curWallText->loadFromImage(fullWallSheet, sf::IntRect(32 * (wallSpot - 3), 32, 32, 32));
	} else {
		curWallText->loadFromImage(fullWallSheet, sf::IntRect(32 * (wallSpot - 4), 0, 32, 32));
	}
	allTiles[id][0][0] = curWallText;
	renderPriority[priority] = id;
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


sf::Color Assets::createHSVColor(int hue, float sat, float val) {
	hue %= 360;
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h) {
	default:
	case 0:
	case 6: return sf::Color(val * 255, t * 255, p * 255);
	case 1: return sf::Color(q * 255, val * 255, p * 255);
	case 2: return sf::Color(p * 255, val * 255, t * 255);
	case 3: return sf::Color(p * 255, q * 255, val * 255);
	case 4: return sf::Color(t * 255, p * 255, val * 255);
	case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}
