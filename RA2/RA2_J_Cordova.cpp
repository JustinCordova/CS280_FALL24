#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
//boolean helpers
bool isCommand(const string& line) {
    return !line.empty() && line[0] == '%';
}

bool isComment(const string& line) {
    return !line.empty() && line[0] == '#';
}

int main() {
    string filename;
    cout << "Enter the name of a file to read from: ";
    cin >> filename;
    std::cout << "\n";
    std::cout << "\n";

    //Cannot be opened error
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "File cannot be opened " << filename << endl;
        return 1;
    }

    //Count Variables
    string line;
    int lineCount = 0;
    int commentedLines = 0;
    int paragraphCount = 0;
    int wordCount = 0;
    int boldCount = 0;
    int italicCount = 0;
    int underlineCount = 0;

    bool insideParagraph = false;
    int lastParagraph = 0;
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        lineNumber++;
        lineCount++;

        if (isComment(line)) {
            commentedLines++;
            continue;
        }

        if (isCommand(line)) {
            istringstream iss(line);
            string command;
            iss >> command;
            
            //%begin and %end errors
            if (command == "%begin") {
                if (insideParagraph) {
                    cout << "Missing end command for defining a paragraph after paragraph " << lastParagraph << endl << endl;
                }
                insideParagraph = true;
                lastParagraph = ++paragraphCount;
            } else if (command == "%end") {
                if (!insideParagraph) {
                    cout << "Missing begin command for defining a paragraph after paragraph " << paragraphCount << endl << endl;
                }
                insideParagraph = false;

            //bold, italic, and underline counts
            } else if (command == "%bold") {
                boldCount++;
            } else if (command == "%italic") {
                italicCount++;
            } else if (command == "%underline") {
                underlineCount++;
            } else {
                cout << "Incorrect command name at line: " << lineNumber << endl << endl;
            }
            string word;
            while (iss >> word) {
                wordCount++;
            }
            continue;
        }

        istringstream iss(line);
        string word;
        while (iss >> word) {
            wordCount++;
        }
    }

    if (insideParagraph) {
        
        cout << "Missing end command for defining a paragraph after paragraph " << lastParagraph-1 << endl << endl;
        paragraphCount-=1;
    }

    cout << "Total lines: " << lineCount << endl;
    cout << "Commented lines: " << commentedLines << endl;
    cout << "Number of Paragraphs: " << paragraphCount << endl;
    cout << "Total Number of Words: " << wordCount << endl;
    cout << "Bold commands: " << boldCount << endl;
    cout << "Italic commands: " << italicCount << endl;
    cout << "Underline commands: " << underlineCount << endl;
    return 0;
}
