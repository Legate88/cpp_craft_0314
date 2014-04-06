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
    
	double readToDouble(ifstream &file) const
	{
		double val = 0.0;
        file.read((char *)&val, sizeof(double));

		return val;
	}
    
	uint32_t readToUint32(ifstream &file) const
	{
		uint32_t val = 0;
        file.read((char *)&val, sizeof(uint32_t));

		return val;
	}
    
	uint32_t readDateToUint32(ifstream &file, const size_t size) const
	{
		char *message_ = new char [size + 1];
        file.read(message_, size * sizeof(char));
        message_[size] = '\0';
        
		uint32_t val = atoi(message_);
        
        delete [] message_;
        
		return val;
	}
    
	string readMessage(ifstream &file, const size_t size) const
	{
		char *message_ = new char [size + 1];
        file.read(message_, size * sizeof(char));
        message_[size] = '\0';
        
        string buf = string(message_);
        delete [] message_;
        
        return buf;
	}
};


void readInputFileError()
{
	ofstream outputFile (BINARY_DIR "/output.txt");
    
	outputFile << "unknown error while reading input.txt";
    
	outputFile.close();
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

