// lab1_3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>

using namespace std;

static const char isle = 'o';
static const char water = '~';

inline void searchForIsle(vector<string> &map_, long line, long row)
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

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt");
	ofstream outputFile (BINARY_DIR "/output.txt");

	long numberOfIsles = 0;
	
	vector <string> map;

	while (!inputFile.eof())
	{
		string currentMapString = "";
		getline(inputFile, currentMapString);
		map.push_back(currentMapString);
	}

	for (long line = 0; line < map.size(); line++)
	{
		for (long row = 0; row < map[line].size(); row++)
		{
			if (map[line][row] == isle)
			{
				numberOfIsles += 1;
				searchForIsle(map, line, row);
			}
		}
	}

	outputFile << numberOfIsles;

	outputFile.close();
	inputFile.close();

	return EXIT_SUCCESS;
}


