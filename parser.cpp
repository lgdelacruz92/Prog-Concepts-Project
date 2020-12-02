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
void Parser::_Assignment() {
    _ReadIdentifier();
    if (_IsToken(":=")) {
        _ReadToken(":=");
        _Expression();
    }
    else if (_IsToken(":=:")) {
        _ReadToken(":=:");
        _Name();
    }
}

/**
 * Grammar for Body
 * @return void
 */
void Parser::_Body() {
    _ReadToken("begin");
    _Statement();
    _ReadToken("end");
}

/**
 * Gramnmar for Caseclause
 * @return void
 */
void Parser::_Caseclause() {
    
}

/**
 * Grammar for Caseclauses
 * @return void
 */
void Parser::_Caseclauses() {
    do {
        _Caseclause();
    } while (_IsToken(";"));
}

/**
 * Grammar for Const
 * @return void
 */
void Parser::_Const() {
    _Name();
    _ReadToken("=");
}

/**
 * Grammar for Consts
 * @return void
 */
void Parser::_Consts() {
    if (_IsToken("const")) {
        _ReadToken("const");
        _Const();
    }
}

/**
 * Grammar for Dcln
 * @return void
 */
bool Parser::_Dcln() {
    int original_pos = fin->tellg();
    do {
        if (_IsToken(",")) {
            _ReadToken(",");
        }
        _Name();
    } while (_IsToken(","));
    if (_IsToken(":")) {
        _ReadToken(":");
        _ReadIdentifier();
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
void Parser::_Dclns() {
    if (_IsToken("var")) {
        _ReadToken("var");
        while (_Dcln()) {
            _ReadToken(";");
        }
    }
}

/**
 * Grammar for Expression
 * @return void
 */
void Parser::_Expression() {
    _Term();
    if (_IsToken("<=") ||
        _IsToken("<") ||
        _IsToken(">=") ||
        _IsToken(">") ||
        _IsToken("=") ||
        _IsToken("<>")) {
        _Term();
    }
}

/**
 * Grammar for Factor
 * @return void
 */
void Parser::_Factor() {
    _Primary();
    while (_IsToken("*") ||
           _IsToken("/") ||
           _IsToken("and") ||
           _IsToken("mod")) {
        if (_IsToken("*")) {
            _ReadToken("*");
        } else if (_IsToken("/")) {
            _ReadToken("/");
        } else if (_IsToken("and")) {
            _ReadToken("and");
        } else if (_IsToken("mod")) {
            _ReadToken("mod");
        }
        _Primary();
    }
}

/**
 * Grammar for Fcn
 * @return void
 */
void Parser::_Fcn() {
    _ReadToken("function");
    _Name();
    _ReadToken("(");
    _Params();
    _ReadToken(")");
    _ReadToken(":");
    _Name();
    _ReadToken(";");
    _Consts();
    _Types();
    _Dclns();
    _Body();
}

/**
 * Grammar for ForStat
 * @return void
 */
void Parser::_ForStat() {
    if (_IsIdentifier()) {
        _Assignment();
    }
}

/**
 * Grammar for ForExp
 * @return void
 */
void Parser::_ForExp() {
    if (_IsExpression()) {
        _Expression();
    }
}

/**
 * Method that checks if the next token is char
 * @return bool
 */
bool Parser::_IsChar() {
    int original_position = fin->tellg();
    _ReadWhitespace();
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
bool Parser::_IsExpression() {
    return _IsTerm();
}

/**
 * Method that checks if next token is a factor
 * @return bool
 */
bool Parser::_IsFactor() {
    return _IsPrimary();
}

/**
 * Method that checks if next token is an identifier
 * @return bool
 */
bool Parser::_IsIdentifier() {
    int original_position = fin->tellg();
    _ReadWhitespace();

    if (isIdentifierStart(my_c)) {
        while(!isWhiteSpace(my_c) && !(fin->eof())) {
            if (!isIdentifierCharacter(my_c)) {
                return false;
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
bool Parser::_IsPrimary() {
    return _IsToken("-") ||
           _IsToken("+") ||
           _IsToken("not") ||
           _IsToken("eof") ||
           _IsIdentifier() ||
           _IsInteger() ||
           _IsChar() ||
           _IsToken("(") ||
           _IsToken("succ") ||
           _IsToken("pred") ||
           _IsToken("chr") ||
           _IsToken("ord");
}

/**
 * Method that checks if the next token is a Term
 * @return bool
 */
bool Parser::_IsTerm() {
    return _IsFactor();
}

/**
 * Method that checks if the following characters make an integer
 * @return bool
 */
bool Parser::_IsInteger() {
    int original_position = fin->tellg();
    _ReadWhitespace();
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
bool Parser::_IsToken(string token) {
    int original_pos = fin->tellg();
    _ReadWhitespace();
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
void Parser::_LitList() {
    _ReadToken("(");
    _ReadWhitespace();
    while (!_IsToken(")")) {
        _Name();
        _ReadWhitespace();
        if (_IsToken(")")) {
            break;
        } else if (_IsToken(",")) {
            _ReadToken(",");
        }
        _ReadWhitespace();
    }
    _ReadToken(")");
}

/**
 * Grammar for Name
 * @return void
 */
void Parser::_Name() {
    _ReadIdentifier();
}

/**
 * Grammar for out expression
 * @return void
 */
void Parser::_OutExp() {
    if (_IsInteger()) {
        _Expression();
    } else {
        _StringNode();
    }
}

/**
 * Grammar for Params
 * @return void
 */
void Parser::_Params() {
    while(!_IsToken(")")) {
        _Dcln();
        if (_IsToken(";")) {
            _ReadToken(";");
        }
    }
}

/**
 * Grammar for Primary
 * @return void
 */
void Parser::_Primary() {
    if (_IsToken("-")) {
        _ReadToken("-");
        _Primary();
    } else if (_IsToken("+")) {
        _ReadToken("+");
        _Primary();
    } else if (_IsToken("not")) {
        _ReadToken("not");
        _Primary();
    } else if (_IsToken("eof")) {
        _ReadToken("eof");
    } else if (_IsIdentifier()) {
        _ReadIdentifier();
        if (_IsToken("(")) {
            _ReadToken("(");
            _Expression();
            while (_IsToken(",")) {
                _ReadToken(",");
                _Expression();
            }
            _ReadToken(")");
        }
    } else if (_IsInteger()) {
        _ReadInteger();
    } else if (_IsChar()) {
        _ReadChar(); 
    }
}

/**
 * Method that reads a char
 * @return void
 */
void Parser::_ReadChar() {
    _ReadWhitespace();
    _ReadToken("\'");
    fin->get(my_c);
    _ReadToken("\'");
}

/**
 * Method that iterates through the comment characters
 * @return void
 */
void Parser::_ReadComment() {
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
void Parser::_ReadIdentifier() {
    // First skip through all whitespace
    _ReadWhitespace();

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
void Parser::_ReadInteger() {
    _ReadWhitespace();
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
void Parser::_ReadToken(string token) {
    // First skip all whitespace
    _ReadWhitespace();

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
void Parser::_ReadWhitespace() {
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
void Parser::_Statement() {
    if (_IsIdentifier()) {
        _Assignment();
    }
    else if (_IsToken("output")) {
        _ReadToken("output");
        _ReadToken("(");
        do {
            _OutExp();
        } while(_IsToken(","));
        _ReadToken(")");
    }
    else if (_IsToken("if")) {
        _ReadToken("if");
        _Expression();
        _ReadToken("then");
        _Statement();
        if (_IsToken("else")) {
            _Statement();
        }
    }
    else if (_IsToken("while")) {
        _ReadToken("while");
        _Expression();
        _ReadToken("do");
        _Statement();
    }
    else if (_IsToken("repeat")) {
        do {
            _Statement();
        } while (_IsToken(";"));
        _ReadToken("until");
        _Expression();

    }
    else if (_IsToken("for")) {
        _ReadToken("for");
        _ReadToken("(");
        _ForStat();
        _ReadToken(";");
        _ForExp();
        _ReadToken(";");
        _ForStat();
        _ReadToken(")");
        _Statement();
    }
    else if (_IsToken("loop")) {
        _ReadToken("loop");
        do {
            _Statement();
        } while (_IsToken(";"));
        _ReadToken("pool");
    }
    else if (_IsToken("case")) {
        _ReadToken("case");
        _Expression();
        _ReadToken("of");
        _Caseclauses();
        _OtherwiseClause();
        _ReadToken("end");
    }

    _ReadToken(";");
    _ReadToken("end");
}

/**
 * Grammar for StringNode
 * @return void
 */
void Parser::_StringNode() {
    _ReadWhitespace();
    _ReadToken("\"");
    do {
        fin->get(my_c);
    } while(!fin->eof() && my_c != '"');
}

/**
 * Grammar for SubProgs
 * @return void
 */
void Parser::_SubProgs() {
    while (_IsToken("function")) {
        _Fcn();
    }
}

/**
 * Grammar for Term
 * @return void
 */
void Parser::_Term() {
    _Factor();
    while (_IsToken("+") ||
           _IsToken("-") ||
           _IsToken("or")) {

        if (_IsToken("+")) {
            _ReadToken("+");
        } else if (_IsToken("-")) {
            _ReadToken("-");
        } else if (_IsToken("or")) {
            _ReadToken("or");
        }
        _Factor();
    }
}

/**
 * Grammar for Type
 * @return void
 */
void Parser::_Type() {
    _Name();
    _ReadToken("=");
    _LitList();
}

/**
 * Grammar for Types
 * @return void
 */
void Parser::_Types() {
    if (_IsToken("type")) {
        do {
            _ReadToken("type");
            _Type();
            _ReadWhitespace();
        } while (isIdentifierStart(my_c));
        _ReadToken(";");
    }
}

/**
 * Grammar for Tiny
 * @return void
 */
void Parser::_Tiny() {
    _ReadComment();
    _ReadToken("program");
    _Name();
    _ReadToken(":");
    _Consts();
    _Types();
    _Dclns();
    _SubProgs();
}

Parser::~Parser() {
}
