#include <iostream>
#include <string>
#include "lex.h"
#include <map>

// Map directory of keywords or reserved words
static std::map<std::string, Token> kwdir = {
    {"if", IF}, {"else", ELSE}, {"print", PRINT}, {"int", INT},
    {"float", FLOAT}, {"char", CHAR}, {"string", STRING}, {"bool", BOOL},
    {"program", PROGRAM}, {"true", TRUE}, {"false", FALSE}
};

// Returns Token if it matches kwdir, Returns Token IDENT otherwise (T/F BCONST Exception)
LexItem id_or_kw(const std::string& lexeme, int linenum) {
    if (kwdir.find(lexeme) != kwdir.end()) {
        if (lexeme == "true" || lexeme == "false") {
            return LexItem(BCONST, lexeme, linenum); 
        }
        return LexItem(kwdir[lexeme], lexeme, linenum);  
    }
    return LexItem(IDENT, lexeme, linenum); 
}

// Handles how LexItem is printed
std::ostream& operator<<(std::ostream& out, const LexItem& tok) {
    Token token = tok.GetToken();
    std::string lexeme = tok.GetLexeme();
    int lineNum = tok.GetLinenum();

    std::string tokenString;
    switch (token) {
        case IF: tokenString = "IF"; break;
        case ELSE: tokenString = "ELSE"; break;
        case PRINT: tokenString = "PRINT"; break;
        case INT: tokenString = "INT"; break;
        case FLOAT: tokenString = "FLOAT"; break;
        case CHAR: tokenString = "CHAR"; break;
        case STRING: tokenString = "STRING"; break;
        case BOOL: tokenString = "BOOL"; break;
        case PROGRAM: tokenString = "PROGRAM"; break;
        case TRUE: tokenString = "TRUE"; break;
        case FALSE: tokenString = "FALSE"; break;
        case IDENT: tokenString = "IDENT"; break;
        case ICONST: tokenString = "ICONST"; break;
        case RCONST: tokenString = "RCONST"; break;
        case SCONST: tokenString = "SCONST"; break;
        case BCONST: tokenString = "BCONST"; break;
        case CCONST: tokenString = "CCONST"; break;
        case PLUS: tokenString = "PLUS"; break;
        case MINUS: tokenString = "MINUS"; break;
        case MULT: tokenString = "MULT"; break;
        case DIV: tokenString = "DIV"; break;
        case ASSOP: tokenString = "ASSOP"; break;
        case EQ: tokenString = "EQ"; break;
        case NEQ: tokenString = "NEQ"; break;
        case ADDASSOP: tokenString = "ADDASSOP"; break;
        case SUBASSOP: tokenString = "SUBASSOP"; break;
        case MULASSOP: tokenString = "MULASSOP"; break;
        case DIVASSOP: tokenString = "DIVASSOP"; break;
        case REMASSOP: tokenString = "REMASSOP"; break;
        case GTHAN: tokenString = "GTHAN"; break;
        case LTHAN: tokenString = "LTHAN"; break;
        case AND: tokenString = "AND"; break;
        case OR: tokenString = "OR"; break;
        case NOT: tokenString = "NOT"; break;
        case REM: tokenString = "REM"; break;
        case COMMA: tokenString = "COMMA"; break;
        case SEMICOL: tokenString = "SEMICOL"; break;
        case LPAREN: tokenString = "LPAREN"; break;
        case RPAREN: tokenString = "RPAREN"; break;
        case DOT: tokenString = "DOT"; break;
        case LBRACE: tokenString = "LBRACE"; break;
        case RBRACE: tokenString = "RBRACE"; break;
        case ERR: tokenString = "ERR"; break;
        case DONE: tokenString = "DONE"; break;
        default: tokenString = "UNKNOWN"; break;
    }

    switch (token) {
        case ICONST:
        case RCONST:
            out << tokenString << ": (" << lexeme << ")";
            break;
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

        case IF:
            out << tokenString << ": KEYWORD";
            break;
        
        case ELSE:
            out << tokenString << ": KEYWORD";
            break;

        case PRINT:
            out << tokenString << ": KEYWORD";
            break;
        
        case INT:
            out << tokenString << ": KEYWORD";
            break;
        
        case FLOAT:
            out << tokenString << ": KEYWORD";
            break;
        
        case CHAR:
            out << tokenString << ": KEYWORD";
            break;

        case STRING:
            out << tokenString << ": KEYWORD";
            break;

        case BOOL:
            out << tokenString << ": KEYWORD";
            break;
        
        case PROGRAM:
            out << tokenString << ": KEYWORD";
            break;
        
        case COMMA:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case SEMICOL:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case LPAREN:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case RPAREN:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case LBRACE:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case RBRACE:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case DOT:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case PLUS:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case MINUS:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case MULT:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case DIV:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case ASSOP:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case EQ:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case NEQ:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case ADDASSOP:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case SUBASSOP:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case MULASSOP:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case DIVASSOP:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case REMASSOP:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case GTHAN:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case LTHAN:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case AND:
            out << tokenString << ": [" << lexeme << "]";
            break;
        
        case OR:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case NOT:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case REM:
            out << tokenString << ": [" << lexeme << "]";
            break;

        case ERR:
            out << tokenString << ": {" << lexeme << "} error at line no. " << lineNum;
            break;

        default:
            out << tokenString;
            break;
    }

    return out;
}