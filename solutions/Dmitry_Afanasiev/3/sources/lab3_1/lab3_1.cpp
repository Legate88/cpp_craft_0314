#include <iostream>
#include <fstream>
#include <string>
#include <boost\cstdint.hpp>
#include <boost\thread.hpp>
#include <boost\bind.hpp>
#include <vector>
#include <algorithm>


using namespace std;

static const unsigned long long maxMessagesCount = 10e16;
static const uint32_t maxAvaliableMessageType = 4;
static const size_t filesCount = 1000;

struct messageData
{
	uint32_t		messageType;
	uint32_t		messageTime;
	uint32_t		messageLength;
	char*			message;

	messageData(const uint32_t type, const uint32_t time, const uint32_t len, const char * const mess) : messageType(0), messageTime(0), messageLength(0), message(NULL)
	{
		messageType = type;
		messageTime = time;
		messageLength = len;
        
		message = new char [len + 1];
		strcpy(message, mess);
		message[len] = '\0';
	}
    
    messageData(ifstream &file) : messageType(0), messageTime(0), messageLength(0), message(NULL)
    {
        file.read((char*)&messageType, sizeof(uint32_t));
        file.read((char*)&messageTime, sizeof(uint32_t));
        file.read((char*)&messageLength, sizeof(uint32_t));
        
        message = new char [messageLength + 1];
        file.read(message, messageLength * sizeof(char));
        message[messageLength] = '\0';
    }
    
	~messageData()
	{
		delete [] message;
        message = NULL;
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
		file.write((char*)&messageType, sizeof(uint32_t));
		file.write((char*)&messageTime, sizeof(uint32_t));
		file.write((char*)&messageLength, sizeof(uint32_t));
		file.write(message, messageLength * sizeof(char));
	}
};


bool compareFunction (const messageData* const a, const messageData* const b)
{
	return a->messageTime < b->messageTime;
}


string fileNameFromIndex(const size_t index, const bool isInput)
{
	string name = (isInput)? "input_" : "output_";
	string endOfName = ".txt";
	string midName = to_string(index);
	string additionals = (index < 10)? "00" : (index < 100)? "0" : "";
	string finalName = name.append(additionals.append(midName.append(endOfName)));

	return finalName;
}


void processFileWithIndex(const size_t index)
{
	ifstream inputFile (BINARY_DIR fileNameFromIndex(index, true), ios::binary);

	if (inputFile)
	{
		uint32_t maxMessageTime = 0;
		unsigned long long messagesCount = 0;
		vector<messageData*> passingTestMessages;
        
        inputFile.seekg(0, ios::end);
        const streamoff fileSize = inputFile.tellg();
        inputFile.seekg(0);
        streamoff curPos = inputFile.tellg();
        
		while (curPos < fileSize && messagesCount <= maxMessagesCount)
		{
			messageData *newData = new messageData(inputFile);
            curPos = inputFile.tellg();
            
			if (newData->messageTime > maxMessageTime)
			{
				maxMessageTime = newData->messageTime;
			}
            
			messagesCount += 1;
            
			if (newData->messageType <= maxAvaliableMessageType)
			{
				if (newData->messageTime + 2 > maxMessageTime)
				{
					passingTestMessages.push_back(newData);
				}
                else
                {
                    delete newData;
                    newData = NULL;
                }
			}
            else
            {
                delete newData;
                newData = NULL;
            }
		}
        
		inputFile.close();
        
		sort(passingTestMessages.begin(), passingTestMessages.end(), compareFunction);
        
		ofstream outputFile (BINARY_DIR fileNameFromIndex(index, false), ios::binary | ios::out);
        
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
        
        inputFile.close();
	}
}


int main(int argc, char* argv[])
{
	size_t filesCounter = 1;
	while (filesCounter < filesCount)
	{
		boost::thread newThread(boost::bind(&processFileWithIndex, filesCounter));
		newThread.join();

		filesCounter += 1;
	}

	return EXIT_SUCCESS;
}

