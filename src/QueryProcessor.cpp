//Query Processor written by Andrew Socha

#include "QueryProcessor.h"

bool sortFunction(Word::File* i, Word::File* j) {return *i>*j;} //used to sort files_ by TFIDF (greatest to least)

QueryProcessor::QueryProcessor(FileIndex* index, /*const*/ Parser& parser) {
	FI = index;
	P = parser;
}
/*
QueryProcessor::QueryProcessor(const QueryProcessor& orig) {
}
*/
const QueryProcessor& QueryProcessor::operator=(const QueryProcessor& rhs){
	return *this;
}

QueryProcessor::~QueryProcessor() {
}

void QueryProcessor::Query(string query){

	files_.clear();

	stringstream queryStream(query);
    string first, next; //first is used to obtain AND and OR search terms, as well as the first word to be queried
                        //next is used to obtain NOT, DATEGT, and DATELT search terms, as well as every other word to be queried
    
    //all return statements mean we have reached the end of the query
    
    queryStream >> first;
    if (first == "AND"){ //AND          note: only AND needs to worry about stop words as ORing or NOTing with a stop word will not change the file set
        queryStream >> first;
        while (P.checkStopWord(first)){
            queryStream >> first;
            if (first == "NOT" || first == "DATEGT" || first == "DATELT") return;
            if (queryStream.eof()){
                if (P.checkStopWord(first)) return;
                else{
                    first = P.stemWord(first);
                    firstWord(FI->find(first));
                    return;
                }
            }
        }
        first = P.stemWord(first);
        firstWord(FI->find(first));
        
        queryStream >> next;
        while (next != "NOT" && next != "DATEGT" && next != "DATELT"){
            next = P.stemWord(next);
            if (!P.checkStopWord(next)) And(FI->find(next));
            if (queryStream.eof()) return;
            queryStream >> next;
        }
    }
    else if (first == "OR"){ //OR
        queryStream >> first;
        first = P.stemWord(first);
        firstWord(FI->find(first));
        
        queryStream >> next;
        while (next != "NOT" && next != "DATEGT" && next != "DATELT"){
            next = P.stemWord(next);
            Or(FI->find(next));
            if (queryStream.eof()) return;
            queryStream >> next;
        }
    }
    else{ //single word search
        first = P.stemWord(first);
        if (!P.checkStopWord(first)) firstWord(FI->find(first));
        else return;
        queryStream >> next;
    }
    
    if (next == "NOT"){ //NOT
        queryStream >> next;
        while (next != "DATEGT" && next != "DATELT"){
            next = P.stemWord(next);
            Not(FI->find(next));
            if (queryStream.eof()) return;
            queryStream >> next;
        }
    }
    
    if (next == "DATEGT"){ //Date Greater Than
        queryStream >> next;
        DateGreater(next);
        queryStream >> next;
    }
    
    if (next == "DATELT"){ //Date Lesser Than
        queryStream >> next;
        DateLesser(next);
        queryStream >> next;
    }
    
    if (next == "DATEGT"){ //repeats Date Greater Than incase user does Less Than followed by Greater Than
        queryStream >> next;
        DateGreater(next);
    }
}

void QueryProcessor::firstWord(Word* word){
    if (word == 0) return;
    vector<Word::File*> index = word->getRanking();
    for(int i = 0; i < index.size(); i++){  //initializes files_ with the files containing the given word
        files_.push_back(index[i]);
    }
    sort(files_.begin(), files_.end(), sortFunction);
}

//takes each file in files_ and compares it to each file with the given word, removing the file if it does not contain that word
void QueryProcessor::And(Word* word){
    if (word == 0){
        files_.clear();
        return;
    }
    vector<Word::File*> index = word->getRanking();
    for (int j = 0; j < files_.size(); j++){
        bool wasFound = false;
        for (int i = 0; i < index.size(); i++){
            if (files_[j]->fileName == index[i]->fileName){
                wasFound = true;
                files_[j]->TFIDF += index[i]->TFIDF;      //when a file contains more than one word from the search, adds their TFIDFs

                i = index.size();      //breaks out of the inner for loop
            }
        }
        if (!wasFound){
            files_.erase(files_.begin() + j);
            j--;
        }
    }
    sort(files_.begin(), files_.end(), sortFunction);
}

//takes each file with the given word and checks if that file is already in files_. if it is not, inserts it
void QueryProcessor::Or(Word* word){
    if (word == 0) return;
    vector<Word::File*> index = word->getRanking();
    for(int i = 0; i < index.size(); i++){
        int j = 0;
        while (j < files_.size() && index[i]->fileName != files_[j]->fileName) j++;         //checks all files from files_ from the file from index
        if (j < files_.size()){ //if the file is found...
            files_[j]->TFIDF += index[i]->TFIDF;      //when a file contains more than one word from the search, adds their TFIDFs
        }
        else{ //else, inserts the file in files_
            files_.push_back(index[i]);
        }
    }
    sort(files_.begin(), files_.end(), sortFunction);
}

//gets all files with the given word and compares each file to the files in files_, removing all files from files_ that contain the word
void QueryProcessor::Not(Word* word){
    if (word == 0) return;
    vector<Word::File*> index = word->getRanking();
    for(int i = 0; i < index.size(); i++){
        for (int j = 0; j < files_.size(); j++){
            if (files_[j]->fileName == index[i]->fileName){
                files_.erase(files_.begin() + j);
                j--;
            }
        }
    }
}

//removes files dated less than the given date
void QueryProcessor::DateGreater(string date){  //date in the form mm/dd/yyyy
    stringstream dateStream(date);
    int month, day, year;
    dateStream >> month;
    dateStream.ignore();
    dateStream >> day;
    dateStream.ignore();
    dateStream >> year;
    
    for (int j = 0; j < files_.size(); j++){    //loops through files_, removing all files with a date lesser than the given date
        if (files_[j]->year < year) files_.erase(files_.begin() + j);
        else if (files_[j]->year == year && files_[j]->month < month) files_.erase(files_.begin() + j);
        else if (files_[j]->year == year && files_[j]->month == month && files_[j]->day <= day) files_.erase(files_.begin() + j);
        
        //the following 2 lines decrement j only if a File has been deleted (this is to prevent the loop from skipping a file)
        else j++;
        j--;
    }
}

//removes files dated greater than the given date
void QueryProcessor::DateLesser(string date){   //date in the form mm/dd/yyyy
    stringstream dateStream(date);
    int month, day, year;
    dateStream >> month;
    dateStream.ignore();
    dateStream >> day;
    dateStream.ignore();
    dateStream >> year;
    
    for (int j = 0; j < files_.size(); j++){    //loops through files_, removing all files with a date greater than the given date
        if (files_[j]->year > year) files_.erase(files_.begin() + j);
        else if (files_[j]->year == year && files_[j]->month > month) files_.erase(files_.begin() + j);
        else if (files_[j]->year == year && files_[j]->month == month && files_[j]->day >= day) files_.erase(files_.begin() + j);
        
        //the following 2 lines decrement j only if a File has been deleted (this is to prevent the loop from skipping a file)
        else j++;
        j--;
    }
}

vector<Word::File*> QueryProcessor::getFiles(){
    return files_;
}