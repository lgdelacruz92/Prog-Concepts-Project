#include <fstream>
#include "parser.h"
#include "utils.h"

Parser::Parser() {
    fin = new ifstream();
    line = 0;
}

Parser::Parser(istream* _fin) {
    fin = _fin;
    line = 0;
}

/**
 * Grammar for Assignment
 * @return void
 */
void Parser::Assignment() {
    ReadIdentifier();
    if (IsToken(":=")) {
        ReadToken(":=");
        Expression();
    }
    else if (IsToken(":=:")) {
        ReadToken(":=:");
        Name();
    }
}

/**
 * Grammar for Body
 * @return void
 */
void Parser::Body() {
    ReadToken("begin");
    do {
        if (IsToken(";")) {
            ReadToken(";");
        }
        Statement();
    } while (IsToken(";"));
    ReadToken("end");
}

/**
 * Method for BuildTree
 * @param string node
 * @param int num_pop
 * @return void
 */
void Parser::BuildTree(string node, int num_pop) {
    AST* p = nullptr;
    for (int i = 0; i < num_pop; i++) {
        AST* c = ast_stack.top();
        ast_stack.pop();
        c->right = p;
        p = c;
    }
    AST* new_node = new AST(node, p, nullptr);
    ast_stack.push(new_node);
}

/**
 * Gramnmar for Caseclause
 * @return void
 */
void Parser::Caseclause() {
    do {
        CaseExpression();
    } while (IsToken(","));
    ReadToken(";");
    Statement();
}

/**
 * Grammar for Caseclauses
 * @return void
 */
void Parser::Caseclauses() {
    do {
        Caseclause();
    } while (IsToken(";"));
}

/**
 * Grammar for CaseExpression
 * @return void
 */
void Parser::CaseExpression() {
    ConstValue();
    if (IsToken("..")) {
        ConstValue();
    }
}

/**
 * Grammar for Const
 * @return void
 */
void Parser::Const() {
    Name();
    ReadToken("=");
}

/**
 * Grammar for Consts
 * @return void
 */
void Parser::Consts() {
    if (IsToken("const")) {
        ReadToken("const");
        Const();
    }
}

/**
 * Grammar for CosntValue
 * @return void
 */
void Parser::ConstValue() {
    if (IsInteger()) {
        ReadInteger();
    } else if (IsChar()) {
        ReadChar();
    } else if (IsIdentifier()) {
        ReadIdentifier();
    } else {
        string message = "Nont a valid ConstValue on " + to_string(line);
        throw message;
    }
}

/**
 * Grammar for Dcln
 * @return void
 */
bool Parser::Dcln() {
    int original_pos = fin->tellg();
    do {
        if (IsToken(",")) {
            ReadToken(",");
        }
        Name();
    } while (IsToken(","));
    if (IsToken(":")) {
        ReadToken(":");
        ReadIdentifier();
        return true;
    } else {
        fin->seekg(original_pos-1);
        fin->get(my_c);
        return false;
    }
}

/**
 * Grammar for Dclns
 * @return void
 */
void Parser::Dclns() {
    if (IsToken("var")) {
        ReadToken("var");
        while (Dcln()) {
            ReadToken(";");
        }
    }
}

/**
 * Grammar for Expression
 * @return void
 */
void Parser::Expression() {
    Term();
    if (IsToken("<=")) {
        ReadToken("<=");
        Term();
    } else if(IsToken("<")) {
        ReadToken("<");
        Term();
    } else if (IsToken(">=")) {
        ReadToken(">=");
        Term();
    } else if(IsToken(">")) {
        ReadToken(">");
        Term();
    } else if (IsToken("=")) {
        ReadToken("=");
        Term();
    } else if(IsToken("<>")) {
        ReadToken("<>");
        Term();
    }
}

/**
 * Grammar for Factor
 * @return void
 */
void Parser::Factor() {
    Primary();
    while (IsToken("*") ||
           IsToken("/") ||
           IsToken("and") ||
           IsToken("mod")) {
        if (IsToken("*")) {
            ReadToken("*");
        } else if (IsToken("/")) {
            ReadToken("/");
        } else if (IsToken("and")) {
            ReadToken("and");
        } else if (IsToken("mod")) {
            ReadToken("mod");
        }
        Primary();
    }
}

/**
 * Grammar for Fcn
 * @return void
 */
void Parser::Fcn() {
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
}

/**
 * Grammar for ForStat
 * @return void
 */
void Parser::ForStat() {
    if (IsIdentifier()) {
        Assignment();
    }
}

/**
 * Grammar for ForExp
 * @return void
 */
void Parser::ForExp() {
    if (IsExpression()) {
        Expression();
    }
}

/**
 * Method that checks if the next token is char
 * @return bool
 */
