#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

int main() {

	string fileName;
	cout << "Enter a name for the building file: " << flush;
	getline(cin, fileName);

	CreateDirectory("Buildings", NULL);

	std::ofstream file("Buildings\\" + fileName + ".tgb", std::ios::out | std::ios::trunc | std::ios::binary);

	int x;
	int y;

	cout << "Enter the tile-x coordinate of the building: " << flush;
	cin >> x;
	cout << "Enter the tile-y coordinate of the building: " << flush;
	cin >> y;

	int w;
	int h;

	cout << "Enter the width in tiles of the building: " << flush;
	cin >> w;
	cout << "Enter the height in tiles of the building: " << flush;
	cin >> h;

	int entranceX;

	cout << "Enter relative x-position of the entrance: " << flush;
	cin >> entranceX;

	int exitX;
	int exitY;

	cout << "Enter the x coordinate of the building exit: " << flush;
	cin >> exitX;
	cout << "Enter the y coordinate of the building exit: " << flush;
	cin >> exitY;

	std::string junk;

	std::string outsideWorldName;
	cout << "Enter the name of the outside world: ";
	getline(cin, junk);
	getline(cin, outsideWorldName);

	size_t outWorldNameSize = outsideWorldName.size();

	std::string insideWorldName;
	cout << "Enter the name of the inside world: ";
	getline(cin, insideWorldName);

	size_t inWorldNameSize = insideWorldName.size();

	int textureToUse;
	cout << "Enter the texture ID to use: " << flush;
	cin >> textureToUse;

	file.write((char*)&x, sizeof(int));
	file.write((char*)&y, sizeof(int));

	file.write((char*)&w, sizeof(int));
	file.write((char*)&h, sizeof(int));

	file.write((char*)&entranceX, sizeof(int));

	file.write((char*)&exitX, sizeof(int));
	file.write((char*)&exitY, sizeof(int));

	file.write((char*)&outWorldNameSize, sizeof(size_t));
	file.write((char*)&outsideWorldName[0], outWorldNameSize);

	file.write((char*)&inWorldNameSize, sizeof(size_t));
	file.write((char*)&insideWorldName[0], inWorldNameSize);

	file.write((char*)&textureToUse, sizeof(int));


	cout << "File written!" << endl;
	system("PAUSE");

	return 0;
}