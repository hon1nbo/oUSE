//AVL Tree from http://www.sourcecodesworld.com/source/show.asp?ScriptID=1090


#ifndef AVL_H
#define	AVL_H

#include "Word.h"
#include "FileIndex.h"
#include <fstream>

struct node
{
   Word* element_;
   node *left_;
   node *right_;
   int height_;
};

class AVL: public FileIndex {
public:
    AVL();
    AVL(const AVL& orig);
    ~AVL();
    
    void printValues(int i);
    void insert(Word* w) {insert(w, root_);}
    void del(Word* w) {del(w, root_);}
    Word* find(string w) {return find(w, root_);}
    void preorder() {preorder(root_); cout << endl;}
    void inorder() {inorder(root_); cout << endl;}
    void postorder() {postorder(root_); cout << endl;}
    int bsheight() {return bsheight(root_);}
    int nonodes() {int x = 0; return nonodes(root_, x);}
    void clear() {makeempty(root_);}
    void writeToFile(string fileName);    

private:
    void insert(Word*,node* &);
    void del(Word*, node* &);
    Word* find(string,node*);
    void preorder(node*);
    void inorder(node*);
    void postorder(node*);
    int bsheight(node*);
    int nonodes(node* &, int&);
    
    node* srl(node* &);
    node* drl(node* &);
    node* srr(node* &);
    node* drr(node* &);
    
    Word* deletemin(node* &);
    
    void copy(node* const &,node* &);
    node* nodecopy(node*);
    void makeempty(node* &);

    void writeToFile(string fileName, node*, ofstream& outFile);  
    
    node* root_;
};

#endif	/* AVL_H */

