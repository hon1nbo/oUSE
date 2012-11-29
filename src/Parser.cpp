/********************************************************
* File: Parser.cpp										*
* Authors:												*
*	James (Jimmy) Hartnett								*
*	Collin Dobmeyer										*
*	Andrew Socha										*
* IP: Search Engine										*
*This file implements the memeber functions of Parser.h *
********************************************************/

#include "Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "english_stem.h"
#include <ctype.h>


Parser::Parser()
{
	filesAreParsed_ = false;
	numberOfFiles_ = 0;
	inputFilePath_ = "";

	// this sets default file paths for different OS

	#if defined (_MSC_VER)
		outputFilePath_ = "C:/index.txt";	// windows testing
		//outputFilePath_ = "E:/CSE2341/Hartnett-Dobmeyer-Socha-IP-7/data/index.txt"; // just for testing
	#else
		outputFilePath_ = "/index.txt"; // Ubuntu testing
	#endif

}

Parser::~Parser()
{
	int listSize = wordList_.size();
	for (int i = 0; i < listSize; i++)
		delete wordList_[i];
}

/**
	parseFile does the actual parsing work
	it is the root of all of the parsing functions
**/
void Parser::parseFile(string fullFileName)
{
		vector<Word*> tempWordList;
		int numberOfWords = 0;

		fstream myFile;

		myFile.open(fullFileName);

		if (!myFile.is_open())
		{
			cerr << "There was a problem opening file: " << fullFileName << endl;
			//break; // if we decided to break out of entire parsing operation for each error
			return;
		}

		bool validFile = true; // if this variable is ever set to false, the loop breaks and the file is discarded

		int fieldCount = 1;

		int day = 1;
		string month = "";
		int year = 1;

		bool dateChecked = false;

		while (!myFile.eof())
		{
			string tempLine = "";
			getline(myFile, tempLine);
			string tempWord = "";

			switch (fieldCount)	// this is to help determine what metadata is currently laoded, and proceed properly
			{
				case 1 :
				{
					if (tempLine.find("Date:") == 0)
						fieldCount = 2;
					break;
				}
				case 2 :
				{
					if (tempLine.find("Lines:") == 0)
						fieldCount = 3;
					break;
				}
				default: cout << "ERROR" << endl; validFile = false; break;	// meta data has somehow ben skipped or mishandled
			}

			size_t found = 0;

			if (fieldCount == 2 && !dateChecked) // parse out date if not done already
			{
				int dateParseCounter = 1; // this is to get through the items in a Date line
				//ex line: "Date: Mon, 29 Mar 1993 11:57:19 GMT"
				while (tempLine.size() != 0)
				{
					found = tempLine.find(" ");
					tempWord = tempLine.substr(0, found);
					tempLine = tempLine.substr(found + 1);
					
					switch (dateParseCounter)
					{
						case 1: break;
						case 2: day = atoi(tempWord.c_str()); break;
						case 3: month = tempWord; break;
						case 4: year = atoi(tempWord.c_str()); break;
						case 5: tempLine = ""; break;
						default: break;
					}
					dateParseCounter++;

					// if this is true, date is not properly formatted (atoi funciton returns 0 if not an integer string)
					if (day == 0 || year == 0) 
						validFile = false;

				}

				dateChecked = true;
			}
			// break out of loop if data is incoherent
			if (!validFile)
				break;

			if (fieldCount == 3)
			{
				while (!myFile.eof())
				{
					myFile >> tempWord;

					bool isStop = checkStopWord(tempWord);
	
					if (!isStop)
					{
						Word* tempWordWord = getWord(tempWord, fullFileName, day, month, year);
						bool wordExists = doesWordExist(tempWordWord, tempWordList);

						if (!wordExists)
							tempWordList.push_back(tempWordWord);	// vector of words specific to file, allows for manipulation

						numberOfWords++;
					}
				}
			}

			//fieldCount++; // this is manual field changing due to file formatting differences in test data.

		}	// end while(!myFile.eof())

		myFile.close();

		calculateTFIDF(tempWordList, fullFileName, numberOfWords);

}

/**
	member function for parsing the Usenet files
	and storing them in the index

	input should be a DIRECTORY, terminated with a / or \
	based on the OS of operation
**/

