#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include "lex.h"
#include <algorithm>
#include <map>

extern std::string toLowerCase(const std::string &str);

bool comp_str(const std::string &str1, const std::string &str2)
{
    // Convert strings to double for numerical comparison
    double num1 = std::stod(str1);
    double num2 = std::stod(str2);

    // Comparison
    return num1 < num2;
}

// Keyword mapping for processKeywords --> kwSet
std::map<std::string, int> createKeywordMap() {
    return std::map<std::string, int>{
        {"if", 0},
        {"else", 1},
        {"print", 2},
        {"int", 3},
        {"float", 4},
        {"char", 5},
        {"string", 6},
        {"bool", 7},
        {"program", 8},
        {"true", 9},
        {"false", 10}
    };
}

// Process kwSet and print in numeric order
void processKeywords(const std::set<std::string> &kwSet)
{
    std::map<std::string, int> kwMake = createKeywordMap();
    std::vector<std::pair<int, std::string>> orderedKeywords;

    for (const auto &token : kwSet)
    {
        // Case sensitive matching
        std::string lowerToken = toLowerCase(token);
        // Check if the keyword exists in kwMake
        auto it = kwMake.find(lowerToken);
        if (it != kwMake.end())
        {
            orderedKeywords.emplace_back(it->second, lowerToken);
        }
    }

    // Sort based on numeric order
    std::sort(orderedKeywords.begin(), orderedKeywords.end());

    // Format Keywords flag output
    std::cout << "KEYWORDS:";
    std::cout << "\n";
    for (size_t i = 0; i < orderedKeywords.size(); ++i)
    {
        std::cout << orderedKeywords[i].second; // Print
        if (i < orderedKeywords.size() - 1)
        {
            std::cout << ", "; // Add comma if it's not the last element
        }
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    // Flag variables
    bool printAll = false, numFlag = false, strFlag = false, charFlag = false, idFlag = false, kwFlag = false, boolFlag = false;
    std::string filename;
    std::vector<std::string> validFlags = {"-all", "-num", "-str", "-char", "-id", "-kw", "-bool"};

    // Process command-line arguments --> uses bools to check flag args, otherwise give unrecognized flag error
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg[0] == '-')
        {
            if (arg == "-all")
            {
                printAll = true;
            }
            else if (arg == "-num")
            {
                numFlag = true;
            }
            else if (arg == "-str")
            {
                strFlag = true;
            }
            else if (arg == "-char")
            {
                charFlag = true;
            }
            else if (arg == "-id")
            {
                idFlag = true;
            }
            else if (arg == "-kw")
            {
                kwFlag = true;
            }
            else if (arg == "-bool")
            {
                boolFlag = true;
            }
            else
            {
                std::cerr << "Unrecognized flag {" << arg << "}" << std::endl;
                return 1;
            }
        }
        else
        {
            if (!filename.empty())
            {
                std::cerr << "Only one file name is allowed." << std::endl;
                return 1;
            }
            filename = arg;
        }
    }

    if (filename.empty())
    {
        std::cerr << "No specified input file." << std::endl;
        return 1;
    }

    // Open the file
    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        std::cerr << "CANNOT OPEN THE FILE " << filename << std::endl;
        return 1;
    }

    // Check if file is empty
    if (inputFile.peek() == std::ifstream::traits_type::eof())
    {
        std::cout << "Empty file." << std::endl;
        return 1;
    }


    std::set<std::string> numSet, strSet, charSet, idSet, kwSet, boolSet;

    int linenum = 1; // L: Line Count
    int totalTokens = 0;  // M: Total Tokens
    int idkwCount = 0;    // N: Identifiers and Keywords
    int irconstCount = 0; // O: Numerics (ICONST and RCONST)
    int bconstCount = 0;  // P: Booleans (BCONST)
    int scconstCount = 0; // Q: Strings and Characters (SCONST and CCONST)
    bool err = false;

    LexItem token;

    // Retrieve tokens until DONE is encountered
    while ((token = getNextToken(inputFile, linenum)) != DONE)
    {
        totalTokens++;
        // ERR token handling
        if (token.GetToken() == ERR)
        {
            std::cout << token;
            err = true;
            break;
        }

        // -all
        if (printAll)
        {
            std::cout << token;
        }

        Token tokType = token.GetToken();
        std::string lexeme = token.GetLexeme();
        size_t dotPos = lexeme.find('.');
        // Sort tokens into sets for flag printing purposes
        switch (tokType)
        {
        case ICONST:
            //Erase the +, only in flag print
            if (lexeme[0] == '+')
            {
                lexeme.erase(0, 1);
            }
            if (numSet.insert(lexeme).second)
            {
                irconstCount++;
            }
            // std::cout << "DEBUG: Added " << lexeme << " to NUMERIC CONSTANTS (irconstCount = " << irconstCount << ")" << std::endl; // Debug Print
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            break;

        case RCONST:
            //Erase the +, only in flag print
            if (lexeme[0] == '+')
            {
                lexeme.erase(0, 1);
            }
            // If decimal point is at beginning or directly after, append a 0
            if (dotPos != std::string::npos)
            {
                // Check if the decimal point is at the beginning (pos 0) or if it follows a sign (pos 1)
                if (dotPos == 0 || (dotPos == 1 && (lexeme[0] == '-' || lexeme[0] == '+')))
                {
                    lexeme.insert(dotPos, "0"); // Add '0' right before the decimal point
                }
            }
            if (numSet.insert(lexeme).second)
            {
                irconstCount++;
            }
            // std::cout << "DEBUG: Added " << lexeme << " to NUMERIC CONSTANTS (irconstCount = " << irconstCount << ")" << std::endl; // Debug Print
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            break;
        case SCONST:
            scconstCount++;
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            strSet.insert(lexeme);
            break;
        case CCONST:
            scconstCount++;
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            charSet.insert(lexeme);
            break;
        case IDENT:
            // Check for duplicates
            if (idSet.insert(lexeme).second) // Insert returns a pair, second is true if insertion was successful
            {
                idkwCount++;
                // std::cout << "DEBUG: Added " << lexeme << " to IDENTIFIERS (idkwCount = " << idkwCount << ")" << std::endl; // Debug Print
                // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            }
            break;
        case IF:
        case ELSE:
        case PRINT:
        case INT:
        case FLOAT:
        case CHAR:
        case STRING:
        case PROGRAM:
        case BOOL:
            if (kwSet.insert(lexeme).second)
            {
                idkwCount++;
                // std::cout << "DEBUG: Added " << lexeme << " to KEYWORDS (idkwCount = " << idkwCount << ")" << std::endl; // Debug Print
                // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            }
            break;
        case BCONST:
            bconstCount++;
            boolSet.insert(lexeme);
            // std::cout << "DEBUG: Added " << lexeme << " to BOOLEAN CONSTANTS (bconstCount = " << bconstCount << ")" << std::endl; // Debug Print
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            break;
        case PLUS:
        case MINUS:
        case MULT:
        case DIV:
        case ASSOP:
        case EQ:
        case NEQ:
        case ADDASSOP:
        case SUBASSOP:
        case MULASSOP:
        case DIVASSOP:
        case REMASSOP:
        case GTHAN:
        case LTHAN:
        case AND:
        case OR:
        case NOT:
        case REM:
            // std::cout << "DEBUG: Arithmetic, Logical, or Relational Operator Token: " << lexeme << std::endl; // Debug Print
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            break;
        case COMMA:
        case SEMICOL:
        case LPAREN:
        case RPAREN:
        case DOT:
        case LBRACE:
        case RBRACE:
            // std::cout << "DEBUG: Delimiter Token: " << lexeme << std::endl; // Debug Print
            // std::cout << "DEBUG: Added " << lexeme << " to TOTAL (total = " << totalTokens << ")" <<std::endl; // Debug Print
            break;
        case ERR:
            // std::cout << "DEBUG: Error Token: " << lexeme << std::endl; // Debug Print
            break;
        case DONE:
            // std::cout << "DEBUG: Reached end of file (DONE)" << std::endl; // Debug Print
            break;
        default:
            // std::cout << "DEBUG: Unrecognized token: " << lexeme << std::endl; // Debug Print
            break;
        }
    }
    if (!err)
    {
        // Print Summary
        std::cout << "\nLines: " << linenum - 1 << std::endl;                 // L: Total Lines
        std::cout << "Total Tokens: " << totalTokens << std::endl;            // M: Total Tokens
        std::cout << "Identifiers and Keywords: " << idkwCount << std::endl;  // N: Total IDENTS and Keywords
        std::cout << "Numerics: " << irconstCount << std::endl;               // O: Total ICONST and RCONSTS
        std::cout << "Booleans: " << bconstCount << std::endl;                // P: Total Booleans
        std::cout << "Strings and Characters: " << scconstCount << std::endl; // Q: Total SCONT and CCONST

        // Print Sets with corresponding flags

        // NumSet Sort
        std::vector<std::string> numV(numSet.begin(), numSet.end());
        std::sort(numV.begin(), numV.end(), comp_str);

        if (numFlag && !numV.empty())
        {
            std::cout << "NUMERIC CONSTANTS:" << std::endl;
            for (size_t i = 0; i < numV.size(); ++i)
            {
                std::cout << numV[i]; // Print the numeric

                if (i < numV.size() - 1) // Check if it's not the last element, print comma if so
                {
                    std::cout << ", "; 
                }
            }
            std::cout << std::endl;
        }
        if (boolFlag && !boolSet.empty())
        {
            std::cout << "BOOLEAN CONSTANTS:" << std::endl;
            auto it = boolSet.begin();
            for (size_t i = 0; i < boolSet.size(); ++i)
            {
                std::cout << *it; // Print the boolean
                ++it;
                if (i < boolSet.size() - 1) // Check if it's not the last element, print comma if so
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }

        if (charFlag && !charSet.empty())
        {
            std::cout << "CHARACTER CONSTANTS:" << std::endl;
            auto it = charSet.begin();
            for (size_t i = 0; i < charSet.size(); ++i)
            {
                std::cout << "'" << *it << "'"; // Print the CCONST with single quotes
                ++it;
                if (i < charSet.size() - 1) // Check if it's not the last element, print comma if so
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }

        if (strFlag && !strSet.empty())
        {
            std::cout << "STRINGS:" << std::endl;
            auto it = strSet.begin();
            for (size_t i = 0; i < strSet.size(); ++i)
            {
                std::cout << "\"" << *it << "\""; // Print the SCONST with double quotes
                ++it;
                if (i < strSet.size() - 1) // Check if it's not the last element, print comma if so
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }

        if (idFlag && !idSet.empty())
        {
            std::cout << "IDENTIFIERS:" << std::endl;
            auto it = idSet.begin();
            for (size_t i = 0; i < idSet.size(); ++i)
            {
                std::cout << *it; // Print the identifier
                ++it;
                if (i < idSet.size() - 1) // Check if it's not the last element, print comma if so
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
    
        if (kwFlag && !kwSet.empty())
        {
            processKeywords(kwSet); // Use function to print kwSet
        }
    }
    return 0;
}