#include <iostream>
#include <fstream>
#include <sstream>
#include "lex.h"
#include <string>
#include <map>
#include <stdio.h>
#include <ctype.h>

void addChar(char ch, string& lexeme) {
    lexeme += ch;  // Append character to the lexeme
}
void getChar(istream& in, char& ch) {
    if (in.get(ch)) {
        // Successfully read a character
        return;
    }
    ch = EOF; // Indicate end of input
}
void lookup(const string& lexeme, int linenum) {
    // Check if the lexeme corresponds to a keyword or identifier
    LexItem token = id_or_kw(lexeme, linenum);
    // Further processing can be added here
}


int main(int argc, char* argv[]) {
    // Read input file
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }
    


    // Tokenize input file
    string line;
    int linenum = 1; // Start line number at 1
    while (getline(inputFile, line)) {
        istringstream stream(line);
        char ch; // Character to read
        LexItem token; // LexItem to hold the token

        // Use a while loop to read tokens from the stream
        while (true) {
            token = getNextToken(stream, linenum); // Get the next token
            if (token.GetToken() == DONE) {
                break; // End of file
            } else if (token.GetToken() == ERR) {
                cerr << "Error: Invalid token on line " << linenum << ": " << token.GetLexeme() << endl;
            } else {
                // Process the token (for example, print it)
                cout << token << endl; // Assuming you have an overloaded operator<< for LexItem
            }
        }
    }

    return 0;
}