void Parser::parseFiles(string input)
{
	for (int i = 0; i < wordList_.size(); i++)
		delete wordList_[i];
	
	cout << "Parsing Files in " << input << endl;

	organizeFiles(input);

	cout << " Number of files: " << numberOfFiles_ << endl;
	cout << endl;
	cout << "Progress: " << endl;

/*
	double progressFraction = 50 / numberOfFiles_;

		#if defined (_MSC_VER)
				system("cls");
		#else
				system("clear");
		#endif

		// loading bar code

		cout << "Progress: " << fullFileName << endl;
		cout << "<";

		for (int i = 0; i < (progressFraction * (double)currentFileNumber); i++)
			cout << "#";

		for (int i = 0; i < (50 - (progressFraction * (double)currentFileNumber) -1 ); i++)
			cout << "-";

		cout << ">" << endl;
*/

	for (int currentFileNumber = 1; currentFileNumber <= numberOfFiles_; currentFileNumber++)
	{

		stringstream ss(stringstream::in | stringstream::out);

		ss << input;

		ss << "0";
		ss << " (";
		ss << currentFileNumber;
		ss << ")";

		//ss << ".txt";
		string fullFileName = ss.str();


		if ((currentFileNumber % 25) == 0)
			cout << fullFileName << endl;

		parseFile(fullFileName);

	}
	writeOutputFile();	// write the word list to the file.

	filesAreParsed_ = true;
}

/**
	parseSingleFile(string) is for parsing a single file
	it deletes the old wordList_ and will output a fresh file
**/
void Parser::parseSingleFile(string fullFileName)
{
	for (int i = 0; i < wordList_.size(); i++)
		delete wordList_[i];

	wordList_.clear();

	parseFile(fullFileName);

	writeOutputFile();
	filesAreParsed_ = true;
}

/**
	calculateTFIDF takes the words in the file and the path
	and calculates each word and file's TFIDF
	NOTE: this function is PRIVATE
**/

void Parser::calculateTFIDF(vector<Word*> tempWordList, string fullFileName, int numberOfWords)
{
	// first must find words that appear multiple times
	// and then calculate that term frequency for the document
	// then calculate the temporary TFIDF and also store values
	// to aid in recalculation as the parsing continues

	for (int i = 0; i < tempWordList.size(); i++)
	{
		vector<Word::File*> tempRanking = tempWordList[i]->getRanking();
		for (int j = 0; j < tempRanking.size(); j++)
		{
			if (tempRanking[j]->fileName == fullFileName)
			{
				double termFrequency = ((double)tempRanking[j]->numberOfAppearances / (double)numberOfWords);
				double iDFTemp = ((double)numberOfFiles_ / (double)tempRanking.size());
				double inverseDocumentFrequency = log10(iDFTemp);
				double tfidf = termFrequency * inverseDocumentFrequency;
				tempRanking[j]->TFIDF = tfidf;
				tempRanking[j]->totalWordsInFile = numberOfWords;
				break;
			}
		}
	}
}

/**
	checkStopWord(string) checks to see if the word is a stop word
	and therefore if we can ignore it
	NOTE: this function is PRIVATE
**/

bool Parser::checkStopWord(string tempWord)
{

	int strSize = tempWord.size();

	for (int i = 0; i < strSize; i++)
		tempWord[i] = tolower(tempWord[i]);

	if (tempWord == "and")
		return true;
	else if (tempWord == "or")
		return true;
	else if (tempWord == "a")
		return true;
	else if (tempWord == "an")
		return true;
	else if (tempWord == "the")
		return true;

	else
		return false;
}

/**
	doesWordExist returns true if the word is found within the given vector
	and false if not
	vector is passed by reference to the head of the vector
**/

bool Parser::doesWordExist(Word* tempWord, vector<Word*> wordList)
{
	for (int i = 0; i < wordList.size(); i++)
		if (wordList[i] == tempWord)
			return true;

	return false;
}

/**
	getWord(string) finds or creates a word object as the file is read
	Note: this is PRIVATE
**/

Word* Parser::getWord(string tempWord, string filePath, int day, string month, int year)
{
	bool found = false;
	Word* foundWord;

	tempWord = stemWord(tempWord);

	for (int i = 0; i < wordList_.size(); i++)
	{
		if (wordList_[i]->getWord() == tempWord)
		{
			foundWord = wordList_[i];
			found = true;
			break;
		}
	}

	if (found)
	{
		vector<Word::File*> tempRanking = foundWord->getRanking();
		bool foundFile = false;
		for (int i = 0; i < tempRanking.size(); i++)
		{
			if (tempRanking[i]->fileName == filePath)
			{
				tempRanking[i]->numberOfAppearances++;
				foundFile = true;
				break;
			}
		}

		if (!foundFile)
		{
			foundWord->addFile(filePath, year, month, day);
		}

		return foundWord;

	}
	else
	{
		Word* newWord = new Word(tempWord);
		wordList_.push_back(newWord);
		newWord->addFile(filePath, year, month, day);
		return newWord;
	}
}
/**
	organizeFiles() takes the directory path
	and renames the files and returns the
	total number of files
**/

