#include "functions.cpp"

int main(int argc, char *argv[]) {
    cout << endl << "************ OUTPUT *************" << endl;
    cout << "---- DFS Node Information" << endl;

    // start the clock
    auto start = std::chrono::high_resolution_clock::now();

    // creating our suffixTree object
    ST suffixTree(argv[1], argv[2]);

    // stop the clock and calculate how long its been
    auto stop = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

    // calculating tree size
    auto nodeSize = sizeof(Node *);
    auto nodesSize = suffixTree.getTotalNodes() * nodeSize;
    auto size = sizeof(suffixTree) + nodesSize;

    cout << endl << endl << "---- String and Alphabet Information" << endl;
    cout << "Sequence File Name: " << argv[1] << endl;
    cout << "Alphabet File Name: " << argv[2] << endl << endl;
 
    cout << "---- Tree Statistics" << endl;
    cout << "Number of Internal Nodes: " << suffixTree.getInternalNodes() << endl;
    cout << "Number of Leaf Nodes: " << suffixTree.getLeafNodes() << endl;
    cout << "Total Number of Nodes: " << suffixTree.getTotalNodes() << endl;
    cout << "Average String Depth of Internal Nodes: " << suffixTree.getAverageInternalDepth() << endl;
    cout << "Deepest String Depth of Internal Nodes: " << suffixTree.getDeepestStringDepth() << endl;
    cout << endl << "Tree Size: " << size << " bytes." << endl;
    cout << "Tree Construction Time: " << time.count() << " microseconds." << endl;

    // cout << endl << "---- BWT Information" << endl;
    // cout << "The BWT index is: ";
    // for (int i=0; i<BWT.size(); i++) {
    //     cout << BWT[i] << " ";
    // }
    // cout << endl;

    // cout << endl << "---- Exact Matching Repeat Information" << endl;
    // cout << "STRING: " << LONGEST_MATCHING_REPEAT << endl;
    // cout << "Start Position: " << MR_START << endl;
    // cout << "Length: " << MR_LENGTH <<endl; 
}