#include <iostream>
#include <fstream>
#include <string>
#include <boost\cstdint.hpp>
#include <boost\thread.hpp>
#include <boost\bind.hpp>

using namespace std;

static const size_t max_TYPE_Value = 100001;
static const size_t capacityFilter = 2048;
static const size_t filesCount = 1000;

struct Message
{
	uint32_t		messageType;
	uint32_t		messageTime;
	uint32_t		messageLength;
	char*			message;
    
	Message(const uint32_t type, const uint32_t time, const uint32_t len, const char * const mess) : messageType(0), messageTime(0), messageLength(0), message(NULL)
	{
		messageType = type;
		messageTime = time;
		messageLength = len;
        
		message = new char [len + 1];
		strcpy(message, mess);
	}
    
    Message(ifstream &file) : messageType(0), messageTime(0), messageLength(0), message(NULL)
    {
        file.read((char*)&messageType, sizeof(uint32_t));
        file.read((char*)&messageTime, sizeof(uint32_t));
        file.read((char*)&messageLength, sizeof(uint32_t));
        
        message = new char [messageLength + 1];
        file.read(message, messageLength * sizeof(char));
        message[messageLength] = '\0';
    }
    
	~Message()
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
    
    uint32_t getSize() const
    {
        return 3 * sizeof(uint32_t) + messageLength * sizeof(char);
    }
};


struct messageData
{
	size_t		messagesCount;
	size_t		messagesCapacity;
};

struct staticticData
{
	size_t		messageCount;
	size_t		secondsCount;
};

void cleanBuffers(messageData *bufer, const size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		bufer[i].messagesCapacity = 0;
		bufer[i].messagesCount = 0;
	}
}

void addToBuffer(messageData *bufer, const uint32_t messType, const uint32_t messSize)
{
	if (bufer[messType].messagesCapacity + messSize <= capacityFilter)
	{
		bufer[messType].messagesCount++;
		bufer[messType].messagesCapacity += messSize;
	}
}

void cleanStatistic(staticticData *statistic, const size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		statistic[i].messageCount = 0;
		statistic[i].secondsCount = 0;
	}
}

void saveStatistic(staticticData *statistic, messageData *buffer, const size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		statistic[i].messageCount += buffer[i].messagesCount;
		if (buffer[i].messagesCount > 0)
		{
			statistic[i].secondsCount += 1;
		}
	}
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


void saveStatisticToFile(staticticData *statistic, const size_t size, size_t index)
{
	ofstream outputFile (BINARY_DIR fileNameFromIndex(index, false), ios::binary | ios::out);
    
	for (size_t i = 0; i < size; i++)
	{
		if (statistic[i].messageCount > 0)
		{
			const double averageValue = static_cast<double>(statistic[i].messageCount) / static_cast<double>(statistic[i].secondsCount);
            
			outputFile.write((char *)&i, sizeof(uint32_t));
			outputFile.write((char *)&averageValue, sizeof(double));
		}
	}
    
	outputFile.close();
}


void processFileWithIndex(size_t index)
{
	ifstream inputFile (BINARY_DIR fileNameFromIndex(index, true), ios::binary | ios::in);
    
	inputFile.seekg(0, ios::end);
	const streamoff fileSize = inputFile.tellg();
	inputFile.seekg(0);
	streamoff curPos = inputFile.tellg();
    
	if (inputFile.is_open())
	{
		uint32_t prevSecondValue = 0;
        
		messageData *messagesBufer = new messageData[max_TYPE_Value];
		cleanBuffers(messagesBufer, max_TYPE_Value);
        
		staticticData *messagesStatistic = new staticticData[max_TYPE_Value];
		cleanStatistic(messagesStatistic, max_TYPE_Value);
        
		while (curPos < fileSize)
		{
            Message *message = new Message(inputFile);
            
			curPos = inputFile.tellg();
            
			if (message->messageTime >= prevSecondValue)
			{
				if (message->messageTime != prevSecondValue)
				{
					saveStatistic(messagesStatistic, messagesBufer, max_TYPE_Value);
					cleanBuffers(messagesBufer, max_TYPE_Value);
				}
                
				addToBuffer(messagesBufer, message->messageType, message->getSize());
				prevSecondValue = message->messageTime;
			}
            
            delete message;
		}
        
		inputFile.close();

		saveStatistic(messagesStatistic, messagesBufer, max_TYPE_Value);
		saveStatisticToFile(messagesStatistic, max_TYPE_Value, index);
        
		delete [] messagesBufer;
		delete [] messagesStatistic;
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
