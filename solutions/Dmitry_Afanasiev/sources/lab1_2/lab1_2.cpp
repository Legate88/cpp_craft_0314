// lab1_2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>

using namespace std;

static const long codesArrayMaxSize = 1000000;

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt");

	long numberOfValues = 0;
	float bufferValue = 0.f;
			
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

		long codesArrayCount = 0;
		inputFile >> codesArrayCount;
		numberOfValues -= 1;

		codesArrayCount = (codesArrayCount > codesArrayMaxSize)? codesArrayMaxSize : (codesArrayCount <= 0)? 0 : codesArrayCount;

		if (codesArrayCount < numberOfValues)
		{
			vector <float> codesArray;
			
			while (!inputFile.eof() && codesArrayCount > 0)
			{
				inputFile >> bufferValue;
				codesArray.push_back(bufferValue);
				codesArrayCount -= 1;
				cout << bufferValue << endl;
			}

			bool equalsFound = false;

			while (!inputFile.eof())
			{
				inputFile >> bufferValue;
				
				for (long i = 0; i < codesArray.size(); i++)
				{
					if (fabsf(codesArray[i] - bufferValue) <= 1e-4)
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

