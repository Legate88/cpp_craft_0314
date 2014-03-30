#include <iostream>
#include <fstream>
#include <string>
#include <boost/cstdint.hpp>

using namespace std;

struct messageData
{
	double			price;
	double			vwap;
	double			f1;
	double			t1;
	double			f2;
	double			f3;
	double			f4;
    
	uint32_t		volume;
	uint32_t		year;
	uint32_t		month;
	uint32_t		day;
    
	string			message;
	
    
	messageData(ifstream &file)
	{
		message = this->readMessage(file, 8);
		year = this->readDateToUint32(file, 4);
		month = this->readDateToUint32(file, 2);
		day = this->readDateToUint32(file, 2);
		price = this->readToDouble(file);
		vwap = this->readToDouble(file);
		volume = this->readToUint32(file);
		f1 = this->readToDouble(file);
		t1 = this->readToDouble(file);
		f2 = this->readToDouble(file);
		f3 = this->readToDouble(file);
		f4 = this->readToDouble(file);
	}
    
	~messageData()
	{
		
	}
    
	void printInfo() const
	{
		cout << message << " ";
		cout << year << month << day << " ";
		cout << price << " " << vwap << " " << volume << " ";
		cout << f1 << " " << t1 << " " << f2 << " " << f3 << " " << f4 << endl;
	}
    
	void writeInfoToFile(ofstream &file) const
	{
        const uint32_t date = (year - 1) * 372 + (month - 1) * 31 + day;
        const double writePrice = vwap;
        const uint32_t writeVolume = volume;
        
        file.write((char *)message.c_str(), (message.length() + 1) * sizeof(char));
        file.write((char *)&date, sizeof(uint32_t));
        file.write((char *)&writePrice, sizeof(double));
        file.write((char *)&writeVolume, sizeof(uint32_t));
        file.write((char *)&f2, sizeof(double));
	}
    
	double readToDouble(ifstream &file)
	{
		double val = 0.0;
        file.read((char *)&val, sizeof(double));

		return val;
	}
    
	uint32_t readToUint32(ifstream &file)
	{
		uint32_t val = 0;
        file.read((char *)&val, sizeof(uint32_t));

		return val;
	}
    
	uint32_t readDateToUint32(ifstream &file, const size_t size)
	{
		string buf = "";
		char bufChar = ' ';
		uint32_t val = 0;
        
		for (size_t i = 0; i < size; i++)
		{
			if (!file.eof())
			{
				file.read((char *)&bufChar, sizeof(char));
				buf.push_back(bufChar);
			}
		}
        
		val = atoi(buf.c_str());
        
		return val;
	}
    
	string readMessage(ifstream &file, const size_t size)
	{
		char bufChar = ' ';
		string buf = "";
        
		for (size_t i = 0; i < size; i++)
		{
			if (!file.eof())
			{
				file.read((char *)&bufChar, sizeof(char));
				buf.push_back(bufChar);
			}
		}
        
		return buf;
	}
};


void readInputFileError()
{
	ofstream outputFile (BINARY_DIR "/output.txt");
    
	outputFile << "unknown error while reading input.txt";
    
	outputFile.close();
}


void generateMessageToFile(ofstream &file)
{
    string mess_ = "";
    for (size_t i = 0; i < 8; i++)
    {
        mess_.push_back(rand() % ('z' - 'a') + 'a');
        srand(static_cast<uint32_t>(clock()));
    }
    
    string date_ = "20140505";
    
    double price_ = static_cast<double>(rand() % 99 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    double vwap_ = static_cast<double>(rand() % 99 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    uint32_t volume_ = static_cast<uint32_t>(rand() % 99 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    double f1_ = static_cast<double>(rand() % 99 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    double t1_ = static_cast<double>(rand() % 99 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    double f2_ = static_cast<double>(rand() % 99 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    double f3_ = static_cast<double>(rand() % 99+ 1);
    srand(static_cast<uint32_t>(clock()));
    
    double f4_ = static_cast<double>(rand() % 99 + 1);
    srand(static_cast<uint32_t>(clock()));
    
    file.write((char *)mess_.c_str(), mess_.length() * sizeof(char));
    file.write((char *)date_.c_str(), date_.length() * sizeof(char));
    file.write((char *)&price_, sizeof(double));
    file.write((char *)&vwap_, sizeof(double));
    file.write((char *)&volume_, sizeof(uint32_t));
    file.write((char *)&f1_, sizeof(double));
    file.write((char *)&t1_, sizeof(double));
    file.write((char *)&f2_, sizeof(double));
    file.write((char *)&f3_, sizeof(double));
    file.write((char *)&f4_, sizeof(double));
}



int main(int argc, char* argv[])
{
	ifstream inputFile (BINARY_DIR "/input.txt", ios::binary | ios::in);
	ofstream outputFile (BINARY_DIR "/output.txt", ios::binary | ios::out);
    
	if (inputFile.is_open())
	{
        inputFile.seekg(0, ios::end);
        const size_t fileSize = inputFile.tellg();
        inputFile.seekg(0);
        size_t curPos = inputFile.tellg();

		while (curPos < fileSize)
		{
			messageData *newData = new messageData(inputFile);
			newData->writeInfoToFile(outputFile);
			delete newData;
            
            curPos = inputFile.tellg();
		}
        
		inputFile.close();
		outputFile.close();
	}
	else
	{
		readInputFileError();
	}
    
	return EXIT_SUCCESS;
}

