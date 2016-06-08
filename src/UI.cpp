/********************************************************
* File: UI.cpp											*
* Authors:												*
*	Jim Hartnett								*
*	Collin Dobmeyer										*
*	Andrew Socha										*
* IP: Search Engine										*
*This file implements the memeber functions of UI.h		*
********************************************************/

#include "UI.h"
#include <fstream>

using namespace std;

UI::UI(string indexType)
{
	if (indexType == "AVL")
		F_ = new AVL;
	else if (indexType == "HashMap")
		F_ = new HashMap;
	else
	{
		cerr << "A valid data structure was not selected" << endl;
	}

	sP_ = new SecondaryParser(F_);
	Q_ = new QueryProcessor(F_, P_);

	currentQuery_ = "";
	currentScreen_ = "start";
	dataStructureType_ = indexType;
	indexFilePath_ = "";
	debuggingPath_ = "C:\\Users\\Mononoke\\Desktop\\documents\\Hartnett-Dobmeyer-Socha-IP-7\\data\\index_GOOD.txt";
	debuggingPath2_ = "C:\\Users\\Mononoke\\Desktop\\documents\\Hartnett-Dobmeyer-Socha-IP-7\\data\\indexSingle.txt";
}

UI::~UI()
{
	delete F_;
	delete sP_;
	delete Q_;
}

void UI::startUI()
{
	clearScreen();
	cout << "Please enter the path to the file index: ";
	getline(cin, indexFilePath_);
	getline(cin, indexFilePath_);

	if (indexFilePath_ == "")
		indexFilePath_ = debuggingPath_;
	else if (indexFilePath_ == "2")
		indexFilePath_ = debuggingPath2_;

	cout << endl;
	cout << "Importing Index at " << indexFilePath_ << "... ";
	sP_->parseFile(indexFilePath_);
	cout << "Done." << endl;

	printTopBar();

	cout << "**************************************************" << endl;
	cout << endl;
	cout << "The engine is currently using a " << dataStructureType_ << " for the data structure" << endl;
	cout << endl;
	bottomScreen();
}

void UI::clearIndex()
{
	delete F_, sP_, Q_;

	if (dataStructureType_ == "AVL")
		F_ = new AVL;
	else if (dataStructureType_ == "HashMap")
		F_ = new HashMap;

	sP_ = new SecondaryParser(F_);
	Q_ = new QueryProcessor(F_, P_);

	clearScreen();

	printTopBar();
	cout << "**************************************************" << endl;
	cout << endl;
	cout << "INDEX CLEARED!!!" << endl;
	cout << endl;

	bottomScreen();

}

void UI::displayResults()
{
	clearScreen();

	currentScreen_ = "displayResults";
	printTopBar();

	cout << "* Results for: " << "\"" << currentQuery_ << "\"" << endl;
	cout << "**************************************************" << endl;
	cout << endl;

	vector<Word::File*> tempList = Q_->getFiles();

	int displaySize = 15;

	if (tempList.size() < 15)
		displaySize = tempList.size();

	for (int i = 0; i < displaySize; i++)
	{
		cout << "* " << (i + 1) << ": " << tempList[i]->fileName << endl;
		cout << "*** " << tempList[i]->day << ", " << tempList[i]->strmonth << " " << tempList[i]->year << endl;
		parseSenderAndGroup(tempList[i]->fileName);
		cout << endl;
	}

	cout << endl;
	bottomScreen();

}

void UI::bottomScreen()
{
	cout << "**************************************************" << endl;
	cout << "* Query Menu                              | oUSE *" << endl;
	if (currentScreen_ == "displayResults")
		cout << "* To open a result, type 'CMDOPEN'               *" << endl;
	cout << "**************************************************" << endl;
	cout << "Query: ";

	string temp;
	getline(cin, temp);
	if (temp == "")
		getline(cin, temp);

	if (temp == "ENTERCOMMAND")
		commandScreen();
	else if (temp == "ADD_FILE_TO_INDEX")
		parseNewFile();
	else if (temp == "SWITCH_DATA_STRUCTURE")
		switchDataStructure();
	else if (temp == "WRITE_INDEX_TO_FILE")
		writeIndexFile();
	else if (temp == "CLEAR_INDEX")
		clearIndex();
	else if (temp == "CMDOPEN")
	{
		cout << endl;
		cout << "Result Number: ";
		cin >> temp;
		openResult(atoi(temp.c_str()));
	}
	else
	{
		currentQuery_ = temp;
		Q_->Query(currentQuery_);
		displayResults();
	}
}

void UI::openResult(int resultNumber)
{

	clearScreen();
	printTopBar();
	cout << "* displaying result #" << resultNumber << endl;
	cout << "**************************************************" << endl;

	vector<Word::File*> tempList = Q_->getFiles();

	Word::File* desiredFile = tempList[ (resultNumber - 1) ];

	fstream myFile;
	myFile.open(desiredFile->fileName);

	string tempLine = "";

	if (!myFile.is_open())
		cerr << "there was a problem opening the file" << endl;
	else
		while (!myFile.eof())
		{
			getline(myFile, tempLine);
			cout << tempLine << endl;
		}

	myFile.close();

	bottomScreen();
}

