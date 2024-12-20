# **Lexical Analyzer and Interpreter in C++**

## **Description**  
This project implements a **Lexical Analyzer** and **Interpreter** in C++ to process a mini-C like programming language. 
The **Lexical Analyzer** reads source code, breaks it into meaningful units (tokens), and identifies their types (e.g., keywords, identifiers, operators). 
The **Interpreter** evaluates these tokens and executes instructions directly, supporting basic arithmetic, variable assignments, and control structures.

## **Features**  
- **Lexical Analysis**:  
  - Tokenizes input source code into keywords, identifiers, constants, operators, and delimiters.
  - Detects errors like invalid tokens or unrecognized characters.

- **Interpreter**:  
  - Executes tokenized instructions.
  - Supports arithmetic operations, variable assignment, and simple expressions.
  - Handles basic control flow (e.g., if-else, loops).

- **Extensible Design**:  
  - Modular implementation for easy addition of new features or tokens.
  - Clean separation between lexical analysis and interpretation.

Mini-C Language EBNF
Prog ::= PROGRAM IDENT CompStmt
StmtList ::= Stmt { Stmt }
Stmt ::= DeclStmt | ControlStmt | CompStmt
DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING ) VarList ;
VarList ::= Var [= Expr] { ,Var [= Expr]}
ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
PrintStmt ::= PRINT (ExprList)
CompStmt ::= ‘{‘ StmtList ‘}’
IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
Var ::= IDENT
ExprList ::= Expr { , Expr }
Expr ::= LogANDExpr { || LogANDRxpr }
LogANDExpr ::= EqualExpr { && EqualExpr }
EqualExpr ::= RelExpr [ (== | != ) RelExpr ]
RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
AddExpr :: MultExpr { ( + | - ) MultExpr }
MultExpr ::= UnaryExpr { ( * | / | % ) UnaryExpr }
UnaryExpr ::= [( - | + | ! )] PrimaryExpr
PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )

## **Getting Started**

### **Prerequisites**  
- A C++ compiler (e.g., GCC, Clang, or MSVC).
- Basic knowledge of C++ and programming language syntax.

### **Installation**
1. Clone the repository:  
   ```bash
   https://github.com/JustinCordova/CS280_FALL24.git
2. Navigate to Final Project Directory 'PA3'
   ```bash
   cd PA3
3. Compile and Run with specified file
   ```bash
   g++ -o PA3 prog3.cpp parseInterp.cpp va.cpp parser.cpp
   ./PA3 TC/testprog9

# **RA1-9**

## **Description**  
Other mini projects relating to the class CS280

