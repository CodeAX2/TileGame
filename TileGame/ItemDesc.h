#pragma once

#include <map>
#include <string>

class ItemDesc
{
public:
	static void init();
	static std::string getInfo(int id);

private:
	static std::map<int, std::string> itemDescriptions;


};

