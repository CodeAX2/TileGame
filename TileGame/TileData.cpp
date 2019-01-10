#include "TileData.h"
#include <bitset>
using namespace tg;

TileData::TileData(sf::Uint8 baseId, std::vector<sf::Uint8> surroundingData, Handler* handler) {
	if (handler == nullptr) {
		return;
	}
	this->baseId = baseId;
	this->handler = handler;

	for (int i = 0; i < 8; i++) {
		sf::Uint8 curId = surroundingData[i];
		//if (curId == -1) continue;

		if (handler->assets->getTilePriority(curId) <= handler->assets->getTilePriority(baseId) && baseId != 8) continue;

		std::map<sf::Uint8, sf::Uint8>::iterator it = adjacentInfo.find(curId);
		if (it == adjacentInfo.end()) {
			// Did not find the key, so lets add it

			sf::Uint8 around = surroundingData[0] == curId;
			for (int j = 1; j < 8; j++) {
				around <<= 1;
				around += surroundingData[j] == curId;
			}

			adjacentInfo[curId] = around;
		}
	}



}

TileData::TileData() {

}


