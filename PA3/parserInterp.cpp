#include "parserInterp.h"

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; // Container of temporary locations of Value objects for results of expressions, variables values and constants
queue<Value> *ValQue;			 // declare a pointer variable to a queue of Value objects

bool g_exec = true;
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

string TokenToString(Token t)
{
	switch (t)
	{
	case IF:
		return "IF";
	case ELSE:
		return "ELSE";
	case PRINT:
		return "PRINT";
	case INT:
		return "INT";
	case FLOAT:
		return "FLOAT";
	case CHAR:
		return "CHAR";
	case STRING:
		return "STRING";
	case BOOL:
		return "BOOL";
	case PROGRAM:
		return "PROGRAM";
	case TRUE:
		return "TRUE";
	case FALSE:
		return "FALSE";
	case IDENT:
		return "IDENT";
	case ICONST:
		return "ICONST";
	case RCONST:
		return "RCONST";
	case SCONST:
		return "SCONST";
	case BCONST:
		return "BCONST";
	case CCONST:
		return "CCONST";
	case PLUS:
		return "PLUS";
	case MINUS:
		return "MINUS";
	case MULT:
		return "MULT";
	case DIV:
		return "DIV";
	case ASSOP:
		return "ASSOP";
	case EQ:
		return "EQ";
	case NEQ:
		return "NEQ";
	case ADDASSOP:
		return "ADDASSOP";
	case SUBASSOP:
		return "SUBASSOP";
	case MULASSOP:
		return "MULASSOP";
	case DIVASSOP:
		return "DIVASSOP";
	case REMASSOP:
		return "REMASSOP";
	case GTHAN:
		return "GTHAN";
	case LTHAN:
		return "LTHAN";
	case AND:
		return "AND";
	case OR:
		return "OR";
	case NOT:
		return "NOT";
	case REM:
		return "REM";
	case COMMA:
		return "COMMA";
	case SEMICOL:
		return "SEMICOL";
	case LPAREN:
		return "LPAREN";
	case RPAREN:
		return "RPAREN";
	case DOT:
		return "DOT";
	case LBRACE:
		return "LBRACE";
	case RBRACE:
		return "RBRACE";
	case ERR:
		return "ERR";
	case DONE:
		return "DONE";
	default:
		return "UNKNOWN";
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

// Prog ::= PROGRAM IDENT CompStmt
bool Prog(istream &in, int &line)
{
	bool sl = false;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == PROGRAM)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT)
		{
			sl = CompStmt(in, line);
			if (!sl)
			{
				ParseError(line, "Invalid Program");
				return false;
			}
			cout << "(DONE)" << endl;
			return true;
		}
		else
		{
			ParseError(line, "Missing Program name");
			return false;
		}
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	else
	{
		ParseError(line, "Missing Program keyword");
		return false;
	}
}

// CompStmt ::= ‘{‘ StmtList ‘}’
bool CompStmt(istream &in, int &line)
{
	bool status = false;

	LexItem t = Parser::GetNextToken(in, line);
	if (t == LBRACE)
	{
		status = StmtList(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect statement list");
			return false;
		}
		LexItem t = Parser::GetNextToken(in, line);
		if (t == RBRACE)
		{
			return true;
		}
		else if (t == DONE)
		{
			ParseError(line, "Missing end of program right brace.");
			return false;
		}
		else
		{
			ParseError(line, "Missing right brace.");
			return false;
		}
	}
	return status;
}

// StmtList ::= Stmt { Stmt }
bool StmtList(istream &in, int &line)
{
	bool status;
	LexItem tok;

	status = Stmt(in, line);
	tok = Parser::GetNextToken(in, line);
	while (status && tok != RBRACE)
	{
		Parser::PushBackToken(tok);

		status = Stmt(in, line);
		tok = Parser::GetNextToken(in, line);
	}
	if (!status)
	{
		ParseError(line, "Syntactic error in statement list.");
		return false;
	}
	Parser::PushBackToken(tok);
	return true;
}

