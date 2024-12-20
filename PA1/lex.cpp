#include <cctype>
#include <iostream>
#include <string>
#include "lex.h"
#include <map>
#include <algorithm>

// Strings to lower case
std::string toLowerCase(const std::string &str)
{
    std::string lowStr = str;
    std::transform(lowStr.begin(), lowStr.end(), lowStr.begin(), ::tolower);
    return lowStr;
}

// Map directory of keywords or reserved words
static std::map<std::string, Token> kwdir = {
    {"if", IF}, {"else", ELSE}, {"print", PRINT}, {"int", INT}, {"float", FLOAT}, {"char", CHAR}, {"string", STRING}, {"bool", BOOL}, {"program", PROGRAM}, {"true", TRUE}, {"false", FALSE}};

// Returns Token if it matches kwdir, Returns Token IDENT otherwise (T/F BCONST Exception)
LexItem id_or_kw(const std::string &lexeme, int linenum)
{
    std::string lowLexeme = toLowerCase(lexeme); // Convert lexeme to lowercase

    // Check if lexeme is TRUE or FALSE (case insensitive)
    if (lowLexeme == "true" || lowLexeme == "false")
    {
        return LexItem(BCONST, lexeme, linenum); // Return BCONST for TRUE and FALSE
    }

    // Search in kwdir
    if (kwdir.find(lowLexeme) != kwdir.end())
    {
        return LexItem(kwdir[lowLexeme], lexeme, linenum); // Return corresponding keyword Token
    }

    return LexItem(IDENT, lexeme, linenum); // Return IDENT token if not a keyword
}