bool Parser::IsChar() {
    int original_position = fin->tellg();
    ReadWhitespace();
    if (my_c == '\'') {
        fin->get(my_c);
        fin->get(my_c);
        if (my_c == '\'') {
            fin->get(my_c);
        } else {
            return false;
        }

        fin->seekg(original_position-1);
        fin->get(my_c);
        return true;
    } else {
        return false;
    }
}

/**
 * Method that checks if the next token is an expression
 * @return bool
 */
bool Parser::IsExpression() {
    return IsTerm();
}

/**
 * Method that checks if next token is a factor
 * @return bool
 */
bool Parser::IsFactor() {
    return IsPrimary();
}

/**
 * Method that checks if next token is an identifier
 * @return bool
 */
bool Parser::IsIdentifier() {
    int original_position = fin->tellg();
    ReadWhitespace();

    if (isIdentifierStart(my_c)) {
        while(!isWhiteSpace(my_c) && !(fin->eof())) {
            if (!isIdentifierCharacter(my_c)) {
                break;
            }
            fin->get(my_c);
        }
        fin->seekg(original_position-1);
        fin->get(my_c);
        return true;
    } else {
        fin->seekg(original_position-1);
        fin->get(my_c);
        return false;
    }
}

/**
 * Method that checks if the next token is a Primary
 * @return bool
 */
