
#ifndef QUERYPROCESSOR_H
#define	QUERYPROCESSOR_H

#include <sstream>
#include "Word.h"
#include <vector>
#include "FileIndex.h"
#include "Parser.h"

using namespace std;

class QueryProcessor {
public:
    QueryProcessor(FileIndex*, /*const*/ Parser&);
    virtual ~QueryProcessor();

	void Query(string query);
    vector<Word::File*> getFiles();
private:
   // QueryProcessor(const QueryProcessor& orig);
    const QueryProcessor& operator=(const QueryProcessor& rhs);
    
    void firstWord(Word* word);
    void And(Word* word);
    void Or(Word* word);
    void Not(Word* word);
    void DateGreater(string date);
    void DateLesser(string date);
    
    vector<Word::File*> files_;

	FileIndex* FI;
	Parser P;
};

#endif	/* QUERYPROCESSOR_H */