// Stmt ::= DeclStmt ; | ControlStmt ; | CompStmt
bool Stmt(istream &in, int &line)
{
	bool status = true;

	LexItem t = Parser::GetNextToken(in, line);
	switch (t.GetToken())
	{

	case PRINT:
	case IF:
	case IDENT:
		Parser::PushBackToken(t);
		status = ControlStmt(in, line);

		if (!status)
		{
			ParseError(line, "Invalid control statement.");
			return false;
		}

		break;
	case INT:
	case FLOAT:
	case CHAR:
	case BOOL:
	case STRING:
		Parser::PushBackToken(t);
		status = DeclStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid declaration statement.");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		break;
	case LBRACE:
		Parser::PushBackToken(t);
		status = CompStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid compound statement.");
			return false;
		}

		break;
	default:
		Parser::PushBackToken(t);

		return false;
	}

	return status;
}

// DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING) VarList;
bool DeclStmt(istream &in, int &line)
{
	bool status = false;
	LexItem idtok;

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == INT || tok == FLOAT || tok == CHAR || tok == BOOL || tok == STRING)
	{
		idtok = tok;
		status = VarList(in, line, idtok);
		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return status;
		}

		return true;
	}
	return true;
}

// ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
bool ControlStmt(istream &in, int &line)
{
	bool status;

	LexItem t = Parser::GetNextToken(in, line);
	switch (t.GetToken())
	{

	case PRINT:
		status = PrintStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect Print Statement");
			return false;
		}
		t = Parser::GetNextToken(in, line);

		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}

		break;

	case IDENT:
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect Assignment Statement");
			return false;
		}
		t = Parser::GetNextToken(in, line);

		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}

		break;
	case IF:
		status = IfStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect IF Statement");
			return false;
		}

		break;

	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
} // End of ControlStmt

// PrintStmt:= PRINT (ExprList)
bool PrintStmt(istream &in, int &line)
{
	LexItem t;
	ValQue = new queue<Value>;

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
		while (!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		while (!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}
	// Only print the values if g_exec is true
	if (g_exec)
	{
		while (!(*ValQue).empty())
		{
			Value nextVal = (*ValQue).front();
			cout << nextVal;
			ValQue->pop();
		}
		cout << endl;
	}

	// Clean up the queue memory
	delete ValQue;
	return true;
}

// IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
bool IfStmt(istream &in, int &line)
{
	g_exec = false;
	bool ex = false, status;
	static int nestlevel = 0;
	LexItem t;
	Value retVal, prevVal;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	// false state of processing
	ex = Expr(in, line, retVal);
	if (!ex)
	{
		ParseError(line, "Missing if statement condition");
		return false;
	}
	prevVal = retVal;
	//cout << retVal << endl;
	if (retVal.GetType() == true || retVal.GetType() == true)
	{
		g_exec = true;
		t = Parser::GetNextToken(in, line);
		if (t != RPAREN)
		{

			ParseError(line, "Missing Right Parenthesis");
			return false;
		}
		nestlevel += 1;
		// cout << "in IfStmt then-clause at nesting level: " << nestlevel << endl;
		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "If-Stmt Syntax Error");
			return false;
		}
	}
	else if (!(retVal.GetType() == (true || false)))
	{
		ParseError(line, "Invalid type for If statement condition");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if (t == ELSE)
	{
		if (retVal.GetType() == false)
		{
			g_exec = true;
		}
		// cout << "in IfStmt else-clause at nesting level: " << nestlevel << endl;
		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}

		nestlevel--;
		return true;
	}

	nestlevel--;
	Parser::PushBackToken(t);
	return true;
}