void UI::commandScreen()
{
	clearScreen();
	currentScreen_ = "commandScreen";
	printTopBar();
	cout << "* This is the command screen. Enter system commands here. *" << endl;
	cout << "**************************************************" << endl;

	cout << "List of Commands: " << endl;
	cout << endl;
	cout << "* 'ADD_FILE_TO_INDEX' -> parse in a new file to index" << endl;
	cout << "* 'SWITCH_DATA_STRUCTURE' -> re-parse in the index to test another data structure" << endl;
	cout << "* 'WRITE_INDEX_TO_FILE' -> write the index to a new file" << endl;
	cout << "* 'CLEAR_INDEX' -> delete the index" << endl;
	cout << endl;

	bottomScreen();
}

void UI::clearScreen()
{
	#if defined (_MSC_VER)
			system("cls");
	#else
			system("clear");
	#endif
}

void UI::printTopBar()
{
	cout << "**************************************************" << endl;
	cout << "* oUSE | open Usenet Search Engine               *" << endl;
}

void UI::parseNewFile()
{
	clearScreen();
	printTopBar();
	currentScreen_ = "parseNewFile";

	cout << "* Add New File to Index: " << endl;\
	cout << "**************************************************" << endl;
	cout << endl;
	cout << "Please enter the new file to be parsed into the index" << endl;
	string temp = "";
	getline(cin, temp);
	cout << "Please enter a suitable path and filename to store the temporary index file" << endl;
	string tempOut = "";
	getline(cin, tempOut);

	P_.parseSingleFile(temp, tempOut);
	sP_->parseFile(tempOut);

	cout << "Secondary parsing completed." << endl;
	cout << endl;

	cout << endl;
	bottomScreen();
}

void UI::switchDataStructure()
{
	clearScreen();
	currentScreen_ = "switchDataStructure";
	printTopBar();
	cout << "* Switch Data Structure                          *" << endl;
	cout << "**************************************************" << endl;
	cout << endl;
	cout << "NOTE: any alternative files added to index after main file must be re-imported!!!" << endl;
	cout << endl;
	cout << "Current Data Structure: " << dataStructureType_ << endl;
	cout << "Switch to a ";

	if (dataStructureType_ == "AVL")
		cout << "HashMap";
	else
		cout << "AVL tree";

	cout << "? (y/n): ";
	string temp = "";
	cin >> temp;
	temp = tolower(temp[0]);

	if (temp == "y")
		performDataSwitch();
	else
		cout << "\n Data Structure switch operation CANCELLED" << endl;

	cout << endl;
	bottomScreen();
}

void UI::performDataSwitch()
{
	if (dataStructureType_ == "AVL")
		dataStructureType_ = "HashMap";
	else
		dataStructureType_ = "AVL";

	delete F_, sP_, Q_;

	if (dataStructureType_ == "AVL")
		F_ = new AVL;
	else
		F_ = new HashMap;

	sP_ = new SecondaryParser(F_);
	Q_ = new QueryProcessor(F_, P_);

	cout << "Importing Index at " << indexFilePath_ << "... ";
	sP_->parseFile(indexFilePath_);
	cout << "Done." << endl;
}

void UI::writeIndexFile()
{
	clearScreen();
	printTopBar();
	cout << "* Write index to file             *" << endl;
	cout << "**************************************************" << endl;
	cout << endl;

	cout << "Please enter a location for the index file" << endl;

	string tempLocation = "";

	getline(cin, tempLocation);

	F_->writeToFile(tempLocation);

	cout << endl;
	cout << "DONE." << endl;
	cout << endl;

	bottomScreen();
}

void UI::parseSenderAndGroup(string fileName)
{
		fstream myFile;

		string groupName = "";
		string from = "";
		string subject = "";

		myFile.open(fileName);

		if (!myFile.is_open())
		{
			cerr << "There was a problem opening file: " << fileName << endl;
			//break; // if we decided to break out of entire parsing operation for each error
		}

		int fieldCount = 1;

		bool finished = false;

		while (!myFile.eof() && !finished)
		{
			string tempLine = "";
			getline(myFile, tempLine);
			string tempWord = "";

			switch (fieldCount)	// this is to help determine what metadata is currently laoded, and proceed properly
			{
				case 1 :
				{
					if (tempLine.find("From:") == 0)
					{
						fieldCount = 2;
						from = tempLine;
					}
					break;
				}
				case 2 :
				{
					if (tempLine.find("Newsgroups:") == 0)
					{
						fieldCount = 3;
						groupName = tempLine;
					}
					break;
				}
				case 3 :
				{
					if (tempLine.find("Subject:") == 0)
					{
						fieldCount = 4;
						subject = tempLine;
						finished = true;
					}
					break;

				}
				default: cout << "ERROR" << endl; break;	// meta data has somehow ben skipped or mishandled
			}
		}	// end while(!myFile.eof())

		cout << "*** " << groupName << endl;
		cout << "*** " << from << endl;
		cout << "*** " << subject << endl;
}
