// lab1_1.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <algorithm>

using namespace std;

static const size_t stringMaxSize = 100000;

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

	str.erase(remove_if(str.begin(), str.end(), [](const char ch)
	 {
		return isspace(ch) || ispunct(ch);
	 }), str.end());
}

int main(int argc, char* argv[])
{
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
				reverse(stringToResolveMatching.begin(), stringToResolveMatching.end());
				if (stringToFoundMatches.find(stringToResolveMatching) != string :: npos)
				{
					outputFile << "YES \n";
				}
				else
				{
					outputFile << "NO \n";
				}
			}
		}

       inputFile.close();
	   outputFile.close();
	}

	return EXIT_SUCCESS;
}

