#include <iostream>
#include <string>
#include <fstream>

// Remove Vowels from string
std::string RemoveVowels(const std::string &str)
{
    std::string result;
    result.reserve(str.length());
    for (char c : str)
    {
        char lowerC = tolower(c);
        // Skip vowels
        if (lowerC != 'a' && lowerC != 'e' && lowerC != 'i' && lowerC != 'o' && lowerC != 'u')
        {
            result += c; // Append non-vowel char
        }
    }

    return result;
}

int main(int argc, char *argv[])
{
    std::string instr;
    std::string result;

    if (argc < 2)
    {
        std::cout << "NO SPECIFIED INPUT FILE NAME." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cout << "CANNOT OPEN THE FILE " << argv[1] << std::endl;
        return 1;
    }

    std::getline(inputFile, instr);
    inputFile.close();

    result = RemoveVowels(instr);

    std::cout << "The input string \"" << instr << "\" with all vowels are removed: \"" << result << "\"" << std::endl;

    return 0;
}