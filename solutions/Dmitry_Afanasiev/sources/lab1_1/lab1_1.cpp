// lab1_1.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <algorithm>

using namespace std;

static const size_t stringMaxSize = 100000;

inline bool avaliableSymbol(const char symbol)
{
	string notSupportedSymbols = " -\/*:.,!&?;%^";

	for (size_t i = 0; i < notSupportedSymbols.size(); i++)
	{
		if (symbol == notSupportedSymbols[i])
		{
			return false;
		}
	}

	return true;
}

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

	size_t i = 0;
	while (i < str.length())
	{
		if (!avaliableSymbol(str[i]))
		{
			str.erase(i, 1);
		}
		else
		{
			i += 1;
		}
	}
}

int main(int argc, char* argv[])
{
	setlocale (LC_ALL, "RUSSIAN");

	ifstream inputFile (BINARY_DIR "/input.txt");
	ofstream outputFile (BINARY_DIR "/output.txt");

	if (inputFile.is_open())
	{
		string stringToFoundMatches = "";

		getline(inputFile, stringToFoundMatches);
		transformString(stringToFoundMatches);

		while (!inputFile.eof()) 
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

       inputFile.close();
	   outputFile.close();
	}

	return EXIT_SUCCESS;
}

