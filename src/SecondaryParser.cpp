#include "SecondaryParser.h"

using namespace std;

SecondaryParser::SecondaryParser()
{

}
SecondaryParser::SecondaryParser(FileIndex* referenceIndex)
{
	index = referenceIndex;
}
SecondaryParser::~SecondaryParser()
{

}
void SecondaryParser::parseFile(string fileName)
{
	fstream infile;
	infile.open(fileName.c_str());
	if(!infile)
	{
		cerr << "file not found" << endl;
	}
	else
	{
		string str = "";
		getline(infile, str);
		index->setCorpusSize(atoi(str.c_str())); // retrieves the number of posts 
		getline(infile, str);   //retrieves the second line of the file with number of words
		int loopCounterFromFile = atoi(str.c_str());
		for(int i = 0; i < loopCounterFromFile; i++)
		{
			/*getline(infile, str);           //number of words contained in the file index
			int numberOfWords = atoi(str.c_str());
			index->setTotalNumberOfWords(numberOfWords);
			getline(infile, str);          //corpus size
			int docsize = atoi(str.c_str());
			index->setCorpusSize(docsize);*/
			getline(infile, str);           //retrieves the first word in the file
			Word* temp = new Word(str);
			getline(infile, str);           //# of files containing the word
			int loopCounterFromFile2 = atoi(str.c_str());//converts # of files to an int
			for(int i = 0; i < loopCounterFromFile2; i++)
			{
				getline(infile, str); //filename;

				string str2;
				getline(infile, str2);//TFIDF ranking double
				double d1 = atof(str2.c_str());

				string str3;	
				getline(infile, str3);  //number of appearances in that file int 
				int i1 = atoi(str3.c_str());

				string str4;
				getline(infile, str4);    //total number of words in the file int
				int i2 = atoi(str4.c_str());
				
				string yeartemp;
				getline(infile, yeartemp);
				int year = atoi(yeartemp.c_str());

				string monthtemp;
				getline(infile, monthtemp);
				int month = atoi(monthtemp.c_str());

				string daytemp;
				getline(infile, daytemp);
				int day = atoi(daytemp.c_str());

				temp->addFile(str, d1, i1, i2, year, monthtemp, day);
			}
			index->insert(temp);
		}
	}
	infile.close();
}
void SecondaryParser::parseUserAddedFile(string fileName)
{
	fstream infile;
	infile.open(fileName.c_str());
	if(!infile)
	{
		cerr << "file not found" << endl;
	}
	else
	{
	
	}
	infile.close();
}
