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
        this->nextInternalID = this->leaves + 1;
        this->nextLeafID = 1;

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
        Node *v = u;
        Node *currentChild; // for keeping track of child node if edge gets exhausted
        string x = SEQUENCE.substr(index);
        int flag;
        int r = index; // used for keeping track of index if edge gets exhausted

        // continue loop until a return statement is hit
        while (1) {
            flag = 0; // reset flag
            // searching child branch that starts with x[0]
            for (Node *node : v->children) {
                if (node->parentEdgeLabel[0] == x[0]) { // child branch starts with x[0]
                    flag == 1; // update flag to know we found an edge we can reuse
                    currentChild = node; // update current child, in case edge gets exhausted

                    // loop through parentEdgeLabel and check for matches on x
                    for (int i=0; i<node->parentEdgeLabel.length(); i++) {
                        if (node->parentEdgeLabel[i] != x[i]) { // mismatch
                            // break edge
                            string edgePart1 = node->parentEdgeLabel.substr(0, i-1);
                            string edgePart2 = node->parentEdgeLabel.substr(i);
                            string newLeafEdge = x.substr(i);

                            // remove old child from u
                            for (auto it = v->children.begin(); it != v->children.end(); ++it) {
                                if (*it == node) { // correct node is found in children vector
                                    v->children.erase(it);
                                    break;
                                }
                            }

                            // new internal node
                            Node *newInternalNode = new Node(nextInternalID, v->stringDepth+1, v, edgePart1);
                            nextInternalID++;

                            // make internal node new child
                            v->children.push_back(newInternalNode);

                            // update child node 
                            node->parent = newInternalNode;
                            node->stringDepth++;
                            node->parentEdgeLabel = edgePart2;

                            // new leaf node
                            Node *newLeafNode = new Node(nextLeafID, node->stringDepth, newInternalNode, newLeafEdge);
                            nextLeafID++;

                            // adding both new nodes to internals children
                            newInternalNode->children.push_back(node);
                            newInternalNode->children.push_back(newLeafNode);
                            return;
                        }
                        else {
                            r++; // increment r each time we find a match
                        }
                    }

                    // parentEdgeLabel was exhausted, return never hit
                    v = currentChild;
                    x = SEQUENCE.substr(r);

                    break; // break the loop, we already found the child
                }
            }

            // flag never updated, no child has edge label start with x[0]. Insert new child under u
            if (flag == 0) {
                Node *newNode = new Node(nextLeafID, v->stringDepth+1, v, x);
                v->children.push_back(newNode);
                nextLeafID++;
                return;
            }
        }
    }

    void initializeTree() { //TODO
        this->root = new Node(nextInternalID, 0, nullptr, " ");
        this->root->suffixLink = root;
        nextInternalID++;
    }

private:
    Node *root;
    // tree statistics
    int internalNodes, leaves, totalNodes, averageStringDepth, deepestStringDepth, nextInternalID, nextLeafID;
};

#endif