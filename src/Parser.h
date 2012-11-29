/********************************************************
* File: Parser.h										*
* Authors:												*
*	James (Jimmy) Hartnett								*
*	Collin Dobmeyer										*
*	Andrew Socha										*
* IP: Search Engine										*
*This file implements the class structure of the Usenet	*
* parser for the search engine							*
********************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "Word.h"
//#include <math.h>

using namespace std;

class Parser
{
public:

	Parser();

	~Parser();

	void parseFile(string);	// actual file parser

	void parseFiles(string, string); // filename to parse, output index path

	void parseFiles(string);	// filename to parse, uses default file path

	void parseSingleFile(string); // parse a single file, with proper support

	void parseSingleFile(string, string); // overloaded parseSingleFile, adds output filename support

	bool areFilesParsed();

	string getIndexPath();

	void setNumberOfFiles(int );

	bool checkStopWord(string);

	string stemWord(string);

private:

	bool filesAreParsed_;
	string inputFilePath_;
	string outputFilePath_;
	int numberOfFiles_;
	vector<Word*> wordList_;

	Word* getWord(string, string, int, string, int);

	bool doesWordExist(Word*, vector<Word*>);

	void organizeFiles(string);
	void writeOutputFile();
	void calculateTFIDF(vector<Word*>, string, int);

	string wstring2string(wstring);
	wstring string2wstring(string);
};

#endif
