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
 * Grammar for Body
 * @return void
 */
void Parser::_Body() {
    _ReadToken("begin");
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
 * Method that checks if next token is an identifier
 * @return bool
 */
bool Parser::_IsIdentifier() {
    cout << "Here we go again" << endl;
    return true;
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
 * Method that reads the source file
 * Initiates the parsing
 * @return void
 */
void Parser::ReadFile(string _codeFile) {
    ifstream* l_fin = static_cast<ifstream*>(fin);
    l_fin->open(_codeFile, ios::in);
    if (fin->fail()) {
        cout << "Error opening the file." << endl;
        l_fin->close();
        return;
    }
    _Tiny();
    l_fin->close();
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
 * Grammar for SubProgs
 * @return void
 */
void Parser::_SubProgs() {
    while (_IsToken("function")) {
        _Fcn();
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
    if (fin != nullptr) {
        cout << "Deallocating" << endl;
        delete fin;
    }
}
