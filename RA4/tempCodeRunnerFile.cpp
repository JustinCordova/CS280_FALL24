#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <map>

// Invalid Literal
void printInvalidLiteral(const std::string& literal, const std::string& type, int line) {
    std::cout << "Incorrect " << type << " Literal " << literal << " at Line: " << line << std::endl;
}

// Potential Literal
bool isPotentialLiteral(const std::string& word) {
    return !word.empty() && (word[0] == '0' || isdigit(word[0]) || (word.size() > 1 && word[0] == '-' && isdigit(word[1])));
}

// Categorize Literals
void processWord(const std::string& word, std::map<std::string, int> maps[], 
                 int& decimalCount, int& hexCount, int& octalCount, int& binaryCount, int lineNumber) {
    std::regex decimalRegex("[1-9][0-9]*");
    std::regex octalRegex("0[0-7]+");
    std::regex hexRegex("0[xX][0-9a-fA-F]+");
    std::regex binaryRegex("0[bB][01]+");

    bool valid = false; // Flag check, if word is valid literal

    if (std::regex_match(word, decimalRegex)) {
        maps[0][word]++;
        decimalCount++;
        valid = true;
    } else if (std::regex_match(word, octalRegex)) {
        maps[1][word]++;
        octalCount++; 
        valid = true;
    } else if (std::regex_match(word, hexRegex)) {
        maps[2][word]++; 
        hexCount++; 
        valid = true;
    } else if (std::regex_match(word, binaryRegex)) {
        maps[3][word]++;
        binaryCount++;
        valid = true;
    }

    // Err if word is not valid and isPotentialLiteral
    if (!valid && isPotentialLiteral(word)) {
        if (word[0] == '0' && word.size() > 1 && word[1] >= '0' && word[1] <= '7') {
            printInvalidLiteral(word, "Octal", lineNumber);
        } else if (word.substr(0, 2) == "0x" || word.substr(0, 2) == "0X") {
            printInvalidLiteral(word, "Hexadecimal", lineNumber);
        } else if (word.substr(0, 2) == "0b" || word.substr(0, 2) == "0B") {
            printInvalidLiteral(word, "Binary", lineNumber);
        } else if (isdigit(word[0])) {
            printInvalidLiteral(word, "Decimal", lineNumber);
        }
    }
}

// Print occurrences of literals
void printLiteralOccurrences(const std::map<std::string, int>& literals, const std::string& title) {
    std::cout << "List of " << title << " and their number of occurrences:" << std::endl;
    
    for (std::map<std::string, int>::const_iterator it = literals.begin(); it != literals.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl; 
    }
    
    std::cout << std::endl; 
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "NO SPECIFIED INPUT FILE NAME." << std::endl;
        return 1; 
    }

    // Open specified input file
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cout << "CANNOT OPEN THE FILE " << argv[1] << std::endl;
        return 1; 
    }

    std::string line;
    int lineNumber = 0;
    int totalWords = 0;
    int decimalCount = 0;
    int hexCount = 0;
    int octalCount = 0;
    int binaryCount = 0;

    // Array Map for each literal type
    std::map<std::string, int> maps[4];

    

    // Handle flags for output
    if (argc == 3) {
        std::string flag = argv[2];
        if (flag == "-all") {
            // Summary output
            std::cout << "Total Number of Lines: " << lineNumber << std::endl;
            std::cout << "Number of Words: " << totalWords << std::endl;
            std::cout << "Number of Decimal Literals: " << decimalCount << std::endl;
            std::cout << "Number of Hexadecimal Literals: " << hexCount << std::endl;
            std::cout << "Number of Octal Literals: " << octalCount << std::endl;
            std::cout << "Number of Binary Literals: " << binaryCount << std::endl;
            std::cout << std::endl;

            printLiteralOccurrences(maps[3], "Binary-Literals");
            printLiteralOccurrences(maps[0], "Decimal-Literals");
            printLiteralOccurrences(maps[2], "Hex-Literals");
            printLiteralOccurrences(maps[1], "Octal-Literals");
        } else if (flag == "-bin") {
            printLiteralOccurrences(maps[3], "Binary-Literals");
        } else if (flag == "-dec") {
            printLiteralOccurrences(maps[0], "Decimal-Literals");
        } else if (flag == "-hex") {
            printLiteralOccurrences(maps[2], "Hex-Literals");
        } else if (flag == "-oct") {
            printLiteralOccurrences(maps[1], "Octal-Literals");
        } else {
            std::cout << "UNRECOGNIZED FLAG " << flag << std::endl; // Unrecognized flag error message
            return 1;
        }
    } else {
        while (std::getline(inputFile, line)) {
        lineNumber++; 
        std::istringstream stream(line); 
        std::string word; 

        while (stream >> word) {
            totalWords++; 
            processWord(word, maps, decimalCount, hexCount, octalCount, binaryCount, lineNumber);
        }
    }

    if (lineNumber == 0) {
        std::cout << "File is empty." << std::endl;
        return 1;
    }
        // Summary output if no specific flag is provided
        std::cout << "Total Number of Lines: " << lineNumber << std::endl;
        std::cout << "Number of Words: " << totalWords << std::endl;
        std::cout << "Number of Decimal Literals: " << decimalCount << std::endl;
        std::cout << "Number of Hexadecimal Literals: " << hexCount << std::endl;
        std::cout << "Number of Octal Literals: " << octalCount << std::endl;
        std::cout << "Number of Binary Literals: " << binaryCount << std::endl;
        std::cout << std::endl;

        // Print occurrences of all literals
        printLiteralOccurrences(maps[3], "Binary-Literals");
        printLiteralOccurrences(maps[0], "Decimal-Literals");
        printLiteralOccurrences(maps[2], "Hex-Literals");
        printLiteralOccurrences(maps[1], "Octal-Literals");
    }

    return 0; 
}