void Parser::organizeFiles(string input)
{
	inputFilePath_ = input;

	// insert function here

	//numberOfFiles_ = 2;	// just for testing
	//numberOfFiles_ = 42;
}

/**
	overloaded operator for parseFile that allows a specific 
	output filepath.
	It calls the regular parseFile after setting the output path

	input should be a DIRECTORY, terminated with a / or \
	based on the OS of operation
	
	output should be a FULL file path to a specific file,
	either an existing file or a file to be created
**/

void Parser::parseFiles(string input, string output)
{
	outputFilePath_ = output;

	parseFiles(input);
}

/**
	parseSingleFile(string, string) is the overloaded version of
	parseSingleFile(string)
	the second string parameter is the output filename and path
**/

void Parser::parseSingleFile(string input, string output)
{
	outputFilePath_ = output;

	parseSingleFile(input);

}
/**
	returns the status of the parser to the caller.
**/

bool Parser::areFilesParsed()
{
	return filesAreParsed_;
}

/**
	returns the path of the index file created.
**/

string Parser::getIndexPath()
{
	return outputFilePath_;
}

/**
	writeOutputFile writes all of the words and their attributes
	to the index file on disk

	Refer to oUSE/examples for a sample index file
	and the corresponding input files
**/

void Parser::writeOutputFile()
{
	ofstream myFile;

	myFile.open(outputFilePath_);

	if (!myFile.is_open())
	{
		cerr << "There was an error creating the index file" << endl;
	}

	myFile << numberOfFiles_;
	myFile << "\n";
	myFile << wordList_.size();
	myFile << "\n";

	for (int i = 0; i < wordList_.size(); i++)
	{
		myFile << wordList_[i]->getWord();
		myFile << "\n";

		vector<Word::File*> tempRanking = wordList_[i]->getRanking();
		myFile << tempRanking.size();
		myFile << "\n";
		for (int x = 0; x < tempRanking.size(); x++)
		{
			myFile << tempRanking[x]->fileName;
			myFile << "\n";
			myFile << tempRanking[x]->TFIDF;
			myFile << "\n";
			myFile << tempRanking[x]->numberOfAppearances;
			myFile << "\n";
			myFile << tempRanking[x]->totalWordsInFile;
			myFile << "\n";
			myFile << tempRanking[x]->year;
			myFile << "\n";
			myFile << tempRanking[x]->strmonth;
			myFile << "\n";
			myFile << tempRanking[x]->day;
			myFile << "\n";
		}
	}

	myFile.close();
}

/**
	stemWord(string) prepares for and executes the stemming of words
	using the included stemming libray based on the Porter Algorithms
	It also forces the string to lowercase
**/

string Parser::stemWord(string tempWord)
{
	int strSize = tempWord.size();

	for (int i = 0; i < strSize; i++)
		tempWord[i] = tolower(tempWord[i]);

	wstring tempW = string2wstring(tempWord);
    stemming::english_stem<> StemEnglish;
    StemEnglish(tempW);
	tempWord = wstring2string(tempW);

	return tempWord;
}

/**
	converts a wstring to a string
	This code was found online at 
	http://agraja.wordpress.com/2008/09/08/cpp-string-wstring-conversion/
**/

string Parser::wstring2string(wstring wstr) {

string str(wstr.length(),' ');
copy(wstr.begin(),wstr.end(),str.begin());
return str;
}

/**
	converts a string to wstring
	This code was found online at 
	http://agraja.wordpress.com/2008/09/08/cpp-string-wstring-conversion/
**/

wstring Parser::string2wstring(string str) {

wstring wstr(str.length(),L' ');
copy(str.begin(),str.end(),wstr.begin());
return wstr;
}

/**
	setNumberOfFiles(int) sets the number of files in the directory
**/

void Parser::setNumberOfFiles(int number)
{
	numberOfFiles_ = number;
}