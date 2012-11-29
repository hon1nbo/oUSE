/*File: Word.cpp
 *Author: Collin Dobmeyer
 *Project: Search Engine
 *this file contains the functionality of word.h
 */

#include "Word.h"
//#include <math.h>

using namespace std;

bool myFunction(Word::File* i, Word::File* j) {return *i>*j;}

Word::Word()
{

}
Word::Word(string newWord)
{
	word = newWord;
}
Word::~Word()
{
	for(int i = 0; i < ranking.size(); i++)
	{
		delete ranking[i];
	}
}
void Word::setWord(string newWord)
{
	word = newWord;
}
string Word::getWord()
{
	return word;
}
void Word::addFile(string newFileName)
{
	File* temp = new File;
	temp->fileName = newFileName;
	temp->TFIDF = 0;
	temp->numberOfAppearances = 1;
	temp->totalWordsInFile = 1;
	temp->year = 0;
	temp->month = 0;
	temp->day = 0;
	ranking.push_back(temp);
}
void Word::addFile(string newFileName, int year, string month, int day)
{
	File* temp = new File;
	temp->fileName = newFileName;
	temp->TFIDF = 0;
	temp->numberOfAppearances = 1;
	temp->totalWordsInFile = 1;
	temp->year = year;
	temp->strmonth = month;
	temp->day = day;
	ranking.push_back(temp);
}
void Word::addFile(string newFileName, double newTFIDF)
{
	File* temp = new File;
	temp->fileName = newFileName;
	temp->TFIDF = newTFIDF;
	temp->numberOfAppearances = 1;
	temp->totalWordsInFile = 1;
	ranking.push_back(temp);
}
void Word::addFile(string newFileName, double newTFIDF, int newNumberAppearances)
{
	File* temp = new File;
	temp->fileName = newFileName;
	temp->TFIDF = newTFIDF;
	temp->numberOfAppearances = newNumberAppearances;
	temp->totalWordsInFile = 1;
	temp->year = 0;
	temp->month = 0;
	temp->day = 0;
	ranking.push_back(temp);
}
void Word::addFile(string newFileName, double newTFIDF, int newNumberAppearances, int newTotalWordsInFile)
{
	File* temp = new File;
	temp->fileName = newFileName;
	temp->TFIDF = newTFIDF;
	temp->numberOfAppearances = newNumberAppearances;
	temp->totalWordsInFile = newTotalWordsInFile;
	temp->year = 0;
	temp->month = 0;
	temp->day = 0;
	ranking.push_back(temp);
}
void Word::addFile(string newFileName, double newTFIDF, int newNumberAppearances, int newTotalWordsInFile, int newYear, int newMonth, int newDay)
{
	File* temp = new File;
	temp->fileName = newFileName;
	temp->TFIDF = newTFIDF;
	temp->numberOfAppearances = newNumberAppearances;
	temp->totalWordsInFile = newTotalWordsInFile;
	temp->year = newYear;
	temp->month = newMonth;
	temp->day = newDay;
	ranking.push_back(temp);
}
void Word::addFile(string newFileName, double newTFIDF, int newNumberAppearances, int newTotalWordsInFile, int newYear, string newMonth, int newDay)
{
	File* temp = new File;
	temp->fileName = newFileName;
	temp->TFIDF = newTFIDF;
	temp->numberOfAppearances = newNumberAppearances;
	temp->totalWordsInFile = newTotalWordsInFile;
	temp->year = newYear;
	temp->month = calculateMonth(newMonth);
	temp->strmonth = newMonth;
	temp->day = newDay;
	ranking.push_back(temp);
}
void Word::rankTFIDF()
{
	vector<File*> copyVector;
	sort(ranking.begin(), ranking.end(), myFunction);
/*
	for(int i = 0; i < ranking.size(); i++)
	{
		cout << ranking[i]->fileName << " " << ranking[i]->TFIDF << endl;
	}
*/
}
void Word::calculateTFIDF(int totalNumberOfDocuments)
{
	for(int i = 0; i < ranking.size(); i++)
	{
		//term frequency calculation
		ranking[i]->TFIDF = (double)ranking[i]->numberOfAppearances / (double)ranking[i]->totalWordsInFile * log10((double)totalNumberOfDocuments / (double)ranking.size());
	}
	rankTFIDF();//sorts the containing vector
}
int Word::getTotalNumberDocuments()
{
	return ranking.size();
}
vector<Word::File*>& Word::getRanking()
{
	rankTFIDF();
	return ranking;
}
int Word::calculateMonth(string month)
{
	for(int i = 0; i < month.size(); i++)
	{
		month[i] = tolower(month[i]);
	}
	if(month == "jan") return 1;
	else if(month == "feb") return 2;
	else if(month == "mar") return 3;
	else if(month == "apr") return 4;
	else if(month == "may") return 5;
	else if(month == "jun") return 6;
	else if(month == "jul") return 7;
	else if(month == "aug") return 8;
	else if(month == "sep") return 9;
	else if(month == "oct") return 10;
	else if(month == "nov") return 11;
	else if(month == "dec") return 12;
}
