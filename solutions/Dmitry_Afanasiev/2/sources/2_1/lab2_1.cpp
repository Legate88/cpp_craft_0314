// lab2_1.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <fstream>
#include <string>
#include "boost\cstdint.hpp"
#include <vector>
#include <algorithm>

using namespace std;

static const unsigned long long maxMessagesCount = 10e16;
static const uint32_t maxAvaliableMessageType = 4;

struct messageData
{
	uint32_t		messageType;
	uint32_t		messageTime;
	uint32_t		messageLength;
	char*			message;

	messageData(const uint32_t type, const uint32_t time, const uint32_t len, const char * const mess)
	{
		messageType = type;
		messageTime = time;
		messageLength = len;

		message = new char [len + 1];
		strcpy_s(message, len + 1, mess);
	}

	~messageData()
	{
		delete [] message;
	}

	void printInfo() const
	{
		cout << messageType << " ";
		cout << messageTime << " ";
		cout << messageLength << " ";
		cout << message << endl;
	}

	void writeInfoToFile(ofstream &file) const
	{
		file.write(reinterpret_cast<char*>(messageType), sizeof(uint32_t));
		file.write(reinterpret_cast<char*>(messageTime), sizeof(uint32_t));
		file.write(reinterpret_cast<char*>(messageLength), sizeof(uint32_t));
		file.write(reinterpret_cast<char*>(message), messageLength * sizeof(char));
	}
};


bool compareFunction (const messageData* const a, const messageData* const b)
{
	return a->messageTime < b->messageTime;
}


void readInputFileError()
{
	ofstream outputFile (BINARY_DIR "/output.txt");

	outputFile << "unknown error while reading input.in";

	outputFile.close();
}


int main(int argc, char* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt", ios::binary);

	if (inputFile)
	{
		uint32_t maxMessageTime = 0;
		unsigned long long messagesCount = 0;
		vector<messageData*> passingTestMessages;

		while (!inputFile.eof() && messagesCount <= maxMessagesCount)
		{
			uint32_t message_type = 0;
			uint32_t message_time = 0;
			uint32_t message_length = 0;

			inputFile.read(reinterpret_cast<char*>(message_type), sizeof(uint32_t));
			inputFile.read(reinterpret_cast<char*>(message_time), sizeof(uint32_t));
			inputFile.read(reinterpret_cast<char*>(message_length), sizeof(uint32_t));

			char * message_content = new char[message_length + 1];
			message_content[message_length] = '\0';
			inputFile.read(reinterpret_cast<char*>(message_content), message_length);

			if (message_time > maxMessageTime)
			{
				maxMessageTime = message_time;
			}

			messagesCount += 1;

			if (message_type <= maxAvaliableMessageType)
			{
				if (message_time + 2 > maxMessageTime)
				{
					messageData *newData = new messageData(message_type, message_time, message_length, message_content);
					passingTestMessages.push_back(newData);
				}
			}

			delete [] message_content;
		}

		inputFile.close();

		sort(passingTestMessages.begin(), passingTestMessages.end(), compareFunction);

		ofstream outputFile ("output.txt", ios::binary | ios::out);

		for (unsigned long long i = 0; i < passingTestMessages.size(); i++)
		{
			passingTestMessages[i]->writeInfoToFile(outputFile);
		}

		outputFile.close();

		vector<messageData*>::iterator it;
		for (it = passingTestMessages.begin(); it != passingTestMessages.end(); ++it)
		{
			delete *it;
		}
	}
	else
	{
		readInputFileError();
	}

	return EXIT_SUCCESS;
}
