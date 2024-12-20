// AssignStmt:= Var = Expr
bool AssignStmt(istream &in, int &line)
{
    bool varstatus = false, status = false;
    LexItem t;
    Value retVal;

    varstatus = Var(in, line, t);
    string identString = t.GetLexeme();
    Token type = SymTable[identString];

    if (varstatus)
    {
        t = Parser::GetNextToken(in, line);

        if (t == ASSOP)
        {
            // cout << identString << endl;
            // cout << TempsResults[identString] << endl;
            // cout << TempsResults[identString].GetType() << endl;
            status = Expr(in, line, retVal);

            if (!status)
            {
                ParseError(line, "Missing Expression in Assignment Statement");
                return status;
            }
            if ((type == ICONST || type == RCONST || type == NIDENT) && retVal.IsString())
            {
                ParseError(line, "Illegal Assignment Operation");
                return false;
            }
            if ((type == NIDENT || SIDENT) && retVal.IsBool())
            {
                ParseError(line, "Illegal ASsignment of a boolean value to a numeric or string variable");
                return false;
            }
            else if (retVal.IsBool())
            {
                TempsResults[identString] = retVal;
            }
            else if (retVal.IsInt())
            {
                TempsResults[identString] = retVal;
            }
            else if (retVal.IsReal())
            {
                TempsResults[identString] = retVal;
            }
            else if (retVal.IsReal())
            {
                TempsResults[identString] = (int)retVal.GetReal();
            }
            else if (retVal.IsInt())
            {
                TempsResults[identString] = Value(static_cast<double>(retVal.GetInt()));
            }
            else if (retVal.IsString())
            {
                TempsResults[identString] = retVal;
            }
            else
            {
                ParseError(line, "Value type does not match.");
                return false;
            }
        }
        else if (t.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
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
} // End of AssignStmt