#include <fstream>
#include "parser.h"
#include "utils.h"

void printAST2(AST *ast, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << ". ";
    }
    cout << ast->value << endl;
    if (ast->left)
    {
        printAST2(ast->left, n + 1);
    }
    if (ast->right)
    {
        printAST2(ast->right, n);
    }
}

void debugPrint(stack<AST *> ast_stack)
{
    while (ast_stack.size() > 0)
    {
        printAST2(ast_stack.top(), 0);
        ast_stack.pop();
    }
}

Parser::Parser()
{
    fin = new ifstream();
    line = 0;
}

Parser::Parser(istream *_fin)
{
    fin = _fin;
    line = 0;
}

/**
 * Grammar for Assignment
 * @return void
 */
void Parser::Assignment()
{
    Name();
    if (IsToken(":="))
    {
        ReadToken(":=");
        Expression();
        BuildTree("assign", 2);
    }
    else if (IsToken(":=:"))
    {
        ReadToken(":=:");
        Name();
        BuildTree("swap", 2);
    }
}

/**
 * Grammar for Body
 * @return void
 */
void Parser::Body()
{
    ReadToken("begin");
    int n = 0;
    do
    {
        if (IsToken(";"))
        {
            ReadToken(";");
        }
        Statement();
        n++;
    } while (IsToken(";"));
    ReadToken("end");
    BuildTree("block", n);
}

/**
 * Method for BuildTree
 * @param string node
 * @param int num_pop
 * @return void
 */
void Parser::BuildTree(string node, int num_pop)
{
    AST *p = nullptr;
    for (int i = 0; i < num_pop; i++)
    {
        AST *c = ast_stack.top();
        ast_stack.pop();
        c->right = p;
        p = c;
    }
    AST *new_node = new AST(node + "(" + to_string(num_pop) + ")", p, nullptr);
    ast_stack.push(new_node);
}

/**
 * Gramnmar for Caseclause
 * @return void
 */
void Parser::Caseclause()
{
    do
    {
        CaseExpression();
    } while (IsToken(","));
    ReadToken(";");
    Statement();
}

/**
 * Grammar for Caseclauses
 * @return void
 */
void Parser::Caseclauses()
{
    do
    {
        Caseclause();
    } while (IsToken(";"));
}

/**
 * Grammar for CaseExpression
 * @return void
 */
void Parser::CaseExpression()
{
    ConstValue();
    if (IsToken(".."))
    {
        ConstValue();
    }
}

/**
 * Grammar for Const
 * @return void
 */
void Parser::Const()
{
    Name();
    ReadToken("=");
}

/**
 * Grammar for Consts
 * @return void
 */
void Parser::Consts()
{
    int n = 0;
    if (IsToken("const"))
    {
        ReadToken("const");
        Const();
        n++;
    }
    BuildTree("consts", n);
}

/**
 * Grammar for CosntValue
 * @return void
 */
void Parser::ConstValue()
{
    if (IsInteger())
    {
        ReadInteger();
    }
    else if (IsChar())
    {
        ReadChar();
    }
    else if (IsIdentifier())
    {
        ReadIdentifier();
    }
    else
    {
        string message = "Nont a valid ConstValue on " + to_string(line);
        throw message;
    }
}

/**
 * Grammar for Dcln
 * @return void
 */
void Parser::Dcln()
{
    ReadWhitespace();
    int n = 0;
    do
    {
        if (IsToken(","))
        {
            ReadToken(",");
        }
        Name();
        n++;
    } while (IsToken(","));
    ReadToken(":");
    Name();
    n++;
    BuildTree("var", n);
}

/**
 * Grammar for Dclns
 * @return void
 */
void Parser::Dclns()
{
    int n = 0;
    if (IsToken("var"))
    {
        ReadToken("var");
        do
        {
            Dcln();
            ReadToken(";");
            n++;
        } while (IsIdentifier() && !IsPredefinedToken());
    }
    BuildTree("dclns", n);
}

/**
 * Grammar for Expression
 * @return void
 */
