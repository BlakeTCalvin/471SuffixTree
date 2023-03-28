#ifndef ST_H
#define ST_H

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
#include <boost/algorithm/string/erase.hpp>

using namespace std;

/***** GLOBALS *****/

string SEQUENCE_HEADER, SEQUENCE;
string ALPHABET;

class ST {
public:
    ST(string fasta, string alphabet) {
        
    }
private:
    struct Node {
        int id;
        int stringDepth;
        Node *parent;
        Node *leftChild;
        Node *rightChild;
        Node *suffixLink;
        string parentEdgeLabel;
        Node(int id, int stringDepth, Node *parent, string parentEdgeLabel) {
            this->id = id;
            this->stringDepth = stringDepth;
            this->parent = parent;
            this->parentEdgeLabel = parentEdgeLabel;
            
            // nullptrs
            this->leftChild = nullptr;
            this->rightChild = nullptr;
            this->suffixLink = nullptr;
        }
    };
};

#endif