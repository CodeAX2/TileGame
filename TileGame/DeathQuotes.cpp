#include "DeathQuotes.h"

using namespace tg;


std::map<int, std::vector<std::string>> DeathQuotes::quotesMap;

void DeathQuotes::init() {

	std::vector<std::string> zombieQuotes = {
		"\"You've been zomboozled!\"",
		"\"Bad zombie! Bad boy!\"",
		"\"Welcome to the hoard!\"",
		"\"Major OOF!\"",
		"\"RIP\"",
		"\"Don't look now!\"",
		"\"In the words of Nelson Muntz: Ha Ha\"",
		"\"You have fallen to the brain lover!\"",
		"\"Forgot to stay at range?\"",
		"\"Brains... BRAINS!\"",
		"\"You have joined the walking dead!\""
	};
	addQuotesToEntity(ZOMBIE_E, zombieQuotes);




}

void DeathQuotes::addQuotesToEntity(int entity, std::vector<std::string> quotes) {


	if (quotesMap.find(entity) == quotesMap.end()) {
		quotesMap[entity] = quotes;
	} else {
		std::vector<std::string> existingQuotes = quotesMap[entity];
		for (std::string s : quotes) {
			existingQuotes.push_back(s);
		}
		quotesMap[entity] = existingQuotes;
	}


}

std::string DeathQuotes::getRandomDeathQuote(int entity) {

	srand(time(NULL));

	std::vector<std::string> quotes = quotesMap.at(entity);
	return quotes[rand() % quotes.size()];
	return "";
}