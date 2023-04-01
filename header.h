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
#include <unordered_map>
#include <chrono>
#include <boost/algorithm/string/erase.hpp>

using namespace std;

// BWT
vector<char> BWT;

// sequence/alphabet
string SEQUENCE_HEADER, SEQUENCE;
string ALPHABET;
int SEQUENCE_LENGTH;

/***** FUNCTION DECLARATIONS *****/
void setAlphabet(string file);
void setSequence(string file);

/****** NODE STRUCT ******/
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

// Deepest Internal Node
Node *DEEPEST_INTERNAL_NODE;
string LONGEST_MATCHING_REPEAT;

// Matching Repeat
int MR_LENGTH;
int MR_START;

/****** ST CLASS ******/
class ST {
public:
    // public function i | Constructor
    ST(string fasta, string alphabet) {
        setAlphabet(alphabet);
        setSequence(fasta);
        
        this->nextInternalID = SEQUENCE.length() + 1;
        this->nextLeafID = 1;
        this->deepestStringDepth = 0;
        this->totalInternalDepth = 0;

        int start = -1;

        // tree building
        initializeTree();
        buildST();

        // other functions
        enumerateNodes(root, start);
        generateBWT(root);
        
        char temp = BWT.back();
        BWT.insert(BWT.begin(), temp);
        BWT.pop_back();

        // ------------
        longestMatchingRepeat();
        MR_LENGTH = LONGEST_MATCHING_REPEAT.length();

    }

    ~ST() { }

    // ---- getters
    Node *getRoot() { return this->root; }

    int getLeaves() { return this->leaves; }

    int getNextInternalID() { return this->nextInternalID; }

    int getNextLeafID() { return this->nextLeafID; }

    int getInternalNodes() { return this->internalNodes; }

    int getLeafNodes() { return this->leaves; }

    int getTotalNodes() { return this->totalNodes; }

    int getDeepestStringDepth() { return this->deepestStringDepth; }

    int getAverageInternalDepth() { return this->averageInternalDepth; }

    // ---- other functions
    // public function ii | Display all children of a node 
    void displayChildren(Node *node) { 
        cout << "Node: " << node->id << "'s children from left to right are: " << endl;
        for (Node *child : node->children) {
            cout << "Node: " << child->id << " | ";
        }
    }

    // public function iii | Enumerate our nodes in DFS order 
    void enumerateNodes(Node *node, int &counter) { // Depth First Search Enumeration
        counter++; // increase counter for each node
        if (counter % 7 == 0) {
            cout << endl;
        }
        cout << "Node ID: " << node->id << " has depth " << node->stringDepth << " | ";

        for (Node *child : node->children) {
            enumerateNodes(child, counter);
        }
    }

    // public function iv | get our BWT 
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

    // function to get longest matching repeat in our tree and save the start and length to globals
    void longestMatchingRepeat() {
        Node *currentNode = DEEPEST_INTERNAL_NODE;
        string temp;
        MR_START = DEEPEST_INTERNAL_NODE->id;
        while (currentNode != root) {
            temp = currentNode->parentEdgeLabel;
            reverse(temp.begin(), temp.end());
            LONGEST_MATCHING_REPEAT += temp;
            currentNode = currentNode->parent;
        }
    }

    // find path function
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
                                DEEPEST_INTERNAL_NODE = newInternalNode;
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

    // ---- tree construction functions
    void initializeTree() {
        this->root = new Node(nextInternalID, 0, root, " ");
        this->root->suffixLink = root;
        internalNodes++;
        nextInternalID++;
    }

    Node *NodeHops(Node *node, int index, int startBeta, int endBeta) {
        int betaIndex = startBeta;
        Node *currentNode = node;
        Node *v = nullptr;
        while (v == nullptr) {
            for (Node *child : currentNode->children) {
                if (child->parentEdgeLabel[0] == SEQUENCE[betaIndex]) {
                    if (endBeta < betaIndex + child->parentEdgeLabel.length()) { // quit early create a new internal node
                        // break edge
                        string edgePart1 = child->parentEdgeLabel.substr(0, endBeta-currentNode->stringDepth);
                        string edgePart2 = child->parentEdgeLabel.substr(endBeta-currentNode->stringDepth);

                        // remove old child from currentNode
                        for (auto it = currentNode->children.begin(); it != currentNode->children.end(); ++it) {
                            if (*it == child) { // correct node is found in children vector
                                currentNode->children.erase(it);
                                break;
                            }
                        }

                        // new internal node
                        Node *newInternalNode = new Node(nextInternalID, currentNode->stringDepth + edgePart1.length(), currentNode, edgePart1);
                        internalNodes++;
                        nextInternalID++;

                        // make internal node new child and order the children using lambda function to balance based off of the parentEdgeLabels 
                        currentNode->children.push_back(newInternalNode);
                        sort(currentNode->children.begin(), currentNode->children.end(), [](Node *node1, Node *node2) { 
                            return node1->parentEdgeLabel[0] < node2->parentEdgeLabel[0]; 
                            });

                        // update child node 
                        child->parent = newInternalNode;
                        child->stringDepth = newInternalNode->stringDepth + edgePart2.length();
                        child->parentEdgeLabel = edgePart2;

                        // adding former child as new Internal node child
                        newInternalNode->children.push_back(child);

                        // comparing our deepestStringDepth with our newest created internal node
                        if (newInternalNode->stringDepth > deepestStringDepth) {
                            deepestStringDepth = newInternalNode->stringDepth;
                            DEEPEST_INTERNAL_NODE = newInternalNode;
                        }
                        
                        // setting v equal to internal node
                        v = newInternalNode;

                    }
                    else if (endBeta == betaIndex + child->parentEdgeLabel.length()) { // landed on a node
                        v = child;
                    }
                    else { // not finished with beta string
                        betaIndex = betaIndex + child->parentEdgeLabel.length();
                    }
                }
            }
        }

        return v;
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

    //almost working build tree function with suffix links
    // void buildST() { // Using suffix links
    //     // trivial case of the first insertion
    //     Node *newNode = new Node(nextLeafID, SEQUENCE.length(), root, SEQUENCE);
    //     root->children.push_back(newNode);
    //     leaves++;
    //     nextLeafID++;
    
    //     Node *mostRecentLeaf = newNode;
    //     for (int i=1; i < SEQUENCE.length(); i++) {
    //         if (mostRecentLeaf->parent->suffixLink) { // suffix link exists for u
    //             Node *v = mostRecentLeaf->parent->suffixLink;
    //             if (v != root) { // Case 1A
    //                 mostRecentLeaf = findPath(v, i+v->stringDepth);
    //             }
    //             else { // Case 1B
    //                 mostRecentLeaf = findPath(root, i);
    //             }
    //         }
    //         else { // suffix link doesnt exist for u
    //             Node *u = mostRecentLeaf->parent;
    //             Node *uPrime = u->parent;
    //             Node *vPrime = uPrime->suffixLink;
    //             if (uPrime != root) { // Case 2A
    //                 Node *v = NodeHops(vPrime, i+vPrime->stringDepth, uPrime->stringDepth, u->stringDepth);
    //                 u->suffixLink = v;
    //                 mostRecentLeaf = findPath(v, uPrime->stringDepth);
    //             }
    //             else { // Case 2B
    //                 Node *v = NodeHops(root, i, 1, u->stringDepth);
    //                 u->suffixLink = v;
    //                 mostRecentLeaf = findPath(v, i+v->stringDepth);
    //             }
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