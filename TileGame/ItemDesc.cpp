#include "ItemDesc.h"


std::map<int, std::string> ItemDesc::itemDescriptions;

void ItemDesc::init() {
	itemDescriptions[0] =
		"LOG:\n"
		" Used for crafting tools\n"
		" and other various things.";

	itemDescriptions[1] = 
		"GEM:\n"
		" Used as a form of currency\n"
		" and not fully implemented yet.";

	itemDescriptions[2] =
		"AXE:\n"
		" Used to effeciently cut\n"
		" down trees for logs.";

	itemDescriptions[3] =
		"CANDY:\n"
		" Eat it to restore 10\n"
		" of your stamina points.";

	itemDescriptions[4] =
		"CAVE CANDY:\n"
		" Eat it to restore 15\n"
		" of your stamina points.";

	itemDescriptions[5] =
		"WOOD BRIDGE:\n"
		" Place it on some water\n"
		" and walk across it.";

}

std::string ItemDesc::getInfo(int id) {
	return itemDescriptions[id];
}
