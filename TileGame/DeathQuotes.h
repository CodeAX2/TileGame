#pragma once
#include <string>
#include "Entity.h"
#include <map>

namespace tg {

	class DeathQuotes
	{
	public:

		static std::string getRandomDeathQuote(int entity);
		static void init();

	private:
		static std::map<int, std::vector<std::string>> quotesMap;

	private:
		static void addQuotesToEntity(int entity, std::vector<std::string> quotes);

	};

}