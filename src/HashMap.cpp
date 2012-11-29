#include "HashMap.h"

using namespace std;

HashMap::HashMap()
{
	
}
HashMap::~HashMap()
{
	//clear();
}
void HashMap::insert(Word* newWord)
{
	if(table.empty())
	{
		table.insert(Hash::value_type(newWord->getWord(), newWord));
	}
	else if(table.find(newWord->getWord()) == table.end())
		table.insert(Hash::value_type(newWord->getWord(), newWord));
	else
	{
		Hash::iterator it = table.find(newWord->getWord());
		vector<Word::File*> vec = newWord->getRanking();
		Word* tempWord = it->second;
		vector<Word::File*> tempRanking = tempWord->getRanking();
		for(int i = 0; i < vec.size(); i++)
		{
			//it->second->getRanking().push_back(vec[i]);
			tempRanking.push_back(vec[i]);
		}
	}
}
void HashMap::printValues(int i)
{
	for(Hash::iterator it = table.begin(); it != table.end(); it++ )
	{
		cout << it->first << " " << it->second->getWord() << endl;
	}
}
void HashMap::del(Word* deleteNode)
{
	Hash::iterator it = table.find("hello");
	if(it != table.end())
		delete it->second;
	table.erase(deleteNode->getWord());
}
Word* HashMap::find(string lookup)
{
	Hash::iterator it = table.find(lookup);
	if(it != table.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}
int HashMap::nonodes()
{
	return table.size();
}
void HashMap::clear()
{
	Hash::iterator it = table.begin();
	while(it != table.end())
	{
		delete it->second;
		table.erase(it->first);
		++it;
	}
}
void HashMap::writeToFile(string fileName)
{
	ofstream outFile;
	outFile.open(fileName.c_str());
	int size = nonodes();
	if(!outFile.is_open())
	{
		cerr << "file not found" << endl;
	}
	else
	{
		outFile << size << endl;                   //number of words total
		Hash::iterator it = table.begin();
		while(it != table.end())
		{
			outFile << it->second->getWord() << endl;              //prints the word
			outFile << it->second->getTotalNumberDocuments() << endl;   //prints the number of files
			vector<Word::File*> temp = it->second->getRanking();
			for(int i = 0; i < temp.size(); i++)
			{
				outFile << temp[i]->fileName << endl;  //writes the filename to document
				outFile << temp[i]->TFIDF << endl;      //TFIDF
				outFile << temp[i]->numberOfAppearances << endl;  //number of appearances in that particular file
				outFile << temp[i]->totalWordsInFile << endl;   //total words in the file
				outFile << temp[i]->year << endl;	// year of post
				outFile << temp[i]->strmonth << endl;	// month of post
				outFile << temp[i]->day << endl;		// day of post
			}
			
			it++;
		}
		outFile.close();
	}
}
