#include "functions.cpp"

int main(int argc, char *argv[]) {
    setAlphabet(argv[2]);
    setSequence(argv[1]);
    cout << "Head = " << SEQUENCE_HEADER << endl;
    cout << "Sequence = " << SEQUENCE << endl;
    cout << "Alphabet = " << ALPHABET << endl;
}