bool checkSameType(const string &identStr, Value &rVal)
{
	Token tokType = SymTable[identStr];
	ValType valType = rVal.GetType();

	if ((tokType == INT && valType == VINT) ||
		(tokType == FLOAT && valType == VREAL) ||
		(tokType == BOOL && valType == VBOOL) ||
		(tokType == STRING && valType == VSTRING) ||
		(tokType == CHAR && valType == VCHAR))
	{
		return true;
	}
	// cout << tokType << endl;
	// cout << valType << endl;
	return false;
}

void checkConvert(const string &identStr, Value &rVal)
{
	Token tokType = SymTable[identStr];
	if (tokType == INT && rVal.IsReal())
	{
		// Float to Int
		rVal = Value(static_cast<int>(rVal.GetReal()));
	}
	else if (tokType == INT && rVal.IsChar())
	{
		// Char to Int
		rVal = Value(static_cast<int>(rVal.GetChar()));
	}
	else if (tokType == FLOAT && rVal.IsInt())
	{
		// Int to Float
		rVal = Value(static_cast<float>(rVal.GetInt()));
	}
	else if (tokType == FLOAT && rVal.IsChar())
	{
		// Char to Float
		rVal = Value(static_cast<float>(rVal.GetChar()));
	}
	else if (tokType == CHAR && rVal.IsInt())
	{
		// Int to Char
		rVal = Value(static_cast<char>(rVal.GetInt()));
	}
}

