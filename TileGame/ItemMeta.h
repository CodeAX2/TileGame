#pragma once
#include <vector>
#include <map>

namespace tg {
	// Basically, this class contains static data about items
	// I.E. how much damage each item does to what entity (axes do more to trees, swords more to monsters)
	class ItemMeta {

	private:
		int id;
		std::map<int, int> entityDamageMap;

	private:
		ItemMeta(int id);

	private:
		void setEntityDamage(int entityType, int damage);

	private:
		static std::map<int, ItemMeta*> allItemMetas;
		static const int BASE_DAMAGE = 20;

	public:
		static void init();
		static int getItemDamage(int entityType, int itemId);


	};
}
