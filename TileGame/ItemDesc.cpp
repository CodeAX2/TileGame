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
		" and for crafting.";

	itemDescriptions[2] =
		"METAL AXE:\n"
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

	itemDescriptions[6] =
		"METAL SWORD:\n"
		" Use this weapon to\n"
		" effectively kill monsters.";

	itemDescriptions[7] =
		"WOODEN SWORD:\n"
		" Use this weapon to\n"
		" damage and kill monsters.";

	itemDescriptions[8] =
		"CHEST:\n"
		" Stores a variety of\n"
		" items. Contains 10 slots.";

	itemDescriptions[9] =
		"WOODEN PICKAXE:\n"
		" Use to mine up rocks\n"
		" in order to make better tools.";

	itemDescriptions[10] =
		"STONE:\n"
		" Used for crafting \n"
		" improved tools and weapons.";

	itemDescriptions[11] =
		"WORKBENCH:\n"
		" Required for crafting \n"
		" anything under the sun.";

	itemDescriptions[12] =
		"COAL:\n"
		" Used as a fuel \n"
		" in the smelter.";

	itemDescriptions[13] =
		"SMELTER:\n"
		" Used to smelt certain\n"
		" items into other items.";

	itemDescriptions[14] =
		"PINE NEEDLES:\n"
		" Used to create\n"
		" various types of medicine.";

	itemDescriptions[15] =
		"PINE SALVE:\n"
		" Used it to restore\n"
		" 30 of your lost health.";

	itemDescriptions[16] =
		"GOLD ORE:\n"
		" Smelt it to turn\n"
		" it into a gold ingot.";

	itemDescriptions[17] =
		"IRON ORE:\n"
		" Smelt it to turn\n"
		" it into an iron ingot.";

	itemDescriptions[18] =
		"PLATINUM ORE:\n"
		" Smelt it to turn\n"
		" it into a platinum ingot.";

	itemDescriptions[19] =
		"SILVER ORE:\n"
		" Smelt it to turn\n"
		" it into a silver ingot.";

	itemDescriptions[20] =
		"STEEL ORE:\n"
		" Smelt it to turn\n"
		" it into a steel ingot.";

	itemDescriptions[21] =
		"TITANIUM ORE:\n"
		" Smelt it to turn\n"
		" it into a titanium ingot.";

	itemDescriptions[22] =
		"VIBRANIUM ORE:\n"
		" How did this\n"
		" find its way here?";

	itemDescriptions[23] =
		"GOLD INGOT:\n"
		" Highly valuable\n"
		" material, but not very strong.";

	itemDescriptions[24] =
		"IRON INGOT:\n"
		" Versatile material\n"
		" used in a variety of ways.";

	itemDescriptions[25] =
		"PLATINUM INGOT:\n"
		" Incredibly valuable\n"
		" material, and very durable.";

	itemDescriptions[26] =
		"SILVER INGOT:\n"
		" Used in a variety\n"
		" of circuits and technology.";

	itemDescriptions[27] =
		"STEEL INGOT:\n"
		" Very durable and strong\n"
		" material, great for tools.";

	itemDescriptions[28] =
		"TITANIUM INGOT:\n"
		" Incredibly durable material\n"
		" used only for the best tools.";

	itemDescriptions[29] =
		"VIBRANIUM INGOT:\n"
		" Again, how did\n"
		" this find its way here?";

}

std::string ItemDesc::getInfo(int id) {
	return itemDescriptions[id];
}
