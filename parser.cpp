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
}

void Parser::_ReadIdentifier() {
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
        fin->get(my_c);
    } while(my_c == ' ' || my_c == '\n' || my_c == '\t');
}

void Parser::_ReadToken(string token) {
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

void Parser::_Tiny() {
    _ReadComment();
    _ReadWhitespace();
    _ReadToken("program");
    _ReadWhitespace();
    _Name();
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
