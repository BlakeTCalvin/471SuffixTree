#include "functions.cpp"

int main(int argc, char *argv[]) {
    ST suffixTree(argv[1], argv[2]);

    cout << endl << endl << "************ OUTPUT *************" << endl << endl;

    cout << "---- String and Alphabet Information" << endl;
    cout << "Sequence File Name: " << argv[1] << endl;
    cout << "Alphabet File Name: " << argv[2] << endl << endl;
 
    cout << "---- Tree Statistics" << endl;
    cout << "Number of Internal Nodes: " << suffixTree.getInternalNodes() << endl;
    cout << "Number of Leaf Nodes: " << suffixTree.getLeafNodes() << endl;
    cout << "Total Number of Nodes: " << suffixTree.getTotalNodes() << endl;
    cout << "Average String Depth of Internal Nodes: " << suffixTree.getAverageInternalDepth() << endl;
    cout << "Deepest String Depth of Internal Nodes: " << suffixTree.getDeepestStringDepth() << endl;
}