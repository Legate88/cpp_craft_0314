// lab1_3.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>

using namespace std;

static const char isle = 'o';
static const char water = '~';

inline void searchForIsle(vector<string> &map_, const size_t line, const size_t row)
{
	if (map_[line][row] == isle)
	{
		map_[line][row] = water;

		if (line > 0)
		{
			searchForIsle(map_, line - 1, row);
		}

		if (line < map_.size() - 1)
		{
			searchForIsle(map_, line + 1, row);
		}

		if (row > 0)
		{
			searchForIsle(map_, line, row - 1);
		}

		if (row < map_[0].size() - 1)
		{
			searchForIsle(map_, line, row + 1);
		}
	}
}

int main(int argc, char* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt");
	size_t numberOfIsles = 0;
	vector <string> map;

	while (!inputFile.eof())
	{
		string currentMapString = "";
		getline(inputFile, currentMapString);
		map.push_back(currentMapString);
	}

	inputFile.close();

	for (size_t line = 0; line < map.size(); line++)
	{
		for (size_t row = 0; row < map[line].size(); row++)
		{
			if (map[line][row] == isle)
			{
				numberOfIsles += 1;
				searchForIsle(map, line, row);
			}
		}
	}

	ofstream outputFile (BINARY_DIR "/output.txt");
	outputFile << numberOfIsles;
	outputFile.close();
	
	return EXIT_SUCCESS;
}


