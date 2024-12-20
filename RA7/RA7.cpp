#include <iostream>
#include <fstream>
#include <stack>
#include <string>

using namespace std;

int BalancedBrackets(const std::string &inputString)
{
    // Using stack to handle matching between opening and closing brackets
    std::stack<char> bracketStack;
    bool hasBrackets = false;

    for (char ch : inputString)
    {
        // Push opening brackets onto stack
        if (ch == '{' || ch == '(' || ch == '[')
        {
            bracketStack.push(ch);
            hasBrackets = true;
        }
        else if (ch == '}' || ch == ')' || ch == ']')
        {
            hasBrackets = true;

            // Case 4: If stack is empty then there are more closing brackets than opening
            if (bracketStack.empty())
            {
                std::cout << "case 4: There are more closing brackets than opening ones for one or more bracket types in the input string." << std::endl;
                return -1;
            }

            // Case 3: Mismatch if top of stack does not match current closing bracket
            char top = bracketStack.top();
            if ((ch == '}' && top == '{') || (ch == ')' && top == '(') || (ch == ']' && top == '['))
            {
                bracketStack.pop();
            }
            else
            {
                std::cout << "case 3: Mismatching bracket type between an opening and a closing bracket." << std::endl;
                return -1;
            }
        }
    }

    // Case 2: If stack is not empty then there are mistmatched bracket symbols
    if (!bracketStack.empty())
    {
        std::cout << "case 2: There are more opening brackets than closing ones for one or more bracket types in the input string." << std::endl;
        return -1;
    }

    // Case 5: Strings with no brackets
    if (!hasBrackets)
    {
        std::cout << "case 5: Scanning the input string is completed successfully." << std::endl;
        return 0;
    }

    // Case 1: Balanced brackets
    std::cout << "case 1: Scanning the input string is completed successfully." << std::endl;
    return 1;
}

void PrintBracketResult(int balanced, const std::string &sentence)
{
    if (balanced == 1)
        std::cout << "The following sentence contains balanced brackets: " << sentence << std::endl;
    else if (balanced == -1)
        std::cout << "The following sentence does not contain balanced brackets: " << sentence << std::endl;
    else
        std::cout << "The following sentence does not contain any brackets: " << sentence << std::endl;
}

int main(int argc, char *argv[])
{
    string sentence;
    int balanced;

    ifstream file;
    if (argc >= 2)
    {
        file.open(argv[1]);
        if (!file)
        {
            /* error */
            cerr << "File cannot be opened: " << argv[1] << endl;
            exit(1);
        }
    }
    else
    {
        cerr << "No file is found" << endl;
        exit(1);
    }

    getline(file, sentence);
    if (sentence.length() == 0)
    {
        cout << "File is empty." << endl;
        return 0;
    }

    balanced = BalancedBrackets(sentence);
    cout << endl;

    if (balanced == 1)
        cout << "The following sentence contains balanced brackets: " << sentence << endl;
    else if (balanced == -1)
        cout << "The following sentence does not contain balanced brackets: " << sentence << endl;
    else
        cout << "The following sentence does not contain any brackets: " << sentence << endl;
}
