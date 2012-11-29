#include <iostream>
#include <string>
#include "HashMap.h"
#include "Word.h"
#include "AVL.h"
#include "SecondaryParser.h"
#include "Parser.h"
#include "QueryProcessor.h"
#include "UI.h"

using namespace std;

void parseFilesFirst();

int main()
{
	#if defined (_MSC_VER)
			system("cls");
	#else
			system("clear");
	#endif
	
	string temp;
	cout << "Does Index exist? (y/n): ";
	cin >> temp;

	if (temp == "n")
		parseFilesFirst();

	#if defined (_MSC_VER)
			system("cls");
	#else
			system("clear");
	#endif

	cout << "Please select either AVL or HashMap" << endl;
	temp = "";
	cin >> temp;

	UI* userInterface = new UI(temp);

	userInterface->startUI();

	return 0;
}

void parseFilesFirst()
{
	Parser myParser;
	cout << "please enter the path to the file corpus: ";
	string inputFilePath = "";
	getline(cin, inputFilePath);
	getline(cin, inputFilePath);
	myParser.setNumberOfFiles(21);
	myParser.parseFiles(inputFilePath);
}