void Parser::Expression()
{
    Term();
    if (IsToken("<="))
    {
        ReadToken("<=");
        Term();
        BuildTree("<=", 2);
    }
    else if (IsToken("<"))
    {
        ReadToken("<");
        Term();
        BuildTree("<", 2);
    }
    else if (IsToken(">="))
    {
        ReadToken(">=");
        Term();
        BuildTree(">=", 2);
    }
    else if (IsToken(">"))
    {
        ReadToken(">");
        Term();
        BuildTree(">", 2);
    }
    else if (IsToken("="))
    {
        ReadToken("=");
        Term();
        BuildTree("=", 2);
    }
    else if (IsToken("<>"))
    {
        ReadToken("<>");
        Term();
        BuildTree("<>", 2);
    }
}

/**
 * Grammar for Factor
 * @return void
 */
void Parser::Factor()
{
    Primary();

    if (IsToken("*"))
    {
        ReadToken("*");
        Primary();
        BuildTree("*", 2);
    }
    else if (IsToken("/"))
    {
        ReadToken("/");
        Primary();
        BuildTree("/", 2);
    }
    else if (IsToken("and"))
    {
        ReadToken("and");
        Primary();
        BuildTree("and", 2);
    }
    else if (IsToken("mod"))
    {
        ReadToken("mod");
        Primary();
        BuildTree("mod", 2);
    }
}

/**
 * Grammar for Fcn
 * @return void
 */
void Parser::Fcn()
{
    ReadToken("function");
    Name();
    ReadToken("(");
    Params();
    ReadToken(")");
    ReadToken(":");
    Name();
    ReadToken(";");
    Consts();
    Types();
    Dclns();
    Body();
    Name();
    ReadToken(";");
    BuildTree("fcn", 8);
}

/**
 * Grammar for ForStat
 * @return void
 */
void Parser::ForStat()
{
    if (IsIdentifier())
    {
        Assignment();
    }
}

/**
 * Grammar for ForExp
 * @return void
 */
void Parser::ForExp()
{
    if (IsExpression())
    {
        Expression();
    }
}

/**
 * Method that checks if the next token is char
 * @return bool
 */
