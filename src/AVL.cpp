//AVL Tree from http://www.sourcecodesworld.com/source/show.asp?ScriptID=1090
//edited greatly


#include "AVL.h"
#include <iostream>
#include <math.h>

using namespace std;

AVL::AVL() {
    root_ = NULL;
}

AVL::AVL(const AVL& orig) {
    root_ = NULL;
    copy(orig.root_, root_);
}

AVL::~AVL() {
    makeempty(root_);
}

//		Inserting a node
void AVL::printValues(int i)
{
	if(i == 1) { preorder(); }
	if(i == 2) { postorder(); }
	if(i == 3) { inorder(); }
}
void AVL::insert(Word* x,node* &p)
{
   if (p == NULL)
   {
	p = new node;
	p->element_ = x;
	p->left_ = NULL;
	p->right_ = NULL;
	p->height_ = 0;
	if (p==NULL) cout << "Out of Space" << endl;
   }
   else
   {
	if (x->getWord()<p->element_->getWord())
	{
            insert(x,p->left_);
            if ((bsheight(p->left_) - bsheight(p->right_))==2)
            {
                if (x->getWord() < p->left_->element_->getWord())
                    p = srl(p);
                else
                    p = drl(p);
            }
	}
	else if (x->getWord() > p->element_->getWord())
	{
            insert(x,p->right_);
            if ((bsheight(p->right_) - bsheight(p->left_))==2)
            {
                if (x->getWord() > p->right_->element_->getWord())
                    p = srr(p);
                else
                    p = drr(p);
            }
	}
        else 
		{
			vector<Word::File*> xFiles = x->getRanking();
			for (int i = 0; i < xFiles.size(); i++)
			{
				p->element_->getRanking().push_back(xFiles[i]);
			}
		}
    }
    p->height_ = max(bsheight(p->left_),bsheight(p->right_)) + 1;
}

//		Finding an element
Word* AVL::find(string x,node* p)
{
    if (p==NULL)
        return NULL;
    else if (x < p->element_->getWord())
        return find(x,p->left_);
    else if (x > p->element_->getWord())
        return find(x,p->right_);
    else
        return p->element_;
}

//		Copy a tree
void AVL::copy(node* const &p,node* &p1)
{
    makeempty(p1);
    p1 = nodecopy(p);
}

//		Make a tree empty
void AVL::makeempty(node* &p)
{
    if (p != NULL)
    {
        makeempty(p->left_);
        makeempty(p->right_);
        delete p;
        p=NULL;
    }
}

//Used by copy to Copy the nodes
node* AVL::nodecopy(node* p)
{
    if (p==NULL)
        return p;
    else
    {
        node* temp = new node;
        temp->element_ = p->element_;
        temp->height_ = p->height_;
        temp->left_ = nodecopy(p->left_);
        temp->right_ = nodecopy(p->right_);
        return temp;
    }
}

//Deleting a node
void AVL::del(Word* x,node* &p)
{
    node* d;
    if (p==NULL)
        cout<<"element not found" << endl;
    else if (x->getWord() < p->element_->getWord()){
        del(x,p->left_);
        p->height_ = max(bsheight(p->left_),bsheight(p->right_)) + 1;
        if ((bsheight(p->right_) - bsheight(p->left_))==2)
        {
            if (x->getWord() > p->right_->element_->getWord())
                p = srr(p);
            else
                p = drr(p);
        }
    }
    else if (x->getWord() > p->element_->getWord()){
        del(x,p->right_);
        p->height_ = max(bsheight(p->left_),bsheight(p->right_)) + 1;
        if ((bsheight(p->left_) - bsheight(p->right_))==2)
        {
            if (x->getWord() < p->left_->element_->getWord())
                p = srl(p);
            else
                p = drl(p);
        }
    }
    else if ((p->left_ == NULL) && (p->right_ == NULL))
    {
        delete p;
        p=NULL;
        cout<<"element deleted!"<<endl;
    }
    else if (p->left_ == NULL)
    {
        d=p;
        p=p->right_;
        delete d;
        cout<<"element deleted!"<<endl;
    }
    else if (p->right_ == NULL)
    {
        d=p;
        p=p->left_;
        delete d;
        cout<<"element deleted!"<<endl;
    }
    else{
        p->element_ = deletemin(p->right_);
        cout<<"element deleted!"<<endl;
        if ((bsheight(p->left_) - bsheight(p->right_))==2)
        {
            if (p->left_->right_ == NULL)
                p = srl(p);
            else{
                p = drl(p);
            }
        }
    }
}

