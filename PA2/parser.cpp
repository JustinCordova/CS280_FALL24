/* Implementation of Recursive-Descent Parser
 * for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
 */

#include "parser.h"

map<string, bool> defVar;
map<string, Token> SymTable;

std::string latestVar;
int nestLvl = 1;

namespace Parser
{
	bool pushed_back = false;
	LexItem pushed_token;

	static LexItem GetNextToken(istream &in, int &line)
	{
		if (pushed_back)
		{
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem &t)
	{
		if (pushed_back)
		{
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}
}

static int error_count = 0;

int ErrCount()
{
	return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream &in, int &line);

// PrintStmt:= PRINT (ExprList)
bool PrintStmt(istream &in, int &line)
{
	LexItem t;
	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex)
	{
		ParseError(line, "Missing expression list after Print");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	return true;
}

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
	bool status = false;
	status = Expr(in, line);
	if (!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == COMMA)
	{
		status = ExprList(in, line);
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

// Prog ::= PROGRAM IDENT CompStmt
bool Prog(istream &in, int &line)
{
	LexItem tok = Parser::GetNextToken(in, line);
	// PROGRAM
	if (tok != PROGRAM)
	{
		ParseError(line, "Missing Program keyword");
		return false;
	}
	// IDENT
	tok = Parser::GetNextToken(in, line);
	if (tok != IDENT)
	{
		ParseError(line, "Missing Program name");
		return false;
	}
	// Parse CompStmt
	bool status = CompStmt(in, line);
	if (!status)
	{
		ParseError(line, "Invalid Program");
		return false;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		return false;
	}
	else if (status)
	{
		std::cout << "(DONE)" << std::endl;
		return true;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

// CompStmt ::= ‘{‘ StmtList ‘}’
bool CompStmt(istream &in, int &line)
{
	// Left Brace
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != LBRACE)
	{
		return false;
	}
	// Parse StmtList
	bool status = StmtList(in, line);
	if (!status)
	{
		ParseError(line, "Incorrect statement list");
		return false;
	}
	// Right Brace
	tok = Parser::GetNextToken(in, line);
	if (tok != RBRACE)
	{
		return false;
	}
	return true;
}

// StmtList ::= Stmt { Stmt }
bool StmtList(istream &in, int &line)
{
	bool status = Stmt(in, line);
	if (!status)
	{
		ParseError(line, "Syntactic error in statement list.");
		return false;
	}
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == RBRACE || tok == DONE)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		status = StmtList(in, line);
	}
	return status;
}

// Stmt ::= DeclStmt | ControlStmt | CompStmt
bool Stmt(istream &in, int &line)
{
	bool status = false;
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == INT || tok == FLOAT || tok == BOOL || tok == CHAR || tok == STRING)
	{
		Parser::PushBackToken(tok);
		status = DeclStmt(in, line);
		if (!status)
			return false;
	}
	else if (tok == IDENT || tok == IF || tok == PRINT)
	{
		Parser::PushBackToken(tok);
		status = ControlStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid control statement.");
			return false;
		}
	}
	else if (tok == LBRACE)
	{
		Parser::PushBackToken(tok);
		status = CompStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid Program");
			return false;
		}
	}
	return status;
}

// DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING ) VarList ;
bool DeclStmt(istream &in, int &line)
{
	// Initial check for data type
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == INT || tok == FLOAT || tok == BOOL || tok == CHAR || tok == STRING)
	{
		bool status = VarList(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			ParseError(line, "Invalid declaration statement.");
			return false;
		}
		tok = Parser::GetNextToken(in, line);

		if (tok != SEMICOL)
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		return true;
	}
	return false;
}

