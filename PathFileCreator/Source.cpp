#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;


int main() {

	string fileName;
	cout << "Enter a name for the path file: " << flush;
	getline(cin, fileName);

	CreateDirectory("Paths", NULL);

	std::ofstream file("Paths\\" + fileName + ".tgh", std::ios::out | std::ios::trunc | std::ios::binary);

	int pathSize;

	cout << "Enter the number of steps to the path: " << flush;
	cin >> pathSize;

	file.write((char*)&pathSize, sizeof(int));

	for (int i = 0; i < pathSize; i++) {

		string command;
		int commandInt;

		cout << " -=-=-=-=- PATH STEP: " << i << " -=-=-=-=-" << endl;
		cout << "Enter the command for the current step (MOVE_TO, WAIT): " << flush;
		cin >> command;

		if (command == "MOVE_TO") {
			commandInt = 0;
			float xPos, yPos;

			cout << "Enter the xPos: " << flush;
			cin >> xPos;

			cout << "Enter the yPos: " << flush;
			cin >> yPos;

			file.write((char*)&commandInt, sizeof(int));
			file.write((char*)&xPos, sizeof(float));
			file.write((char*)&yPos, sizeof(float));
		} else if (command == "WAIT") {
			commandInt = 1;
			
			int32_t waitTime;
			cout << "Enter the time to wait: " << flush;
			cin >> waitTime;

			file.write((char*)&commandInt, sizeof(int));
			file.write((char*)&waitTime, sizeof(int32_t));

		}


	}


	cout << "File written!" << endl;
	system("PAUSE");
	return 0;
}