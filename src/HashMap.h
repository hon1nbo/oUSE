#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "Word.h"
#include "FileIndex.h"
#include <fstream>

using namespace std;

class HashMap: public FileIndex {
public:
	HashMap();
	~HashMap();

	void insert(Word* newWord);  //adds a value to hash map key represents value to be hashed value is a ptr to a dynamic word
	void printValues(int i);               //prints values out no order does not handle buckets
	void del(Word* deleteValue);
	Word* find(string lookup);
	int nonodes();
	void clear();
	void writeToFile(string fileName);
private:
	typedef unordered_map<string, Word*> Hash;
	Hash table;
};

#endif
