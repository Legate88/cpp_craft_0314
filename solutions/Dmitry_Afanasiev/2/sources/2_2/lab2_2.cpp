// lab2_2.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <iostream>
#include <fstream>
#include "boost\cstdint.hpp"

using namespace std;

static const size_t max_TYPE_Value = 100001;
static const size_t capacityFilter = 2048;

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
	ofstream outputFile (BINARY_DIR "/output.txt", ios::binary);

	for (size_t i = 0; i < size; i++)
	{
		if (statistic[i].messageCount > 0)
		{
			const double averageValue = static_cast<double>(statistic[i].messageCount) / static_cast<double>(statistic[i].secondsCount);

			outputFile.write((char *)i, sizeof(size_t));
			outputFile.write((char *)&averageValue, sizeof(double));
		}
	}

	outputFile.close();
}

void readInputFileError()
{
	ofstream outputFile (BINARY_DIR "/output.txt");

	outputFile << "unknown error while reading input.txt";

	outputFile.close();
}


int main(int argc, char* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt", ios::binary);

	inputFile.seekg(0, ios::end);
	size_t const fileSize = inputFile.tellg();
	inputFile.seekg(0);
	size_t curPos = inputFile.tellg();

	if (inputFile)
	{
		uint32_t prevSecondValue = 0;

		messageData *messagesBufer = new messageData[max_TYPE_Value];
		cleanBuffers(messagesBufer, max_TYPE_Value);

		staticticData *messagesStatistic = new staticticData[max_TYPE_Value];
		cleanStatistic(messagesStatistic, max_TYPE_Value);

		while (curPos < fileSize)
		{
			uint32_t message_type = 0;
			uint32_t message_time = 0;
			uint32_t message_length = 0;

			inputFile.read(reinterpret_cast<char*>(&message_type), sizeof(uint32_t));
			inputFile.read(reinterpret_cast<char*>(&message_time), sizeof(uint32_t));
			inputFile.read(reinterpret_cast<char*>(&message_length), sizeof(uint32_t));

			char * message_content = new char[message_length + 1];
			message_content[message_length] = '\0';
			inputFile.read(static_cast<char*>(message_content), message_length);
			delete [] message_content;

			const uint32_t messageCapacity = 3 * sizeof(uint32_t) + message_length * sizeof(char);
			curPos = inputFile.tellg();

			if (message_time >= prevSecondValue)
			{
				if (message_time != prevSecondValue)
				{
					saveStatistic(messagesStatistic, messagesBufer, max_TYPE_Value);
					cleanBuffers(messagesBufer, max_TYPE_Value);
				}

				addToBuffer(messagesBufer, message_type, messageCapacity);

				prevSecondValue = message_time;
			}
		}

		inputFile.close();

		saveStatistic(messagesStatistic, messagesBufer, max_TYPE_Value);
		saveStatisticToFile(messagesStatistic, max_TYPE_Value);

		delete messagesBufer;
		delete messagesStatistic;
	}
	else
	{
		readInputFileError();
	}

	return EXIT_SUCCESS;
}