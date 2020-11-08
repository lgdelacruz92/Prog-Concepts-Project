#include "parser.h"
#include "utils.h"

Parser::Parser() {
    fin = new ifstream();
    numberLines = 0;
}

int Parser::_ReadComment(string& comment) {
    int openBraceCount = 0;
    do {
        if (my_c == '{') {
            openBraceCount++;
        } else if (my_c == '}') {
            openBraceCount--;
        }
        comment += my_c;
        fin->get(my_c);
    } while (openBraceCount > 0);
    return COMMENT;
}

int Parser::_ReadIdentifier(string& identifier) {
    do {
        identifier += my_c;
        fin->get(my_c);
    } while (isIdentifierCharacter(my_c));
    return IDENTIFIER;
}

int Parser::_ReadInteger(string& integer) {
    do {
        integer += my_c;
        fin->get(my_c);
    } while (isDigit(my_c));
    return INTEGER;
}

int Parser::_ReadToken(string& token) {
    fin->get(my_c);
    if (my_c == '{') {
        return _ReadComment(token);
    } else if (isIdentifierStart(my_c)) {
        return _ReadIdentifier(token);
    } else if (isDigit(my_c)) {
        return _ReadInteger(token);
    }
    return -1;
}

void Parser::ReadFile(string _codeFile) {
    fin->open(_codeFile, ios::in);
    if (fin->fail()) {
        cout << "Error opening the file." << endl;
        fin->close();
        return;
    }

    while (!fin->eof()) {
        string token;
        int type = _ReadToken(token);
        if (token != "") {
            cout << token << endl;
        }
    }
    fin->close();
}

Parser::~Parser() {
    delete fin;
}