void PrintTempsResults(const map<string, Value> &TempsResults)
{
	for (const auto &pair : TempsResults)
	{
		cout << "Key: " << pair.first << ", Value: " << pair.second.IsReal() << endl;
	}
}
// AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
bool AssignStmt(istream &in, int &line)
{
	bool varstatus = false, status = false;
	LexItem tok, idtok;
	Value rVal, retVal;
	varstatus = Var(in, line, idtok);
	string identStr = idtok.GetLexeme();
	if (varstatus)
	{
		tok = Parser::GetNextToken(in, line);

		if (tok == ASSOP || tok == ADDASSOP || tok == SUBASSOP || tok == MULASSOP || tok == DIVASSOP || tok == REMASSOP)
		{
			status = Expr(in, line, rVal);
			if (!status)
			{
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
			checkConvert(identStr, rVal);
			// cout << SymTable[identStr] << endl;
			// cout << rVal << endl;
			if (tok == ASSOP && checkSameType(identStr, rVal))
			{
				TempsResults[identStr] = rVal;
			}
			else if (tok == ADDASSOP && checkSameType(identStr, rVal))
			{
				if (TempsResults.find(identStr) == TempsResults.end())
				{
					ParseError(line, "Invalid Expression evaluation in Assignment Statement");
					return false;
				}
				TempsResults[identStr] = TempsResults[identStr] + rVal;
			}
			else if (tok == SUBASSOP && checkSameType(identStr, rVal))
			{
				if (TempsResults.find(identStr) == TempsResults.end())
				{
					ParseError(line, "Invalid Expression evaluation in Assignment Statement");
					return false;
				}
				TempsResults[identStr] = TempsResults[identStr] - rVal;
			}
			else if (tok == MULASSOP && checkSameType(identStr, rVal))
			{
				if (TempsResults.find(identStr) == TempsResults.end())
				{
					ParseError(line, "Invalid Expression evaluation in Assignment Statement");
					return false;
				}
				TempsResults[identStr] = TempsResults[identStr] * rVal;
			}
			else if (tok == DIVASSOP && checkSameType(identStr, rVal))
			{
				if (TempsResults.find(identStr) == TempsResults.end())
				{
					ParseError(line, "Invalid Expression evaluation in Assignment Statement");
					return false;
				}
				TempsResults[identStr] = TempsResults[identStr] / rVal;
			}
			else if (tok == REMASSOP && checkSameType(identStr, rVal))
			{
				if (TempsResults.find(identStr) == TempsResults.end())
				{
					ParseError(line, "Invalid Expression evaluation in Assignment Statement");
					return false;
				}
				TempsResults[identStr] = TempsResults[identStr] % rVal;
			}
			/*for (const auto &pair : SymTable)
			{
				cout << "Key: " << pair.first << ", Value: " << TokenToString(pair.second) << endl;
			}*/
			// PrintTempsResults(TempsResults);
			if (!checkSameType(identStr, rVal))
			{
				ParseError(line, "Illegal mixed-mode assignment operation");
				return false;
			}
		}
		else if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		else
		{
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;
}

// VarList ::= Var [= Expr] {, Var [= Expr]}
bool VarList(istream &in, int &line, LexItem &idtok)
{
	bool status = false, exprstatus = false;
	string identstr;
	Value retVal;

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == IDENT)
	{
		identstr = tok.GetLexeme();
		SymTable[identstr] = idtok.GetToken();
		// idtok is from declStmt == float
		if (!(defVar.find(identstr) != defVar.end()))
		{
			defVar[identstr] = true;
		}
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
	}
	else
	{

		ParseError(line, "Missing Variable Name");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == ASSOP)
	{
		exprstatus = Expr(in, line, retVal);
		if (!exprstatus)
		{
			ParseError(line, "Incorrect initialization for a variable.");
			return false;
		}
		TempsResults[identstr] = retVal;

		tok = Parser::GetNextToken(in, line);

		if (tok == COMMA)
		{

			status = VarList(in, line, idtok);
		}
		else
		{
			Parser::PushBackToken(tok);
			return true;
		}
	}
	else if (tok == COMMA)
	{

		status = VarList(in, line, idtok);
	}
	else if (tok == ERR)
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

// Var:= ident
bool Var(istream &in, int &line, LexItem &idtok)
{
	string identstr;
	LexItem tok = Parser::GetNextToken(in, line);
	idtok = tok;

	if (tok == IDENT)
	{
		identstr = tok.GetLexeme();
		// idtok is from declStmt == float
		if (!(defVar.find(identstr) != defVar.end()))
		{
			defVar[identstr] = true;
			SymTable[identstr] = idtok.GetToken();
		}
		return true;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
	bool status = false;
	Value retVal;

	status = Expr(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA)
	{

		status = ExprList(in, line);
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

// Expr ::= LogANDExpr { || LogANDRxpr }
bool Expr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	Value rVal;
	bool status = LogANDExpr(in, line, retVal);

	if (!status)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == OR)
	{
		status = LogANDExpr(in, line, rVal);
		if (!status)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		retVal = retVal || rVal;

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

// LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	Value rVal;
	bool status = EqualExpr(in, line, retVal);

	if (!status)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == AND)
	{
		status = EqualExpr(in, line, rVal);
		if (!status)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (!rVal.IsBool())
		{
			ParseError(line, "Invalid opernads for logical AND operator");
			return false;
		}
		retVal = retVal && rVal;

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}

	Parser::PushBackToken(tok);
	return true;
}

// EqualExpr ::= RelExpr [ (== | !=) RelExpr ]
bool EqualExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	Value rVal;
	bool status = RelExpr(in, line, retVal);

	if (!status)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == EQ || tok == NEQ)
	{
		status = RelExpr(in, line, rVal);

		if (!status)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == EQ)
		{
			retVal = retVal == rVal;
		}
		else if (tok == NEQ)
		{
			retVal = retVal != rVal;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok == EQ || tok == NEQ)
		{
			ParseError(line, "Illegal Equality Expression.");
			return false;
		}
		else if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);

	return true;
}

// RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	Value rVal;
	bool status = AddExpr(in, line, retVal);

	if (!status)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == LTHAN || tok == GTHAN)
	{
		status = AddExpr(in, line, rVal);
		if (!status)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == LTHAN)
		{
			retVal = retVal < rVal;
		}
		else if (tok == GTHAN)
		{
			if (rVal.IsInt() && retVal.IsString())
			{
				ParseError(line, "Invalid opernads for relational operators");
				return false;
			}
			retVal = retVal > rVal;
		}

		tok = Parser::GetNextToken(in, line);

		if (tok == LTHAN || tok == GTHAN)
		{
			ParseError(line, "Illegal Relational Expression.");
			return false;
		}
		else if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

