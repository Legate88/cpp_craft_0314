#include <iostream>
#include <fstream>
#include <boost/cstdint.hpp>


using namespace std;


static const size_t max_TYPE_Value = 100001;
static const size_t capacityFilter = 2048;


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


void readInputFileError()
{
	ofstream outputFile (BINARY_DIR "/output.txt");
    
	outputFile << "unknown error while reading input.txt";
    
	outputFile.close();
}


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

void saveStatisticToFile(staticticData *statistic, const size_t size)
{
	ofstream outputFile (BINARY_DIR "/output.txt", ios::binary | ios::out);
    
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




int main(int argc, char* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt", ios::binary | ios::in);
    
	inputFile.seekg(0, ios::end);
	const uint32_t fileSize = inputFile.tellg();
	inputFile.seekg(0);
	uint32_t curPos = inputFile.tellg();
    
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
		saveStatisticToFile(messagesStatistic, max_TYPE_Value);
        
		delete [] messagesBufer;
		delete [] messagesStatistic;
	}
	else
	{
		readInputFileError();
	}
    
	return EXIT_SUCCESS;
}