/********************************************************
* File: UI.h											*
* Authors:												*
*	James (Jimmy) Hartnett								*
*	Collin Dobmeyer										*
*	Andrew Socha										*
* IP: Search Engine										*
*This file implements the class structure of the Usenet	*
* user interface for the search engine					*
********************************************************/

#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "HashMap.h"
#include "Word.h"
#include "AVL.h"
#include "SecondaryParser.h"
#include "Parser.h"
#include "QueryProcessor.h"

using namespace std;

class UI
{
	public:
		UI(string);
		~UI();
		void startUI();

	private:

		void bottomScreen();
		void printTopBar();
		void displayResults();
		void commandScreen();
		void parseNewFile();
		void switchDataStructure();
		void clearScreen();
		void performDataSwitch();
		void openResult(int);
		void writeIndexFile();
		void parseSenderAndGroup(string);
		void clearIndex();

		string currentQuery_;
		string currentScreen_;
		string dataStructureType_;
		string indexFilePath_;
		string debuggingPath_;
		string debuggingPath2_;

		Parser P_;
		FileIndex* F_;
		QueryProcessor* Q_;
		SecondaryParser* sP_;

};
#endif