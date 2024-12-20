#include <cctype>
#include <iostream>
#include <string>
#include "lex.h"
#include <map>
#include <algorithm>

// Helper function to convert string to lowercase
std::string toLowerCase(const std::string &str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Map directory of keywords or reserved words
static std::map<std::string, Token> kwdir = {
    {"if", IF}, {"else", ELSE}, {"print", PRINT}, {"int", INT}, {"float", FLOAT}, {"char", CHAR}, {"string", STRING}, {"bool", BOOL}, {"program", PROGRAM}, {"true", TRUE}, {"false", FALSE}};

// Returns Token if it matches kwdir, Returns Token IDENT otherwise (T/F BCONST Exception)
LexItem id_or_kw(const std::string &lexeme, int linenum)
{
    std::string lowerLexeme = toLowerCase(lexeme); // Convert lexeme to lowercase

    // Check if lexeme is TRUE or FALSE (case insensitive)
    if (lowerLexeme == "true" || lowerLexeme == "false")
    {
        return LexItem(BCONST, lexeme, linenum); // Return BCONST for TRUE and FALSE
    }

    // Lookup in the keyword directory
    if (kwdir.find(lowerLexeme) != kwdir.end())
    {
        return LexItem(kwdir[lowerLexeme], lexeme, linenum); // Return corresponding keyword Token
    }

    return LexItem(IDENT, lexeme, linenum); // Return IDENT token if not a keyword
}

// Handles how LexItem is printed
std::ostream &operator<<(std::ostream &out, const LexItem &tok)
{
    Token token = tok.GetToken();
    std::string lexeme = tok.GetLexeme();
    int lineNum = tok.GetLinenum();

    std::string tokenString;
    switch (token)
    {
    case IF:
        tokenString = "IF";
        break;
    case ELSE:
        tokenString = "ELSE";
        break;
    case PRINT:
        tokenString = "PRINT";
        break;
    case INT:
        tokenString = "INT";
        break;
    case FLOAT:
        tokenString = "FLOAT";
        break;
    case CHAR:
        tokenString = "CHAR";
        break;
    case STRING:
        tokenString = "STRING";
        break;
    case BOOL:
        tokenString = "BOOL";
        break;
    case PROGRAM:
        tokenString = "PROGRAM";
        break;
    case TRUE:
        tokenString = "TRUE";
        break;
    case FALSE:
        tokenString = "FALSE";
        break;
    case IDENT:
        tokenString = "IDENT";
        break;
    case ICONST:
        tokenString = "ICONST";
        break;
    case RCONST:
        tokenString = "RCONST";
        break;
    case SCONST:
        tokenString = "SCONST";
        break;
    case BCONST:
        tokenString = "BCONST";
        break;
    case CCONST:
        tokenString = "CCONST";
        break;
    case PLUS:
        tokenString = "PLUS";
        break;
    case MINUS:
        tokenString = "MINUS";
        break;
    case MULT:
        tokenString = "MULT";
        break;
    case DIV:
        tokenString = "DIV";
        break;
    case ASSOP:
        tokenString = "ASSOP";
        break;
    case EQ:
        tokenString = "EQ";
        break;
    case NEQ:
        tokenString = "NEQ";
        break;
    case ADDASSOP:
        tokenString = "ADDASSOP";
        break;
    case SUBASSOP:
        tokenString = "SUBASSOP";
        break;
    case MULASSOP:
        tokenString = "MULASSOP";
        break;
    case DIVASSOP:
        tokenString = "DIVASSOP";
        break;
    case REMASSOP:
        tokenString = "REMASSOP";
        break;
    case GTHAN:
        tokenString = "GTHAN";
        break;
    case LTHAN:
        tokenString = "LTHAN";
        break;
    case AND:
        tokenString = "AND";
        break;
    case OR:
        tokenString = "OR";
        break;
    case NOT:
        tokenString = "NOT";
        break;
    case REM:
        tokenString = "REM";
        break;
    case COMMA:
        tokenString = "COMMA";
        break;
    case SEMICOL:
        tokenString = "SEMICOL";
        break;
    case LPAREN:
        tokenString = "LPAREN";
        break;
    case RPAREN:
        tokenString = "RPAREN";
        break;
    case DOT:
        tokenString = "DOT";
        break;
    case LBRACE:
        tokenString = "LBRACE";
        break;
    case RBRACE:
        tokenString = "RBRACE";
        break;
    case ERR:
        tokenString = "ERR";
        break;
    case DONE:
        tokenString = "DONE";
        break;
    default:
        tokenString = "UNKNOWN";
        break;
    }

    switch (token)
    {
    case ICONST:
    case RCONST:
    case BCONST:
        out << tokenString << ": (" << lexeme << ")";
        break;

    case IDENT:
        out << tokenString << ": <" << lexeme << ">";
        // std::cout << "TOKENCHECK" <<endl;
        break;

    case SCONST:
        out << tokenString << ": \"" << lexeme << "\"";
        break;

    case CCONST:
        out << tokenString << ": '" << lexeme << "'";
        break;

    case ERR:
        out << "ERR: In line " << lineNum << ", Error Message {" << lexeme << "}\n";
        break;
    default:
        out << tokenString;
        break;
    }

    out << std::endl;
    return out;
}

LexItem getNextToken(std::istream &in, int &linenum)
{
    enum TokState
    {
        START,
        INID,
        ININT,
        INREAL,
        INSTRING,
        INCHAR,
        INCOMMENT,
        INASSIGN,
        INARITH,
        INOP,
        INREL,
        INLOGIC,
        INMULTICOMMENT
    } lexstate = START;
    char ch;
    std::string lexeme;
    // Loop through the input stream
    while (in.get(ch))
    {
        //std::cout << "Current char: " << ch << " lexstate: " << lexstate << std::endl; // Debug print
        //std::cout << "Lexeme Build: " << lexeme << endl;
        //std::cout << "Linenum: " << linenum << endl;
        switch (lexstate)
        {
        case START:
            // Ignore whitespace
            if (std::isspace(ch))
            {
                linenum += (ch == '\n');
                continue;
            }

            // Initialize lexeme
            lexeme = ch;
            // IDENT
            if (std::isalpha(ch) || ch == '_')
            {
                lexstate = INID;
            }
            // INT
            else if (std::isdigit(ch))
            {
                lexstate = ININT;
            }
            // STRING
            else if (ch == '"')
            {
                lexeme = "";
                lexstate = INSTRING;
            }
            // CHARS
            else if (ch == '\'')
            {
                lexeme = "";
                lexstate = INCHAR;
            }
            // ASSIGNMENT
            else if (ch == '=')
            {
                lexstate = INASSIGN;
            }
            // COMMENT
            else if (ch == '/')
            {
                if (in.peek() == '/')
                {
                    lexstate = INCOMMENT; // Single line comment
                }
                else if (in.peek() == '*')
                {
                    lexstate = INMULTICOMMENT; // Multi-line comment
                    in.get(ch);                // Consume the '*'
                }
                else if (in.peek() == '=')
                {
                    // in.get(ch);
                    lexstate = INOP;
                }
                else
                {
                    // Process division operator
                    return LexItem(DIV, lexeme, linenum);
                }
            }
            // Positive INT
            else if (ch == '+' && (std::isdigit(in.peek())))
            {
                lexstate = ININT;
            }
            // Positive INT
            else if (ch == '-' && (std::isdigit(in.peek())))
            {
                lexstate = ININT;
            }
            // Positive Dec
            else if (ch == '+' && (std::isdigit(in.peek()) || in.peek() == '.'))
            {
                lexstate = INREAL;
            }
            // Negative Dec
            else if (ch == '-' && (std::isdigit(in.peek()) || in.peek() == '.'))
            {
                lexstate = INREAL;
            }
            // OPERATORS
            else if ((ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') && in.peek() == '=')
            {
                lexstate = INOP;
            }
            // ARITHMETIC
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
            {
                lexstate = INARITH;
            }
            // RELATIONAL
            else if (ch == '>' || ch == '<')
            {
                lexstate = INREL;
            }
            // LOGICAL
            else if (ch == '&' || ch == '|' || ch == '!')
            {
                lexstate = INLOGIC;
            }
            // DELIMITERS
            else if (ch == ',')
            {
                return LexItem(COMMA, lexeme, linenum); // Return comma directly
            }
            else if (ch == ';')
            {
                return LexItem(SEMICOL, lexeme, linenum); // Return semicolon directly
            }
            else if (ch == '(')
            {
                return LexItem(LPAREN, lexeme, linenum); // Return left parenthesis
            }
            else if (ch == ')')
            {
                return LexItem(RPAREN, lexeme, linenum); // Return right parenthesis
            }
            else if (ch == '{')
            {
                return LexItem(LBRACE, lexeme, linenum); // Return left brace
            }
            else if (ch == '}')
            {
                return LexItem(RBRACE, lexeme, linenum); // Return right brace
            }
            else if (ch == '.')
            {
                if (std::isdigit(in.peek()))
                {
                    lexstate = INREAL;
                }
                else
                {
                    return LexItem(DOT, lexeme, linenum); // Return dot (.) if not followed by a digit
                }
            }
            // ERR
            else
            {
                // std::cout << "DEBUG CHECK" << std::endl;
                return LexItem(ERR, lexeme, linenum);
            }
            break;

        case INID:
            if (std::isalnum(ch) || ch == '_')
            {
                lexeme += ch;
                // std::cout << "Building IDENT: " << lexeme << std::endl; // Debug print
                // std::cout << "Current char: " << ch << ", Line number: " << linenum << std::endl;
            }
            else
            {
                // std::cout << "Non-IDENT character encountered: " << ch << std::endl; // Debug print
                in.putback(ch);
                lexstate = START;
                return id_or_kw(lexeme, linenum);
            }
            break;

        case ININT:

            // Continue processing digits
            if (std::isdigit(ch))
            {
                lexeme += ch; // Add the digit to the lexeme
            }
            else if (ch == '.' && std::isdigit(in.peek()))
            {
                lexeme += ch;      // Add the decimal point
                lexstate = INREAL; // Switch to INREAL state if decimal is found
            }
            else
            {
                // lexeme += ch; // Put back the non-digit character
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenum);
            }
            break;

        case INREAL:
            if (std::isdigit(ch))
            {
                // If the character is a digit, add it to the lexeme
                lexeme += ch;
            }
            else if (ch == '.')
            {
                // Check if the lexeme already contains a decimal point
                if (lexeme.find('.') != std::string::npos)
                {
                    lexeme += ch;
                    // If a decimal point already exists, return an error
                    return LexItem(ERR, lexeme, linenum);
                }
                lexeme += ch;
            }
            else
            {
                // Put back the non-digit or non-decimal character for further processing
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenum);
            }
            break;

        case INSTRING:
            if (ch == '"')
            {
                //std::cout << "Building INSTRING: " << lexeme << std::endl; // Debug print
                return LexItem(SCONST, lexeme, linenum);
            }
            else if (isprint(ch))
            {
                //std::cout << "Non-INSTRING character encountered: " << ch << std::endl; // Debug print
                lexeme += ch;
            }
            else
            {
                std::string strerrorLexeme = " Invalid string constant \"" + lexeme;
                return LexItem(ERR, strerrorLexeme, linenum); // Return the error with a specific message
            }
            break;

        case INCHAR:
            if (ch == '\n')
            {
                //std::cout << "DEBUG CHECK" << std::endl;
                // If a newline is encountered, return an error for incomplete character constant
                std::string errorMessage = "New line is an invalid character constant.";
                return LexItem(ERR, errorMessage, linenum);
            }
            else if (ch == '\'')
            {
                //std::cout << "DEBUG CHECK1" << std::endl;
                // Check if more than one character has been read
                if (lexeme.length() > 1)
                {
                    //std::cout << "DEBUG CHECK2" << std::endl;
                    std::string errorMessage = " Invalid character constant '";
                    return LexItem(ERR, errorMessage + lexeme + "'", linenum);
                }
                //lexeme += ch;                              // Consume the closing quote
                return LexItem(CCONST, lexeme, linenum); // Return the character constant
            }
            else
            {
                if (lexeme.length() > 1)
                {
                    //std::cout << "DEBUG CHECK4" << std::endl;
                    std::string errorMessage = " Invalid character constant '";
                    return LexItem(ERR, errorMessage + lexeme + "'", linenum);
                }
                //std::cout << "DEBUG CHECK3" << std::endl;
                lexeme += ch; // Add the character to the lexeme
                // Continue to process other characters
            }
            break;

        case INCOMMENT:
            if (ch == '\n')
            {
                linenum++;
                lexstate = START;
            }
            break;

        case INMULTICOMMENT:
            if (ch == '*')
            {
                if (in.peek() == '/')
                {
                    in.get(ch); // Consume the '/'
                    // std::cout << "building" << endl;
                    lexstate = START; // End of multi-line comment
                }
            }
            else if (ch == '\n')
            {
                linenum++; // Increment line number for each newline within the comment
            }
            break;

        case INASSIGN:
            if (ch == '=' && lexeme.size() > 0 && lexeme[0] == '=')
            {
                lexeme += ch;
                return LexItem(EQ, lexeme, linenum);
            }
            else
            {
                in.putback(ch);
                return LexItem(ASSOP, lexeme, linenum);
            }
            break;

        case INARITH:
    // Handle single character operators: +, -, *, /, %
    if (lexeme[0] == '+') {
        if (!isdigit(ch) && ch != '.') {
            in.putback(ch); // Put the character back if it's not part of a number
            return LexItem(PLUS, lexeme, linenum); // + operator
        }
    }
    else if (lexeme[0] == '-') {
        if (!isdigit(ch) && ch != '.') {
            in.putback(ch); // Put the character back if it's not part of a number
            return LexItem(MINUS, lexeme, linenum); // - operator
        }
    }
    else if (lexeme[0] == '*') {
        in.putback(ch); // Always put back after an operator is processed
        return LexItem(MULT, lexeme, linenum); // * operator
    }
    else if (lexeme[0] == '/') {
        in.putback(ch); // Always put back after an operator is processed
        return LexItem(DIV, lexeme, linenum); // / operator
    }
    else if (lexeme[0] == '%') {
        in.putback(ch); // Always put back after an operator is processed
        return LexItem(REM, lexeme, linenum); // % operator
    }
    else {
        // If none of the cases match, put the character back into the stream and handle error
        in.putback(ch); // Put back the character that was read
        return LexItem(ERR, lexeme, linenum); // Return error for unrecognized case
    }
    break;

        case INOP:
            lexeme += ch; // Add '=' to the lexeme

            // Check the first character of the lexeme to determine the operator
            if (lexeme[0] == '+')
            {
                return LexItem(ADDASSOP, lexeme, linenum); // += assignment operator
            }
            else if (lexeme[0] == '-')
            {
                return LexItem(SUBASSOP, lexeme, linenum); // -= assignment operator
            }
            else if (lexeme[0] == '*')
            {
                return LexItem(MULASSOP, lexeme, linenum);
            }
            else if (lexeme[0] == '/')
            {
                return LexItem(DIVASSOP, lexeme, linenum); // /= assignment operator
            }
            else if (lexeme[0] == '%')
            {
                return LexItem(REMASSOP, lexeme, linenum); // %= assignment operator
            }
            else
            {
                return LexItem(ERR, lexeme, linenum); // Error if none of the operators match
            }
        case INREL:
            if (ch == '=')
            {
                return LexItem(lexeme == ">" ? GTHAN : LTHAN, lexeme, linenum);
            }
            in.putback(ch);
            return LexItem(lexeme == ">" ? GTHAN : LTHAN, lexeme, linenum);
            break;

        case INLOGIC:
            // Add the current character to the lexeme
            // Check for logical operators
            if (lexeme[0] == '!')
            {
                lexeme += ch;
                // Check if the next character is '=' for not-equality
                if (ch == '=')
                {
                    lexeme += in.get();                   // Add '=' to the lexeme
                    return LexItem(NEQ, lexeme, linenum); // Return NEQ
                }
                in.putback(ch);
                return LexItem(NOT, lexeme, linenum); // Return NOT if not followed by '='
            }
            else if (lexeme[0] == '&')
            {
                lexeme += ch;
                // Check if the next character is also '&' for logical AND
                if (ch == '&')
                {
                    lexeme += in.get();                   // Add the second '&' to the lexeme
                    return LexItem(AND, lexeme, linenum); // Return AND
                }
                return LexItem(ERR, lexeme, linenum); // Return error if not followed by '&'
            }
            else if (lexeme[0] == '|')
            {
                lexeme += ch;
                // Check if the next character is also '|' for logical OR
                if (ch == '|')
                {
                    lexeme += in.get();                  // Add the second '|' to the lexeme
                    return LexItem(OR, lexeme, linenum); // Return OR
                }
                return LexItem(ERR, lexeme, linenum); // Return error if not followed by '|'
            }
            else
            {
                // If lexeme doesn't match expected values, return error
                return LexItem(ERR, lexeme, linenum);
            }
        }
    }
    // std::cout << "LINENUM: " << linenum << endl;
    return LexItem(DONE, "", linenum);
}
