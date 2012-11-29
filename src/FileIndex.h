#ifndef FILEINDEX_H
#define FILEINDEX_H
#include <iostream> 
#include "Word.h"


using namespace std;

class FileIndex
{
public:
	virtual ~FileIndex() {}

	//virtual void insert(string key, Word* newWord) = 0;
	virtual void insert(Word* newWord) = 0;
	virtual void printValues(int i) = 0;
	virtual void del(Word* deleteValue) = 0;
	virtual Word* find(string lookup) = 0;
	virtual int nonodes() = 0;          //returns the number of elements in the structure dont ask me why its named this
	virtual void clear() = 0;
	virtual void writeToFile(string fileName)=0;
	void incrementCorpusSize() { corpusSize++; }
	int getCorpusSize(){ return corpusSize; }
	void setCorpusSize(int newCorpusSize) { corpusSize = newCorpusSize; }
	int getTotalNumberOfWords() { return totalNumberOfWords; }
	void setTotalNumberOfWords(int newTotalNumberOfWords) { totalNumberOfWords = newTotalNumberOfWords; }
	
private:
	int corpusSize;
	int totalNumberOfWords;
};
#endif
