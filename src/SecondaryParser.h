#ifndef SECONDARYPARSER_H
#define SECONDARYPARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include "HashMap.h"
#include "AVL.h"
#include <sstream>
#include "Word.h"
#include <stdio.h>
#include <string.h>

using namespace std;

class SecondaryParser 
{
public:
	SecondaryParser();
	SecondaryParser(FileIndex* referenceIndex);
	~SecondaryParser();

	void parseFile(string fileName);
	void parseUserAddedFile(string fileName);
private:

	FileIndex* index;
};

#endif