// AddExpr :: MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	Value rVal;
	bool status = MultExpr(in, line, retVal);

	if (!status)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == PLUS || tok == MINUS)
	{
		status = MultExpr(in, line, rVal);
		if (!status)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == PLUS)
		{
			retVal = retVal + rVal;
		}
		else if (tok == MINUS)
		{
			retVal = retVal - rVal;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

// MultExpr ::= UnaryExpr { ( * | / | %) UnaryExpr }
bool MultExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	Value rVal;
	bool status = UnaryExpr(in, line, retVal);

	if (!status)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == MULT || tok == DIV || tok == REM)
	{
		status = UnaryExpr(in, line, rVal);

		if (!status)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == MULT)
		{
			if (!(rVal.IsInt() || rVal.IsReal()))
			{
				ParseError(line, "Illegal operand type for the operation.");
				return false;
			}
			retVal = retVal * rVal;
		}
		else if (tok == DIV)
		{
			if ((rVal.IsInt() && rVal.GetInt() == 0) || (rVal.IsReal() && rVal.GetReal() == 0.0))
			{
				ParseError(line, "Run-Time Error-Illegal division by Zero");
				return false;
			}
			retVal = retVal / rVal;
		}
		else if (tok == REM)
		{
			if (!(rVal.IsReal()))
			{
				ParseError(line, "Invalid operand for the REM operator");
				return false;
			}
			retVal = retVal % rVal;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

// UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok = Parser::GetNextToken(in, line);
	int sign = 0;
	if (tok == MINUS)
	{
		sign = -1;
	}
	else if (tok == PLUS)
	{
		sign = 1;
	}
	else if (tok == NOT)
	{
		sign = 2;
	}
	else
		Parser::PushBackToken(tok);
	bool status = PrimaryExpr(in, line, sign, retVal);
	if (tok == MINUS)
	{
		retVal = retVal * -1;
	}
	if (tok == PLUS)
	{
		retVal = retVal * 1;
	}
	else if (tok == NOT)
	{
		retVal = !retVal;
	}
	return status;
}

// PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )
bool PrimaryExpr(istream &in, int &line, int sign, Value &retVal)
{

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == IDENT)
	{
		string lexeme = tok.GetLexeme();
		if (TempsResults.find(lexeme) == TempsResults.end())
		{
			ParseError(line, "Using uninitialized Variable");
			return false;
		}
		retVal = Value(TempsResults[lexeme]);
		return true;
	}
	else if (tok == ICONST)
	{
		if (sign == 0)
		{
			retVal = Value(stoi(tok.GetLexeme()));
		}
		else if (sign == 1)
		{
			retVal = Value(abs(stoi(tok.GetLexeme())));
		}
		else if (sign == -1)
		{
			retVal = Value(-1 * abs(stoi(tok.GetLexeme())));
		}
		else if (sign == 2)
		{
			ParseError(line, "Illegal Operand Type for NOT Operator");
			return false;
		}
		return true;
	}
	else if (tok == SCONST)
	{
		if (sign != 0)
		{
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if (tok == RCONST)
	{
		if (sign == 0)
		{
			retVal = Value(stod(tok.GetLexeme()));
		}
		else if (sign == 1)
		{
			retVal = Value(abs(stod(tok.GetLexeme())));
		}
		else if (sign == -1)
		{
			retVal = Value(-1 * abs(stod(tok.GetLexeme())));
		}
		return true;
	}
	else if (tok == BCONST)
	{
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if (tok == CCONST)
	{
		retVal = Value(tok.GetLexeme()[0]);
		return true;
	}
	else if (tok == LPAREN)
	{
		bool ex = Expr(in, line, retVal);
		if (!ex)
		{
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if (Parser::GetNextToken(in, line) == RPAREN)
			return ex;
		else
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}