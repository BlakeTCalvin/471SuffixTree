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
// BWT
vector<char> BWT;

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
    // public function i
    ST(string fasta, string alphabet) {
        setAlphabet(alphabet);
        setSequence(fasta);
        
        this->nextInternalID = SEQUENCE.length() + 1;
        this->nextLeafID = 1;
        this->deepestStringDepth = 0;
        this->totalInternalDepth = 0;

        int start = -1;

        initializeTree();
        buildST();
        enumerateNodes(root, start);
        generateBWT(root);
        
        char temp = BWT.back();
        BWT.insert(BWT.begin(), temp);
        BWT.pop_back();
    }

    ~ST() { }

    // getters
    int getLeaves() { return this->leaves; }

    int getNextInternalID() { return this->nextInternalID; }

    int getNextLeafID() { return this->nextLeafID; }

    int getInternalNodes() { return this->internalNodes; }

    int getLeafNodes() { return this->leaves; }

    int getTotalNodes() { return this->totalNodes; }

    int getDeepestStringDepth() { return this->deepestStringDepth; }

    int getAverageInternalDepth() { return this->averageInternalDepth; }

    // other functions
    // public function ii
    void displayChildren(Node *node) { 
        cout << "Node: " << node->id << "'s children from left to right are: " << endl;
        for (Node *child : node->children) {
            cout << "Node: " << child->id << " | ";
        }
    }

    // public function iii
    void enumerateNodes(Node *node, int& counter) { // Depth First Search Enumeration
        counter++; // increase counter for each node
        if (counter % 7 == 0) {
            cout << endl;
        }
        cout << "Node ID: " << node->id << " has depth " << node->stringDepth << " | ";

        for (Node *child : node->children) {
            enumerateNodes(child, counter);
        }
    }

    // public function iv
    void generateBWT(Node *node) {
        if (node->children.empty()) {
            if (node->id - 1 > 0) {
                BWT.push_back(SEQUENCE[node->id - 2]);
            }
            else {
                BWT.push_back(SEQUENCE[SEQUENCE.length() - 1]);
            }
        }
        else {
            for (Node *child : node->children) {
                generateBWT(child);
            }
        }
    }

    Node *findPath(Node *u, int index) {
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
                    flag = 1; // update flag to know we found an edge we can reuse
                    currentChild = node; // update current child, in case edge gets exhausted

                    // loop through parentEdgeLabel and check for matches on x
                    for (int i=1; i<currentChild->parentEdgeLabel.length(); i++) {
                        if (currentChild->parentEdgeLabel[i] != x[i]) { // mismatch
                            // break edge
                            string edgePart1 = currentChild->parentEdgeLabel.substr(0, i);
                            string edgePart2 = currentChild->parentEdgeLabel.substr(i);
                            string newLeafEdge = x.substr(i);

                            // remove old child from u
                            for (auto it = v->children.begin(); it != v->children.end(); ++it) {
                                if (*it == currentChild) { // correct node is found in children vector
                                    v->children.erase(it);
                                    break;
                                }
                            }

                            // new internal node
                            Node *newInternalNode = new Node(nextInternalID, v->stringDepth + edgePart1.length(), v, edgePart1);
                            internalNodes++;
                            nextInternalID++;

                            // make internal node new child and order the children using lambda function to balance based off of the parentEdgeLabels 
                            v->children.push_back(newInternalNode);
                            sort(v->children.begin(), v->children.end(), [](Node *node1, Node *node2) { 
                                return node1->parentEdgeLabel[0] < node2->parentEdgeLabel[0]; 
                                });

                            // update child node 
                            currentChild->parent = newInternalNode;
                            currentChild->stringDepth = newInternalNode->stringDepth + edgePart2.length();
                            currentChild->parentEdgeLabel = edgePart2;

                            // new leaf node
                            Node *newLeafNode = new Node(nextLeafID, newInternalNode->stringDepth + newLeafEdge.length(), newInternalNode, newLeafEdge);
                            leaves++;
                            nextLeafID++;

                            // adding both new nodes to internals children and order children using lambda function to balance based off of the parentEdgeLabels
                            newInternalNode->children.push_back(currentChild);
                            newInternalNode->children.push_back(newLeafNode);
                            sort(newInternalNode->children.begin(), newInternalNode->children.end(), [](Node *node1, Node *node2) { 
                                return node1->parentEdgeLabel[0] < node2->parentEdgeLabel[0]; 
                                });

                            // comparing our deepestStringDepth with our newest created internal node
                            if (newInternalNode->stringDepth > deepestStringDepth) {
                                deepestStringDepth = newInternalNode->stringDepth;
                            }

                            totalInternalDepth += newInternalNode->stringDepth;
                            return newLeafNode;

                        }
                        else if (i < currentChild->parentEdgeLabel.length() - 1) {
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
                Node *newNode = new Node(nextLeafID, v->stringDepth + x.length(), v, x);
                v->children.push_back(newNode);
                leaves++;
                nextLeafID++;
                return newNode;
            }
        }
    }

    // tree construction functions
    void initializeTree() {
        this->root = new Node(nextInternalID, 0, nullptr, " ");
        this->root->suffixLink = root;
        internalNodes++;
        nextInternalID++;
    }

    void buildST() {
        Node *mostRecentLeaf = root;
        for (int i=0; i < SEQUENCE.length(); i++) {
            mostRecentLeaf = findPath(root, i);
        }

        // updating stats
        totalNodes = internalNodes + leaves;
        averageInternalDepth = totalInternalDepth / internalNodes;
    }

    // void buildST() {
    //     Node *mostRecentLeaf = root;
    //     for (int i=0; i < SEQUENCE.length(); i++) {
    //         Node *v = mostRecentLeaf->parent->suffixLink;
    //         if (v) { // suffix link exists for u
    //             if (v != root) { // Case 1A
    //                 mostRecentLeaf = findPath(v, v->stringDepth);
    //             }
    //             else { // Case 1B
    //                 mostRecentLeaf = findPath(root, i);
    //             }
    //             mostRecentLeaf = findPath(v, i);
    //         }
    //         else { // suffix link doesnt exist for u
    //             Node *uPrime = mostRecentLeaf->parent->parent;
    //             Node *vPrime = uPrime->suffixLink;
    //             mostRecentLeaf = findPath(vPrime, i);
    //         }
    //     }

    //     // updating stats
    //     totalNodes = internalNodes + leaves;
    //     averageInternalDepth = totalInternalDepth / internalNodes;
    // }

private:
    Node *root;
    // tree statistics
    int internalNodes, leaves, totalNodes, averageStringDepth, deepestStringDepth, totalInternalDepth, averageInternalDepth, nextInternalID, nextLeafID;
};

#endif