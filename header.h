#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cctype>
#include <algorithm>
#include <initializer_list>
#include <cmath>
#include <vector>
#include <boost/algorithm/string/erase.hpp>

using namespace std;

/***** GLOBALS *****/

// sequence/alphabet
string SEQUENCE_HEADER, SEQUENCE;
string ALPHABET;
int SEQUENCE_LENGTH;

/***** FUNCTION DECLARATIONS *****/
void setAlphabet(string file);
void setSequence(string file);

/****** NODE STRUCT AND ST CLASS ******/
struct Node {
    int id;
    int stringDepth;
    Node *parent;
    Node *suffixLink;
    vector<Node*> children;
    string parentEdgeLabel;
    
    Node(int id, int stringDepth, Node *parent, string parentEdgeLabel) {
        this->id = id;
        this->stringDepth = stringDepth;
        this->parent = parent;
        this->parentEdgeLabel = parentEdgeLabel;
        
        // nullptr
        this->suffixLink = nullptr;
    }
};

class ST {
public:
    ST(string fasta, string alphabet) {
        setAlphabet(alphabet);
        setSequence(fasta);
        
        this->leaves = SEQUENCE.length();
        this->nextID = this->leaves + 1;

        initializeTree();
    }

    ~ST() { }

    // getters
    int getLeaves() { return this->leaves; }

    int getNextInternalID() { return this->nextInternalID; }

    int getNextLeafID() { return this->nextLeafID; }

    // other functions
    void displayChildren(Node *node) { // TODO 
    }

    void findPath(Node *u, int index) {
        Node *v = u->parent;
        string x = SEQUENCE.substr(index);

        if(u->children.empty()) {
            Node *newNode = new Node(nextLeafID, 0, nullptr, " ");
            u->children.push_back(newNode);
        }
        for (Node *node : u->children) {

        }
    }

    void initializeTree() { //TODO
        this->root = new Node(nextInternalID, 0, nullptr, " ");
        this->root->suffixLink = root;
        this->nextInternalID++;
    }

private:
    Node *root;
    // tree statistics
    int internalNodes, leaves, totalNodes, averageStringDepth, deepestStringDepth, nextInternalID, nextLeafID;
};

#endif