// Handles how LexItem is printed
std::ostream &operator<<(std::ostream &out, const LexItem &tok)
{
    static std::map<Token, std::string> tokenMap = {
        {IF, "IF"}, {ELSE, "ELSE"}, {PRINT, "PRINT"}, 
        {INT, "INT"}, {FLOAT, "FLOAT"}, {CHAR, "CHAR"}, 
        {STRING, "STRING"}, {BOOL, "BOOL"}, {PROGRAM, "PROGRAM"}, 
        {TRUE, "TRUE"}, {FALSE, "FALSE"}, {IDENT, "IDENT"}, 
        {ICONST, "ICONST"}, {RCONST, "RCONST"}, {SCONST, "SCONST"}, 
        {BCONST, "BCONST"}, {CCONST, "CCONST"}, {PLUS, "PLUS"}, 
        {MINUS, "MINUS"}, {MULT, "MULT"}, {DIV, "DIV"}, 
        {ASSOP, "ASSOP"}, {EQ, "EQ"}, {NEQ, "NEQ"}, 
        {ADDASSOP, "ADDASSOP"}, {SUBASSOP, "SUBASSOP"}, 
        {MULASSOP, "MULASSOP"}, {DIVASSOP, "DIVASSOP"}, 
        {REMASSOP, "REMASSOP"}, {GTHAN, "GTHAN"}, 
        {LTHAN, "LTHAN"}, {AND, "AND"}, {OR, "OR"}, {NOT, "NOT"}, 
        {REM, "REM"}, {COMMA, "COMMA"}, {SEMICOL, "SEMICOL"}, 
        {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {DOT, "DOT"}, 
        {LBRACE, "LBRACE"}, {RBRACE, "RBRACE"}, {ERR, "ERR"}, 
        {DONE, "DONE"}};

    // Initialize token object LexItem
    // Get lexeme and linenum from LexItem
    Token token = tok.GetToken();
    std::string lexeme = tok.GetLexeme();
    int lineNum = tok.GetLinenum();

    // Look up the string for the current token
    std::string tokenString = tokenMap[token];

    switch (token)
    {
    case ICONST:
    case RCONST:
    case BCONST:
        out << tokenString << ": (" << lexeme << ")";
        break;

    case IDENT:
        out << tokenString << ": <" << lexeme << ">";
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

// Process characters from input stream and return LexItem with corresponding token
LexItem getNextToken(std::istream &in, int &linenum)
{
    // Switch States
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
        INASSOP,
        INREL,
        INLOGIC,
        INMULTICOMMENT
    } lexstate = START;

    char ch;
    std::string lexeme;

    // Loop through the input stream
    while (in.get(ch))
    {
        // std::cout << "Current char: " << ch << " lexstate: " << lexstate << std::endl; // Debug print
        // std::cout << "Lexeme Build: " << lexeme << endl; // Debug Print
        // std::cout << "Linenum: " << linenum << endl; // Debug Print
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
                    in.get(ch);
                }
                else if (in.peek() == '=')
                {
                    lexstate = INASSOP;
                }
                else
                {
                    return LexItem(DIV, lexeme, linenum);
                }
            }
            // POSITIVE INTEGER
            else if (ch == '+' && (std::isdigit(in.peek())))
            {
                lexstate = ININT;
            }
            // POSITIVE INTEGER
            else if (ch == '-' && (std::isdigit(in.peek())))
            {
                lexstate = ININT;
            }
            // POSITIVE DECIMAL
            else if (ch == '+' && (std::isdigit(in.peek()) || in.peek() == '.'))
            {
                lexstate = INREAL;
            }
            // NEGATIVE DECIMAL
            else if (ch == '-' && (std::isdigit(in.peek()) || in.peek() == '.'))
            {
                lexstate = INREAL;
            }
            // ASSIGNMENT OPERATORS
            else if ((ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') && in.peek() == '=')
            {
                lexstate = INASSOP;
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
                return LexItem(COMMA, lexeme, linenum);
            }
            else if (ch == ';')
            {
                return LexItem(SEMICOL, lexeme, linenum);
            }
            else if (ch == '(')
            {
                return LexItem(LPAREN, lexeme, linenum);
            }
            else if (ch == ')')
            {
                return LexItem(RPAREN, lexeme, linenum);
            }
            else if (ch == '{')
            {
                return LexItem(LBRACE, lexeme, linenum);
            }
            else if (ch == '}')
            {
                return LexItem(RBRACE, lexeme, linenum);
            }
            else if (ch == '.')
            {
                if (std::isdigit(in.peek()))
                {
                    lexstate = INREAL;
                }
                else
                {
                    return LexItem(DOT, lexeme, linenum);
                }
            }
            // ERR
            else
            {
                // std::cout << "DEBUG CHECK" << std::endl; // Debug Print
                return LexItem(ERR, lexeme, linenum);
            }
            break;

        case INID:
            if (std::isalnum(ch) || ch == '_')
            {
                lexeme += ch;
                // std::cout << "Building IDENT: " << lexeme << std::endl; // Debug print
            }
            else
            {
                // std::cout << "Non-IDENT: " << ch << std::endl; // Debug print
                in.putback(ch);
                lexstate = START;
                return id_or_kw(lexeme, linenum);
            }
            break;

        case ININT:
            if (std::isdigit(ch))
            {
                lexeme += ch;
            }
            // Switch to INREAL if decimal is found
            else if (ch == '.' && std::isdigit(in.peek()))
            {
                lexeme += ch;
                lexstate = INREAL;
            }
            else
            {
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenum);
            }
            break;

        case INREAL:
            if (std::isdigit(ch))
            {
                lexeme += ch;
            }
            else if (ch == '.')
            {
                // If lexeme contains second decmial, return ERR
                if (lexeme.find('.') != std::string::npos)
                {
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenum);
                }
                lexeme += ch;
            }
            else
            {
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenum);
            }
            break;

        case INSTRING:
            if (ch == '"')
            {
                // std::cout << "Building INSTRING: " << lexeme << std::endl; // Debug print
                return LexItem(SCONST, lexeme, linenum);
            }
            else if (isprint(ch))
            {
                // std::cout << "Non-INSTRING character encountered: " << ch << std::endl; // Debug print
                lexeme += ch;
            }
            else
            {
                // Return string specific error
                std::string strerrLexeme = " Invalid string constant \"" + lexeme;
                return LexItem(ERR, strerrLexeme, linenum);
            }
            break;

        case INCHAR:
            if (ch == '\n')
            {
                // std::cout << "DEBUG CHECK" << std::endl; // Debug Print
                //  Newline returns invalid ch constant error, character not completed
                std::string errorMessage = "New line is an invalid character constant.";
                return LexItem(ERR, errorMessage, linenum);
            }
            else if (ch == '\'')
            {
                // std::cout << "DEBUG CHECK2" << std::endl; // Debug Print
                //  Check if more than 1 character is read
                if (lexeme.length() > 1)
                {
                    // std::cout << "DEBUG CHECK3" << std::endl; // Debug Print
                    //  Lexeme > 1, return invalid character constant error
                    std::string errorMessage = " Invalid character constant '";
                    return LexItem(ERR, errorMessage + lexeme + "'", linenum);
                }
                return LexItem(CCONST, lexeme, linenum);
            }
            else
            {
                // Check if more than 1 character is read for second safety
                if (lexeme.length() > 1)
                {
                    // std::cout << "DEBUG CHECK4" << std::endl; // Debug Print
                    std::string errorMessage = " Invalid character constant '";
                    return LexItem(ERR, errorMessage + lexeme + "'", linenum);
                }
                // std::cout << "DEBUG CHECK5" << std::endl;
                lexeme += ch;
            }
            break;

        case INCOMMENT:
            // Skips reading charactesr past `//` and increments linenum
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
                    in.get(ch);
                    // std::cout << "Multicomm" << endl; // Debug Print
                    lexstate = START;
                }
            }
            // Increments linenum for each newline within multicomment
            else if (ch == '\n')
            {
                linenum++;
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
            // I messed this up, including single operators in INASSIGN, so this is done a different way compared to other lexstates
            // For some reason it works
            // Reminder: *fix in future if possible* --> dont use lexeme[0]
            // Handle single operators: +, -, *, /, %
            if (lexeme[0] == '+')
            {
                // If current ch is not part of number, putback to be processed
                if (!isdigit(ch) && ch != '.')
                {
                    in.putback(ch);
                    return LexItem(PLUS, lexeme, linenum);
                }
            }
            else if (lexeme[0] == '-')
            {
                if (!isdigit(ch) && ch != '.')
                {
                    in.putback(ch);
                    return LexItem(MINUS, lexeme, linenum);
                }
            }
            else if (lexeme[0] == '*')
            {
                in.putback(ch);
                return LexItem(MULT, lexeme, linenum);
            }
            else if (lexeme[0] == '/')
            {
                in.putback(ch);
                return LexItem(DIV, lexeme, linenum);
            }
            else if (lexeme[0] == '%')
            {
                in.putback(ch);
                return LexItem(REM, lexeme, linenum);
            }
            else
            {
                in.putback(ch);
                return LexItem(ERR, lexeme, linenum);
            }
            break;

        case INASSOP:
            // Reminder: *fix in future if possible* --> dont use lexeme[0]
            lexeme += ch;

            if (lexeme[0] == '+')
            {
                return LexItem(ADDASSOP, lexeme, linenum);
            }
            else if (lexeme[0] == '-')
            {
                return LexItem(SUBASSOP, lexeme, linenum);
            }
            else if (lexeme[0] == '*')
            {
                return LexItem(MULASSOP, lexeme, linenum);
            }
            else if (lexeme[0] == '/')
            {
                return LexItem(DIVASSOP, lexeme, linenum);
            }
            else if (lexeme[0] == '%')
            {
                return LexItem(REMASSOP, lexeme, linenum);
            }
            else
            {
                return LexItem(ERR, lexeme, linenum);
            }
            break;

        case INREL:
            // This was not touched since day 1, strucutre is not good, but it works
            // Reminder: *fix in future!!*
            if (ch == '=')
            {
                return LexItem(lexeme == ">" ? GTHAN : LTHAN, lexeme, linenum);
            }
            in.putback(ch);
            return LexItem(lexeme == ">" ? GTHAN : LTHAN, lexeme, linenum);
            break;

        case INLOGIC:
            // Similar to INASSIGN and INARITH, done poorly
            // Reminder: *fix in future if possible* --> dont use lexeme[0]
            if (lexeme[0] == '!')
            {
                lexeme += ch;
                if (ch == '=')
                {
                    lexeme += in.get();
                    return LexItem(NEQ, lexeme, linenum);
                }
                in.putback(ch);
                return LexItem(NOT, lexeme, linenum);
            }
            else if (lexeme[0] == '&')
            {
                lexeme += ch;
                if (ch == '&')
                {
                    lexeme += in.get();
                    return LexItem(AND, lexeme, linenum);
                }
                return LexItem(ERR, lexeme, linenum);
            }
            else if (lexeme[0] == '|')
            {
                lexeme += ch;
                if (ch == '|')
                {
                    lexeme += in.get();
                    return LexItem(OR, lexeme, linenum);
                }
                return LexItem(ERR, lexeme, linenum);
            }
            else
            {
                return LexItem(ERR, lexeme, linenum);
            }
        }
    }
    return LexItem(DONE, "", linenum);
}