bool Parser::IsPrimary() {
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
 * Method that checks if the next token is a Term
 * @return bool
 */
bool Parser::IsTerm() {
    return IsFactor();
}

/**
 * Method that checks if the following characters make an integer
 * @return bool
 */
bool Parser::IsInteger() {
    int original_position = fin->tellg();
    ReadWhitespace();
    string val = "";
    if (0 <= my_c - '0' && my_c - '0' <= 9) {
        while (!fin->eof() && 0 <= my_c - '0' && my_c - '0' <= 9 && my_c != '\n' && my_c != ' ') {
            val += my_c;
            fin->get(my_c);
        }

        fin->clear();
        fin->seekg(original_position-1);
        fin->get(my_c);
        return true;
    } else {
        fin->clear();
        fin->seekg(original_position-1);
        fin->get(my_c);
        return false;
    }

}

/**
 * Method that checks if the following characters is
 * a token
 * @return bool
 */
bool Parser::IsToken(string token) {
    int original_pos = fin->tellg();
    ReadWhitespace();
    bool is_token = true;
    for (int i = 0; i < token.size(); i++) {
        if (token[i] != my_c) {
            is_token = false;
            break;
        }
        fin->get(my_c);
    }
    fin->seekg(original_pos-1);
    fin->get(my_c);

    return is_token;
}

/**
 * Grammar for LitList
 * @return void
 */
void Parser::LitList() {
    ReadToken("(");
    ReadWhitespace();
    while (!IsToken(")")) {
        Name();
        ReadWhitespace();
        if (IsToken(")")) {
            break;
        } else if (IsToken(",")) {
            ReadToken(",");
        }
        ReadWhitespace();
    }
    ReadToken(")");
}

/**
 * Grammar for Name
 * @return void
 */
void Parser::Name() {
    ReadIdentifier();
}

/**
 * Grammar for OtherwiseClause
 * @return void
 */
void Parser::OtherwiseClause() {
    if (IsToken("otherwise")) {
        ReadToken("otherwise");
        Statement();
    } else {
        ReadToken(";");
    }
}

/**
 * Grammar for out expression
 * @return void
 */
void Parser::OutExp() {
    if (IsExpression()) {
        Expression();
    } else {
        StringNode();
    }
}

/**
 * Grammar for Params
 * @return void
 */
void Parser::Params() {
    while(!IsToken(")")) {
        Dcln();
        if (IsToken(";")) {
            ReadToken(";");
        }
    }
}

/**
 * Grammar for Primary
 * @return void
 */
void Parser::Primary() {
    if (IsToken("-")) {
        ReadToken("-");
        Primary();
    } else if (IsToken("+")) {
        ReadToken("+");
        Primary();
    } else if (IsToken("not")) {
        ReadToken("not");
        Primary();
    } else if (IsToken("eof")) {
        ReadToken("eof");
    } else if (IsIdentifier()) {
        ReadIdentifier();
        if (IsToken("(")) {
            ReadToken("(");
            Expression();
            while (IsToken(",")) {
                ReadToken(",");
                Expression();
            }
            ReadToken(")");
        }
    } else if (IsInteger()) {
        ReadInteger();
    } else if (IsChar()) {
        ReadChar(); 
    }
}

/**
 * Method that reads a char
 * @return void
 */
void Parser::ReadChar() {
    ReadWhitespace();
    ReadToken("\'");
    fin->get(my_c);
    ReadToken("\'");
}

/**
 * Method that iterates through the comment characters
 * @return void
 */
void Parser::ReadComment() {
    // Keep reading charaters until the brackets count
    // goes to zero
    // '{' is +1, '}' is -1
    int openBrackets = 0;
    do {
        if (my_c == '\n') {
            line++;
        }
        fin->get(my_c);
        if (my_c == '{') {
            openBrackets++;
        } else if (my_c == '}') {
            openBrackets--;
        } else if (isIdentifierCharacter(my_c) && openBrackets == 0) {
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
void Parser::ReadIdentifier() {
    // First skip through all whitespace
    ReadWhitespace();

    // Is it a valid identifier start char
    if (isIdentifierStart(my_c)) {
        string identifier;
        // As long as it's valid identifier character
        while(isIdentifierCharacter(my_c)) {
            identifier += my_c;
            fin->get(my_c);
        }
        
        cout << identifier << endl;
    } else {
        cout << "Invalid identifier at line " << line + 1 << endl;
        throw "";
    }
}

/**
 * Method that reads an integer
 * @return void
 */
void Parser::ReadInteger() {
    ReadWhitespace();
    string val = "";
    while (!fin->eof() && 0 <= my_c - '0' && my_c - '0' <= 9) {
        val += my_c;
        fin->get(my_c);
    }
}

/**
 * Method that reads the specified token
 * @return void
 */
void Parser::ReadToken(string token) {
    // First skip all whitespace
    ReadWhitespace();

    // If there is a single character that mismatch
    // throw error
    for (int i = 0; i < token.size(); i++) {
        if (my_c != token[i]) {
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
void Parser::ReadWhitespace() {
    // While there is a whitespace move to the next char
    do {
        // Counting the line is useful for error logs
        if (my_c == '\n') {
            line++;
        }

        if (!isWhiteSpace(my_c)) {
            break;
        }

        fin->get(my_c);
    } while(isWhiteSpace(my_c));
}

/**
 * Grammar for statement
 * @return void
 */
void Parser::Statement() {
    if (IsToken("output")) {
        ReadToken("output");
        ReadToken("(");
        do {
            OutExp();
        } while(IsToken(","));
        ReadToken(")");
    }
    else if (IsToken("if")) {
        ReadToken("if");
        Expression();
        ReadToken("then");
        Statement();
        if (IsToken("else")) {
            Statement();
        }
    }
    else if (IsToken("while")) {
        ReadToken("while");
        Expression();
        ReadToken("do");
        Statement();
    }
    else if (IsToken("repeat")) {
        ReadToken("repeat");
        do {
            if (IsToken(";")) {
                ReadToken(";");
            }
            Statement();
        } while (IsToken(";"));
        ReadToken("until");
        Expression();

    }
    else if (IsToken("for")) {
        ReadToken("for");
        ReadToken("(");
        ForStat();
        ReadToken(";");
        ForExp();
        ReadToken(";");
        ForStat();
        ReadToken(")");
        Statement();
    }
    else if (IsToken("loop")) {
        ReadToken("loop");
        do {
            Statement();
        } while (IsToken(";"));
        ReadToken("pool");
    }
    else if (IsToken("case")) {
        ReadToken("case");
        Expression();
        ReadToken("of");
        Caseclauses();
        OtherwiseClause();
        ReadToken("end");
    }
    else if (IsToken("read")) {
        ReadToken("read");
        ReadToken("(");
        do {
            Name();
        } while (IsToken(","));
        ReadToken(")");
    }
    else if (IsToken("return")) {
        ReadToken("return");
        Expression();
    }
    else if (IsToken("begin")) {
        ReadToken("begin");
        Body();
    }
    else if (IsIdentifier()) {
        Assignment();
    }
}

/**
 * Grammar for StringNode
 * @return void
 */
void Parser::StringNode() {
    ReadWhitespace();
    ReadToken("\"");
    do {
        fin->get(my_c);
    } while(!fin->eof() && my_c != '"');
}

/**
 * Grammar for SubProgs
 * @return void
 */
void Parser::SubProgs() {
    while (IsToken("function")) {
        Fcn();
    }
}

/**
 * Grammar for Term
 * @return void
 */
void Parser::Term() {
    Factor();
    while (IsToken("+") ||
           IsToken("-") ||
           IsToken("or")) {

        if (IsToken("+")) {
            ReadToken("+");
        } else if (IsToken("-")) {
            ReadToken("-");
        } else if (IsToken("or")) {
            ReadToken("or");
        }
        Factor();
    }
}

/**
 * Grammar for Type
 * @return void
 */
void Parser::Type() {
    Name();
    ReadToken("=");
    LitList();
}

/**
 * Grammar for Types
 * @return void
 */
void Parser::Types() {
    if (IsToken("type")) {
        do {
            ReadToken("type");
            Type();
            ReadWhitespace();
        } while (isIdentifierStart(my_c));
        ReadToken(";");
    }
}

/**
 * Grammar for Tiny
 * @return void
 */
void Parser::Tiny() {
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
}

Parser::~Parser() {
}
