/*File: Word.h
 *Author: Collin Dobmeyer
 *Project: Search Engine
 *this file inplements the word object which stores the member data of words parsed in from the file index
 */

#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Word
{
public:
	struct File {
		string fileName;
		int numberOfAppearances;
		double TFIDF;
		int totalWordsInFile;
		int year, month, day;
		string strmonth;
		bool operator<(const File& rhs)
		{
			return this->TFIDF < rhs.TFIDF;
		}
		bool operator>(const File& rhs)
		{
			return this->TFIDF > rhs.TFIDF;
		}
		friend ostream& operator<<(ostream& out, const File& file)
		{
			out << file.fileName << endl
			    << file.TFIDF << endl
			    << file.numberOfAppearances << endl
			    << file.totalWordsInFile << endl
			    << file.year << " " << file.month << " " << file.day << endl;
		}
	};
	
	Word(); 
	Word(string newWord);
	Word(string newWord, double newTFIDF);
	~Word();
	void setWord(string newWord);
	string getWord();
	void rankTFIDF();
	void calculateTFIDF(int totalNumberOfDocuments);
	void addFile(string newFileName);
	void addFile(string newFileName, int year, string month, int day);
	void addFile(string newFileName, double newTFIDF);
	void addFile(string newFileName, double newTFIDF, int newNumberAppearances);
	void addFile(string newFileName, double newTFIDF, int newNumberAppearances, int newTotalWordsInFile);
	void addFile(string newFileName, double newTFIDF, int newNumberAppearances, int newTotalWordsInFile, int year, int month, int day);
	void addFile(string newFileName, double newTFIDF, int newNumberAppearances, int newTotalWordsInFile, int year, string month, int day);
	int calculateMonth(string month);
	int getTotalNumberDocuments();
	vector<File*>& getRanking();
	
private:
	
	string word;
	vector<File*> ranking;
};

#endif