// VarList ::= Var [= Expr] { ,Var [= Expr]}
bool VarList(istream &in, int &line)
{
	// Parse the first variable
	bool status = Var(in, line);
	if (!status)
	{
		ParseError(line, "Invalid variable name");
		return false;
	}

	// Redefinition and defVar Assignment
	if (defVar.find(latestVar) != defVar.end())
	{
		ParseError(line, "Variable Redefinition");
		return false;
	}
	else
	{
		defVar[latestVar] = true; // Add to defVar
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == ASSOP)
	{
		status = Expr(in, line);
		if (!status)
			return false;
		cout << "Initialization of the variable " << latestVar << " in the declaration statement at line " << line << endl;

		tok = Parser::GetNextToken(in, line);
	}

	while (tok == COMMA)
	{
		status = Var(in, line);
		if (!status)
		{
			ParseError(line, "Invalid variable name after comma");
			return false;
		}

		// Redefinition and defVar Assignment
		if (defVar.find(latestVar) != defVar.end())
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		else
		{
			defVar[latestVar] = true;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok == ASSOP)
		{
			status = Expr(in, line);
			if (!status)
			{
				ParseError(line, "Invalid assignment in variable list");
				return false;
			}
			cout << "Initialization of the variable " << latestVar << " in the declaration statement at line " << line << endl;
			tok = Parser::GetNextToken(in, line);
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

// Var ::= IDENT
bool Var(istream &in, int &line)
{
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok != IDENT)
	{
		ParseError(line, "Invalid variable name");
		return false;
	}
	latestVar = tok.GetLexeme();

	return true;
}

// ControlStmt ::= AssignStmt ; | IfStmt | PrintStmt ;
bool ControlStmt(istream &in, int &line)
{
	LexItem tok = Parser::GetNextToken(in, line);

	// AssignStmt;
	if (tok == IDENT)
	{
		Parser::PushBackToken(tok);
		bool status = AssignStmt(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect Assignment Statement");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL)
		{
			line--;
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		return true;
	}

	// IfStmt
	else if (tok == IF)
	{
		bool status = IfStmt(in, line);
		if (!status)
		{
			return false;
		}
		return true;
	}

	// PrintStmt
	else if (tok == PRINT)
	{
		bool status = PrintStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid print statement");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL)
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		return true;
	}
	ParseError(line, "Invalid control statement.");
	return false;
}

// AssignStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
bool AssignStmt(istream &in, int &line)
{
	bool status = false;
	status = Var(in, line);
	if (!status)
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}

	if (defVar.find(latestVar) == defVar.end())
	{
		ParseError(line, "Undeclared Variable");
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != ASSOP &&
		tok != ADDASSOP &&
		tok != SUBASSOP &&
		tok != MULASSOP &&
		tok != DIVASSOP &&
		tok != REMASSOP)
	{
		ParseError(line, "Missing Assignment Operator");
		return false;
	}

	if (!Expr(in, line))
	{
		ParseError(line, "Missing Expression in Assignment Statement");
		return false;
	}
	return true;
}

// IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
bool IfStmt(istream &in, int &line)
{
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != LPAREN)
	{
		ParseError(line, "Missing left parenthesis");
		return false;
	}

	bool status = Expr(in, line);
	if (!status)
	{
		ParseError(line, "Missing if statement condition");
		ParseError(line, "Incorrect IF Statement");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RPAREN)
	{
		ParseError(line, "Missing right parenthesis");
		return false;
	}
	cout << "in IfStmt then-clause at nesting level: " << nestLvl << endl;
	nestLvl++;

	status = Stmt(in, line);
	if (!status)
	{
		ParseError(line, "Invalid statement in IF body");
		return false;
	}

	nestLvl--;

	tok = Parser::GetNextToken(in, line);
	if (tok == ELSE)
	{
		cout << "in IfStmt else-clause at nesting level: " << nestLvl << endl;
		nestLvl++;
		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid statement in ELSE body");
			return false;
		}
		nestLvl--;
	}
	else
	{
		Parser::PushBackToken(tok);
	}

	return true;
}

// Expr ::= LogANDExpr { || LogANDExpr }
bool Expr(istream &in, int &line)
{
	bool status = LogANDExpr(in, line);
	if (!status)
		return false;

	LexItem tok;
	while ((tok = Parser::GetNextToken(in, line)) == OR)
	{
		status = LogANDExpr(in, line);
		if (!status)
			return false;
	}
	Parser::PushBackToken(tok);
	return true;
}

// LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream &in, int &line)
{
	bool status = EqualExpr(in, line);
	if (!status)
		return false;

	LexItem tok;
	while ((tok = Parser::GetNextToken(in, line)) == AND)
	{
		status = EqualExpr(in, line);
		if (!status)
			return false;
	}
	Parser::PushBackToken(tok);
	return true;
}

// EqualExpr ::= RelExpr [ (== | != ) RelExpr ]
bool EqualExpr(istream &in, int &line)
{
	bool status = RelExpr(in, line);
	if (!status)
		return false;

	LexItem tok;
	bool firstEquaExpr = true;

	while ((tok = Parser::GetNextToken(in, line)) == EQ || tok == NEQ)
	{
		if (!firstEquaExpr)
		{
			ParseError(line, "Illegal Equality Expression.");
			return false;
		}
		status = RelExpr(in, line);
		if (!status)
		{
			return false;
		}

		firstEquaExpr = false;
	}
	Parser::PushBackToken(tok);
	return true;
}

// RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream &in, int &line)
{
	bool status = AddExpr(in, line);
	if (!status)
		return false;

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == LTHAN || tok == GTHAN)
	{
		status = AddExpr(in, line);
		if (!status)
			return false;
		tok = Parser::GetNextToken(in, line);
		if (tok == LTHAN || tok == GTHAN)
		{
			ParseError(line, "Illegal Relational Expression.");
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

// AddExpr :: MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream &in, int &line)
{
	bool status = MultExpr(in, line);
	if (!status)
		return false;

	LexItem tok;
	while ((tok = Parser::GetNextToken(in, line)) == PLUS || tok == MINUS)
	{
		status = MultExpr(in, line);
		if (!status)
			return false;
	}
	Parser::PushBackToken(tok);
	return true;
}

// MultExpr ::= UnaryExpr { ( * | / | % ) UnaryExpr }
bool MultExpr(istream &in, int &line)
{
	bool status = UnaryExpr(in, line);
	if (!status)
		return false;

	LexItem tok;
	while ((tok = Parser::GetNextToken(in, line)) == MULT || tok == DIV || tok == REM)
	{
		status = UnaryExpr(in, line);
		if (!status)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
	}

	Parser::PushBackToken(tok);
	return true;
}

// UnaryExpr ::= [( - | + | ! )] PrimaryExpr
bool UnaryExpr(istream &in, int &line)
{
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == MINUS)
	{
		return PrimaryExpr(in, line, -1);
	}
	else if (tok == PLUS)
	{
		return PrimaryExpr(in, line, 1);
	}
	else if (tok == NOT)
	{
		return PrimaryExpr(in, line, 0);
	}

	Parser::PushBackToken(tok);
	return PrimaryExpr(in, line, 1);
}

// PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | '(' Expr ')'
bool PrimaryExpr(istream &in, int &line, int sign)
{
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == IDENT || tok == ICONST || tok == RCONST || tok == SCONST || tok == BCONST || tok == CCONST)
	{
		return true;
	}

	if (tok == LPAREN)
	{
		bool status = Expr(in, line);
		if (!status)
		{
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok != RPAREN)
		{
			ParseError(line, "Missing right parenthesis");
			return false;
		}
		return true;
	}
	return false;
}