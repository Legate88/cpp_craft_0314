// lab1_1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <algorithm>

using namespace std;

static const long stringMaxSize = 100000;

inline void transformString(string &str)
{
	if (str.size() > stringMaxSize)
	{
		str.resize(stringMaxSize);
	}

	transform(str.begin(),
			  str.end(),
			  str.begin(),
			  ::toupper);	

	long i;
	while ((i = str.find(" ")) < str.length())
	{
		str.erase(i, 1);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale (LC_ALL, "RUSSIAN");

	ifstream inputFile (BINARY_DIR "/input.txt");
	ofstream outputFile (BINARY_DIR "/output.txt");

	if (inputFile.is_open())
	{
		long iterationsCount = 0;
		string stringToFoundMatches = "";

		while (!inputFile.eof()) 
		 {
			 if (iterationsCount == 0)
			 {
				getline(inputFile, stringToFoundMatches);
    
				transformString(stringToFoundMatches);
			 }
			 else
			 {
				string stringToResolveMatching = "";

				getline(inputFile, stringToResolveMatching);

				transformString(stringToResolveMatching);

				if (stringToFoundMatches.find(stringToResolveMatching) != string :: npos)
				{
					outputFile << "YES \n";
				}
				else
				{
					outputFile << "NO \n";
				}
			 }

			 iterationsCount += 1;
		}

       inputFile.close();
	   outputFile.close();
	}

	return EXIT_SUCCESS;
}