bool Parser::IsChar()
{
    int original_position = fin->tellg();
    ReadWhitespace();
    if (my_c == '\'')
    {
        fin->get(my_c);
        fin->get(my_c);
        if (my_c == '\'')
        {
            fin->get(my_c);
        }
        else
        {
            return false;
        }

        fin->seekg(original_position - 1);
        fin->get(my_c);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Method that checks if the next token is an expression
 * @return bool
 */
bool Parser::IsExpression()
{
    return IsTerm();
}

/**
 * Method that checks if next token is a factor
 * @return bool
 */
bool Parser::IsFactor()
{
    return IsPrimary();
}

/**
 * Method that checks if next token is an identifier
 * @return bool
 */
bool Parser::IsIdentifier()
{
    int original_position = fin->tellg();
    ReadWhitespace();

    if (isIdentifierStart(my_c))
    {
        while (!isWhiteSpace(my_c) && !(fin->eof()))
        {
            if (!isIdentifierCharacter(my_c))
            {
                break;
            }
            fin->get(my_c);
        }
        fin->seekg(original_position - 1);
        fin->get(my_c);
        return true;
    }
    else
    {
        fin->seekg(original_position - 1);
        fin->get(my_c);
        return false;
    }
}

/**
 * Method that checks if the next token is a Primary
 * @return bool
 */
bool Parser::IsPrimary()
{
    return IsToken("-") ||
           IsToken("+") ||
           IsToken("not") ||
           IsToken("eof") ||
           IsIdentifier() ||
           IsInteger() ||
           IsChar() ||
           IsToken("(") ||
           IsToken("succ") ||
           IsToken("pred") ||
           IsToken("chr") ||
           IsToken("ord");
}

/**
 * Method that checks if token is a predefined token
 * @return bool
 */
bool Parser::IsPredefinedToken()
{
    int original_position = fin->tellg();
    ReadWhitespace();
    string token;
    while (my_c != ' ' && my_c != '\n' && !fin->eof())
    {
        token += my_c;
        fin->get(my_c);
    }
    fin->clear();
    fin->seekg(original_position - 1);
    fin->get(my_c);

    return token == "\n" ||
           token == "program" ||
           token == "var" ||
           token == "const" ||
           token == "type" ||
           token == "function" ||
           token == "return" ||
           token == "begin" ||
           token == "end" ||
           token == ":=:" ||
           token == ":=" ||
           token == "output" ||
           token == "if" ||
           token == "then" ||
           token == "else" ||
           token == "while" ||
           token == "do" ||
           token == "case" ||
           token == "of" ||
           token == ".." ||
           token == "otherwise" ||
           token == "repeat" ||
           token == "for" ||
           token == "until" ||
           token == "loop" ||
           token == "pool" ||
           token == "exit" ||
           token == "<=" ||
           token == "<>" ||
           token == "<" ||
           token == ">=" ||
           token == ">" ||
           token == "=" ||
           token == "mod" ||
           token == "and" ||
           token == "or" ||
           token == "not" ||
           token == "read" ||
           token == "succ" ||
           token == "pred" ||
           token == "chr" ||
           token == "ord" ||
           token == "eof" ||
           token == "{" ||
           token == ":" ||
           token == ";" ||
           token == "." ||
           token == "," ||
           token == "(" ||
           token == ")" ||
           token == "+" ||
           token == "-" ||
           token == "*" ||
           token == "/";
}

/**
 * Method that checks if the next token is a Term
 * @return bool
 */
bool Parser::IsTerm()
{
    return IsFactor();
}

/**
 * Method that checks if the following characters make an integer
 * @return bool
 */
bool Parser::IsInteger()
{
    int original_position = fin->tellg();
    ReadWhitespace();
    string val = "";
    if (0 <= my_c - '0' && my_c - '0' <= 9)
    {
        while (!fin->eof() && 0 <= my_c - '0' && my_c - '0' <= 9 && my_c != '\n' && my_c != ' ')
        {
            val += my_c;
            fin->get(my_c);
        }

        fin->clear();
        fin->seekg(original_position - 1);
        fin->get(my_c);
        return true;
    }
    else
    {
        fin->clear();
        fin->seekg(original_position - 1);
        fin->get(my_c);
        return false;
    }
}

/**
 * Method that checks if the following characters is
 * a token
 * @return bool
 */
bool Parser::IsToken(string token)
{
    int original_pos = fin->tellg();
    ReadWhitespace();
    bool is_token = true;
    for (int i = 0; i < token.size(); i++)
    {
        if (token[i] != my_c)
        {
            is_token = false;
            break;
        }
        fin->get(my_c);
    }
    fin->clear();
    fin->seekg(original_pos - 1);
    fin->get(my_c);

    return is_token;
}

/**
 * Grammar for LitList
 * @return void
 */
void Parser::LitList()
{
    ReadToken("(");
    ReadWhitespace();
    int n = 0;
    do
    {
        if (IsToken(","))
        {
            ReadToken(",");
        }
        Name();
        n++;
    } while (IsToken(","));
    ReadToken(")");
    BuildTree("litlist", n);
}

/**
 * Grammar for Name
 * @return void
 */
void Parser::Name()
{
    ReadIdentifier();
    BuildTree("<identifier>", 1);
}

/**
 * Grammar for OtherwiseClause
 * @return void
 */
void Parser::OtherwiseClause()
{
    if (IsToken("otherwise"))
    {
        ReadToken("otherwise");
        Statement();
    }
    else
    {
        ReadToken(";");
    }
}

/**
 * Grammar for out expression
 * @return void
 */
void Parser::OutExp()
{
    if (IsExpression())
    {
        Expression();
        BuildTree("integer", 1);
    }
    else
    {
        StringNode();
        BuildTree("string", 1);
    }
}

/**
 * Grammar for Params
 * @return void
 */
void Parser::Params()
{
    int n = 0;
    do
    {
        if (IsToken(";"))
        {
            ReadToken(";");
        }
        Dcln();
        n++;
    } while (IsToken(";"));
    BuildTree("params", n);
}

/**
 * Grammar for Primary
 * @return void
 */
void Parser::Primary()
{
    if (IsToken("-"))
    {
        ReadToken("-");
        Primary();
        BuildTree("-", 1);
    }
    else if (IsToken("+"))
    {
        ReadToken("+");
        Primary();
        BuildTree("+", 1);
    }
    else if (IsToken("not"))
    {
        ReadToken("not");
        Primary();
        BuildTree("note", 1);
    }
    else if (IsToken("eof"))
    {
        ReadToken("eof");
        BuildTree("eof", 1);
    }
    else if (IsIdentifier())
    {
        Name();
        int n = 1;
        if (IsToken("("))
        {
            ReadToken("(");
            do {
                if (IsToken(",")) {
                    ReadToken(",");
                }
                Expression();
                n++;
            } while (IsToken(","));
            ReadToken(")");
        }
        BuildTree("call", n);
    }
    else if (IsInteger())
    {
        ReadInteger();
    }
    else if (IsChar())
    {
        ReadChar();
    }
}

/**
 * Method that reads a char
 * @return void
 */
void Parser::ReadChar()
{
    ReadWhitespace();
    ReadToken("\'");
    fin->get(my_c);
    ReadToken("\'");
    BuildTree("char", 0);
}

/**
 * Method that iterates through the comment characters
 * @return void
 */
void Parser::ReadComment()
{
    // Keep reading charaters until the brackets count
    // goes to zero
    // '{' is +1, '}' is -1
    int openBrackets = 0;
    do
    {
        if (my_c == '\n')
        {
            line++;
        }
        fin->get(my_c);
        if (my_c == '{')
        {
            openBrackets++;
        }
        else if (my_c == '}')
        {
            openBrackets--;
        }
        else if (isIdentifierCharacter(my_c) && openBrackets == 0)
        {
            return;
        }
    } while (openBrackets > 0);

    // Move to next char
    fin->get(my_c);
}

/**
 * Method that reads an identifier
 * @return void
 */
void Parser::ReadIdentifier()
{
    // First skip through all whitespace
    ReadWhitespace();

    // Is it a valid identifier start char
    if (isIdentifierStart(my_c))
    {
        string identifier;
        // As long as it's valid identifier character
        while (isIdentifierCharacter(my_c))
        {
            identifier += my_c;
            fin->get(my_c);
        }

        BuildTree(identifier, 0);
    }
    else
    {
        cout << "Invalid identifier at line " << line + 1 << endl;
        throw "";
    }
}

/**
 * Method that reads an integer
 * @return void
 */
void Parser::ReadInteger()
{
    ReadWhitespace();
    string val = "";
    while (!fin->eof() && 0 <= my_c - '0' && my_c - '0' <= 9)
    {
        val += my_c;
        fin->get(my_c);
    }
    BuildTree("integer", 0);
}

/**
 * Method that reads the specified token
 * @return void
 */
void Parser::ReadToken(string token)
{
    // First skip all whitespace
    ReadWhitespace();

    // If there is a single character that mismatch
    // throw error
    for (int i = 0; i < token.size(); i++)
    {
        if (my_c != token[i])
        {
            cout << "Invalid token: " << my_c << " at line " << line << endl;
            throw -1;
        }
        fin->get(my_c);
    }
}

/**
 * Method that reads through all whitespace
 * @return void
 */
void Parser::ReadWhitespace()
{
    // While there is a whitespace move to the next char
    do
    {
        // Counting the line is useful for error logs
        if (my_c == '\n')
        {
            line++;
        }

        if (!isWhiteSpace(my_c))
        {
            break;
        }

        fin->get(my_c);
    } while (isWhiteSpace(my_c));
}

/**
 * Grammar for statement
 * @return void
 */
void Parser::Statement()
{
    if (IsToken("output"))
    {
        ReadToken("output");
        ReadToken("(");
        int n = 0;
        do
        {
            OutExp();
            n++;
        } while (IsToken(","));
        ReadToken(")");
        BuildTree("output", n);
    }
    else if (IsToken("if"))
    {
        ReadToken("if");
        Expression();
        ReadToken("then");
        Statement();
        int n = 2;
        if (IsToken("else"))
        {
            Statement();
            n++;
        }
        BuildTree("if", n);
    }
    else if (IsToken("while"))
    {
        ReadToken("while");
        Expression();
        ReadToken("do");
        Statement();
        BuildTree("while", 2);
    }
    else if (IsToken("repeat"))
    {
        ReadToken("repeat");
        int n = 0;
        do
        {
            if (IsToken(";"))
            {
                ReadToken(";");
            }
            Statement();
            n++;
        } while (IsToken(";"));
        ReadToken("until");
        Expression();
        BuildTree("repeat", n + 1);
    }
    else if (IsToken("for"))
    {
        ReadToken("for");
        ReadToken("(");
        ForStat();
        ReadToken(";");
        ForExp();
        ReadToken(";");
        ForStat();
        ReadToken(")");
        Statement();
        BuildTree("for", 4);
    }
    else if (IsToken("loop"))
    {
        ReadToken("loop");
        int n = 0;
        do
        {
            if (IsToken(";"))
            {
                ReadToken(";");
            }
            Statement();
            n++;
        } while (IsToken(";"));
        ReadToken("pool");
        BuildTree("loop", n);
    }
    else if (IsToken("case"))
    {
        ReadToken("case");
        Expression();
        ReadToken("of");
        Caseclauses();
        OtherwiseClause();
        ReadToken("end");
        BuildTree("case", 3);
    }
    else if (IsToken("read"))
    {
        ReadToken("read");
        ReadToken("(");
        int n = 0;
        do
        {
            if (IsToken(","))
            {
                ReadToken(",");
            }
            Name();
            n++;
        } while (IsToken(","));
        ReadToken(")");
        BuildTree("read", n);
    }
    else if (IsToken("return"))
    {
        ReadToken("return");
        Expression();
        BuildTree("return", 1);
    }
    else if (IsToken("begin"))
    {
        ReadToken("begin");
        Body();
        BuildTree("begin", 1);
    }
    else if (IsIdentifier())
    {
        Assignment();
        BuildTree("identifier", 1);
    }
}

/**
 * Grammar for StringNode
 * @return void
 */
void Parser::StringNode()
{
    ReadWhitespace();
    ReadToken("\"");
    do
    {
        fin->get(my_c);
    } while (!fin->eof() && my_c != '"');
}

/**
 * Grammar for SubProgs
 * @return void
 */
void Parser::SubProgs()
{
    int n = 0;
    while (IsToken("function"))
    {
        Fcn();
        n++;
    }
    BuildTree("subprogs", n);
}

/**
 * Grammar for Term
 * @return void
 */
void Parser::Term()
{
    Factor();
    if (IsToken("+"))
    {
        ReadToken("+");
        Factor();
        BuildTree("+", 2);
    }
    else if (IsToken("-"))
    {
        ReadToken("-");
        Factor();
        BuildTree("-", 2);
    }
    else if (IsToken("or"))
    {
        ReadToken("or");
        Factor();
        BuildTree("or", 2);
    }
}

/**
 * Grammar for Type
 * @return void
 */
void Parser::Type()
{
    Name();
    ReadToken("=");
    LitList();
    BuildTree("type", 2);
}

/**
 * Grammar for Types
 * @return void
 */
void Parser::Types()
{
    int n = 0;
    if (IsToken("type"))
    {
        do
        {
            ReadToken("type");
            Type();
            n++;
            ReadWhitespace();
        } while (isIdentifierStart(my_c));
        ReadToken(";");
    }
    BuildTree("types", n);
}

/**
 * Grammar for Tiny
 * @return void
 */
void Parser::Tiny()
{
    ReadComment();
    ReadToken("program");
    Name();
    ReadToken(":");
    Consts();
    Types();
    Dclns();
    SubProgs();
    Body();
    Name();
    ReadToken(".");
    BuildTree("program", 7);
}

Parser::~Parser()
{
}
