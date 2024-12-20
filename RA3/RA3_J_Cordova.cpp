#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

void printInvalidLiteral(const std::string& literal, const std::string& type, int line) {
    std::cout << "Incorrect " << type << " Literal " << literal << " at Line: " << line << std::endl;
}

bool isPotentialLiteral(const std::string& word) {
    return !word.empty() && (word[0] == '0' || isdigit(word[0]) || (word.size() > 1 && word[0] == '-' && isdigit(word[1])));
}

void processWord(const std::string& word, int& decimalCount, int& hexCount, int& octalCount, int& binaryCount, int lineNumber) {
    std::regex decimalRegex("[1-9][0-9]*");
    std::regex octalRegex("0[0-7]+");
    std::regex hexRegex("0[xX][0-9a-fA-F]+");
    std::regex binaryRegex("0[bB][01]+");

    bool valid = false;

    if (std::regex_match(word, decimalRegex)) {
        decimalCount++;
        valid = true;
    } else if (std::regex_match(word, octalRegex)) {
        octalCount++;
        valid = true;
    } else if (std::regex_match(word, hexRegex)) {
        hexCount++;
        valid = true;
    } else if (std::regex_match(word, binaryRegex)) {
        binaryCount++;
        valid = true;
    }

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "NO SPECIFIED INPUT FILE NAME." << std::endl;
        return 1;
    }

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

    while (std::getline(inputFile, line)) {
        lineNumber++;
        std::istringstream stream(line);
        std::string word;

        if (line.empty()) {
            continue;
        }

        while (stream >> word) {
            totalWords++;
            processWord(word, decimalCount, hexCount, octalCount, binaryCount, lineNumber);
        }
    }

    if (lineNumber == 0) {
        std::cout << "File is empty." << std::endl;
        return 1;
    }

    std::cout << "Total Number of Lines: " << lineNumber << std::endl;
    std::cout << "Number of Words: " << totalWords << std::endl;
    std::cout << "Number of Decimal Literals: " << decimalCount << std::endl;
    std::cout << "Number of Hexadecimal Literals: " << hexCount << std::endl;
    std::cout << "Number of Octal Literals: " << octalCount << std::endl;
    std::cout << "Number of Binary Literals: " << binaryCount << std::endl;

    return 0;
}
