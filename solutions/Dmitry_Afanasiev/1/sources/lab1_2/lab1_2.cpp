// lab1_2.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>

using namespace std;

static const size_t codesArrayMaxSize = 1000000;

int main(int argc, char* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt");

	size_t numberOfValues = 0;
	double bufferValue = 0.0;
			
	while (!inputFile.eof())
	{
		inputFile >> bufferValue;
		numberOfValues += 1;
	}

	inputFile.close();

	if (numberOfValues > 2)
	{
		ifstream inputFile (BINARY_DIR "/input.txt");
		ofstream outputFile (BINARY_DIR "/output.txt");

		size_t codesArrayCount = 0;
		inputFile >> codesArrayCount;
		numberOfValues -= 1;

		codesArrayCount = (codesArrayCount > codesArrayMaxSize)? codesArrayMaxSize : (codesArrayCount <= 0)? 0 : codesArrayCount;

		if (codesArrayCount < numberOfValues)
		{
			vector <double> codesArray;
			
			while (!inputFile.eof() && codesArrayCount > 0)
			{
				inputFile >> bufferValue;
				codesArray.push_back(bufferValue);
				codesArrayCount -= 1;
			}

			bool equalsFound = false;

			while (!inputFile.eof())
			{
				inputFile >> bufferValue;
				
				for (size_t i = 0; i < codesArray.size(); i++)
				{
					if (fabs(codesArray[i] * 1e4 - bufferValue * 1e4) < 1)
					{
						outputFile << "YES \n";
						equalsFound = true;
						break;
					}
				}

				if (!equalsFound)
				{
					outputFile << "NO \n";
				}

				equalsFound = false;
			}
		}

		outputFile.close();
		inputFile.close();
	}

	return EXIT_SUCCESS;
}