//used by delete when the node to be deleted has 2 children
Word* AVL::deletemin(node* &p)
{
    Word* c;
    if (p->left_ == NULL)
    {
        c=p->element_;
        p=p->right_;
        return c;
    }
    else
    {
        c=deletemin(p->left_);
        p->height_ = max(bsheight(p->left_),bsheight(p->right_)) + 1;
        if ((bsheight(p->right_) - bsheight(p->left_))==2)
        {
            if (p->right_->left_ == NULL)
                p = srr(p);
            else{
                p = drr(p);
            }
        }
        return c;
    }
}

void AVL::preorder(node* p)
{
    if (p!=NULL)
    {
        cout<<p->element_->getWord()<<"-->";
        preorder(p->left_);
        preorder(p->right_);
    }
}

//		Inorder Printing
void AVL::inorder(node* p)
{
    if (p!=NULL)
    {
        inorder(p->left_);
        cout<<p->element_->getWord()<<" at height "<<p->height_<<"-->";
        inorder(p->right_);
    }
}

//		PostOrder Printing
void AVL::postorder(node* p)
{
    if (p!=NULL)
    {
        postorder(p->left_);
        postorder(p->right_);
        cout<<p->element_->getWord()<<"-->";
    }
}

int AVL::bsheight(node* p)
{
    if (p == NULL)
        return -1;
    else
    {
        return p->height_;
    }
}

node* AVL:: srl(node* &p1)
{
    node* p2 = p1->left_;
    p1->left_ = p2->right_;
    p2->right_ = p1;
    p1->height_ = max(bsheight(p1->left_),bsheight(p1->right_)) + 1;
    p2->height_ = max(bsheight(p2->left_),p1->height_) + 1;
    return p2;
}

node* AVL:: srr(node* &p1)
{
    node* p2 = p1->right_;
    p1->right_ = p2->left_;
    p2->left_ = p1;
    p1->height_ = max(bsheight(p1->left_),bsheight(p1->right_)) + 1;
    p2->height_ = max(p1->height_,bsheight(p2->right_)) + 1;
    return p2;
}


node* AVL:: drl(node* &p1)
{
    p1->left_=srr(p1->left_);
    return srl(p1);
}

node* AVL::drr(node* &p1)
{
    p1->right_ = srl(p1->right_);
    return srr(p1);
}

//counts the number of nodes
int AVL::nonodes(node* &p, int& count)
{
    if (p!=NULL)
    {
        nonodes(p->left_, count);
        nonodes(p->right_, count);
        count++;
    }
    return count;
}
void AVL::writeToFile(string fileName)
{
	ofstream outFile;
	outFile.open(fileName.c_str());
	int size = nonodes();
	if(!outFile.is_open())
	{
		cout << "file not found" << endl;
	}
	else
	{
		outFile<<size<<endl;
		writeToFile(fileName, root_, outFile);
		outFile.close();
	}
}

void AVL::writeToFile(string fileName, node* p, ofstream& outFile){
    if (p!=NULL)
    {
	Word* word = p->element_;
	writeToFile(fileName, p->left_, outFile);
	outFile << word->getWord() << endl;      //writes word to file
	vector<Word::File*> out = word->getRanking();
	int size = word->getTotalNumberDocuments();
	outFile << size << endl;                      //writes number of files to document
	for(int i = 0; i < size; i++)
	{
		outFile << out[i]->fileName << endl;  //writes the filename to document
		outFile << out[i]->TFIDF << endl;      //TFIDF
		outFile << out[i]->numberOfAppearances << endl;  //number of appearances in that particular file
		outFile << out[i]->totalWordsInFile << endl;    //total words in the file
		outFile << out[i]->year << endl;	// year of post
		outFile << out[i]->strmonth << endl;	// month of post
		outFile << out[i]->day << endl;		// day of post
	}
	writeToFile(fileName, p->right_, outFile);
    }


}
