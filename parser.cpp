#include "parser.h"
#include "utils.h"

Parser::Parser() {
    fin = new ifstream();
    line = 0;
}

void Parser::_ReadComment() {
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
        }
    } while (openBrackets > 0);
    fin->get(my_c);
}

void Parser::_ReadIdentifier() {
    _ReadWhitespace();
    if (isIdentifierStart(my_c)) {
        string identifier;
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

void Parser::_ReadWhitespace() {
    do {
        if (my_c == '\n') {
            line++;
        }
        if (my_c != ' ' && my_c != '\n' && my_c != '\t') {
            break;
        }
        fin->get(my_c);
    } while(my_c == ' ' || my_c == '\n' || my_c == '\t');
}

void Parser::_ReadToken(string token) {
    _ReadWhitespace();
    string theToken;
    for (int i = 0; i < token.size(); i++) {
        theToken += my_c;
        fin->get(my_c);
    }
    if (theToken != token) {
        cout << "Invalid token: " << theToken << " at line " << line << endl;
        throw "";
    }
}

void Parser::_Name() {
    _ReadIdentifier();
}

void Parser::_Const() {
    _Name();
    _ReadToken("=");
}

void Parser::_Consts() {
    if (_IsToken("const")) {
        _ReadToken("const");
        _Const();
    }
}

bool Parser::_IsToken(string token) {
    _ReadWhitespace();
    for (int i = 0; i < token.size(); i++) {
        if (token[i] != my_c)
            return false;
        fin->get(my_c);
    }
    return true;
}

void Parser::_Tiny() {
    _ReadComment();
    _ReadToken("program");
    _Name();
    _ReadToken(":");
    _Consts();
}

void Parser::ReadFile(string _codeFile) {
    fin->open(_codeFile, ios::in);
    if (fin->fail()) {
        cout << "Error opening the file." << endl;
        fin->close();
        return;
    }
    _Tiny();
    fin->close();
}

Parser::~Parser() {
    delete fin;
}
