#include "functions.cpp"

int main(int argc, char *argv[]) {
    ST suffixTree(argv[1], argv[2]);

    cout << "Head = " << SEQUENCE_HEADER << endl;
    cout << "Sequence = " << SEQUENCE << endl;
    cout << "Alphabet = " << ALPHABET << endl;

}