#include "header.h"

// function for reading FASTA file and save the string as the ALPHABET global
void setAlphabet(string file) {
    string localFile = "alphabets/" + file;
    ifstream filename(localFile);

    string temp;
    size_t position;

    while (getline(filename, temp)) {
        ALPHABET += temp;
    }
    boost::algorithm::erase_all(ALPHABET, " ");
}

// function for reading FASTA file and save the string as the SEQUENCE_HEADER and SEQUENCE global
void setSequence(string file) {
    string localFile = "fastafiles/" + file;
    ifstream filename(localFile);

    string temp;
    size_t position;

    while (getline(filename, temp)) {
        if (temp[0] == '>') {
            position = temp.find(" ");
            SEQUENCE_HEADER = temp.substr(1, position-1);
        }
        else {
            for (char x : temp) {
                if (find(ALPHABET.begin(), ALPHABET.end(), x) == ALPHABET.end()) {
                    cerr << "Sequence in fasta file did not match the alphabet given." << endl;
                    exit(1);
                }
            }
            SEQUENCE += temp;
        }
    }
    filename.close();

    SEQUENCE += '$';
}