#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <boost/cstdint.hpp>



using namespace std;
using namespace boost;


static const unsigned long long maxMessagesCount = 10e16;
static const uint32_t maxAvaliableMessageType = 4;


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


void generateMessageToFile(ofstream &file)
{
    const uint32_t type_ = static_cast<uint32_t>(rand() % 5 + 1);
    srand(static_cast<uint32_t>(clock()));
    const uint32_t time_ = static_cast<uint32_t>(rand() % 10 + 1);
    srand(static_cast<uint32_t>(clock()));
    const uint32_t len_ = static_cast<uint32_t>(rand() % 10 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    char *mess_ = NULL;
    mess_ = new char [len_ + 1];
    mess_[len_] = '\0';
    
    for (size_t i = 0; i < len_; i++)
    {
        mess_[i] = rand() % ('z' - 'a') + 'a';
        srand(static_cast<uint32_t>(clock()));
    }
    
    messageData *data_ = new messageData(type_, time_, len_, mess_);
    data_->writeInfoToFile(file);
    
    delete data_;
    data_ = NULL;
    
    delete [] mess_;
    mess_ = NULL;
}


void readInputFileError()
{
	ofstream outputFile (BINARY_DIR "/output.txt");
    
	outputFile << "unknown error while reading input.txt";
    
	outputFile.close();
}


int main(int argc, char* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt", ios::binary | ios::in);
    
	if (inputFile.is_open())
	{
		uint32_t maxMessageTime = 0;
		unsigned long long messagesCount = 0;
		vector<messageData*> passingTestMessages;
        
        inputFile.seekg(0, ios::end);
        const size_t fileSize = inputFile.tellg();
        inputFile.seekg(0);
        size_t curPos = inputFile.tellg();
        
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
        
		ofstream outputFile (BINARY_DIR "/output.txt", ios::binary | ios::out);
        
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
	else
	{
		readInputFileError();
	}
    
	return EXIT_SUCCESS;
}
