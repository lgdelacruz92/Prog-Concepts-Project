#include "parser.h"
#include "utils.h"

Parser::Parser() {
    fin = new ifstream();
    numberLines = 0;
    predefined_tokens.insert("\n");
    predefined_tokens.insert("program");
    predefined_tokens.insert("var");
    predefined_tokens.insert("const");
    predefined_tokens.insert("type");
    predefined_tokens.insert("function");
    predefined_tokens.insert("return");
    predefined_tokens.insert("begin");
    predefined_tokens.insert("end");
    predefined_tokens.insert(":=:");
    predefined_tokens.insert(":=");
    predefined_tokens.insert("output");
    predefined_tokens.insert("if");
    predefined_tokens.insert("then");
    predefined_tokens.insert("else");
    predefined_tokens.insert("while");
    predefined_tokens.insert("do");
    predefined_tokens.insert("case");
    predefined_tokens.insert("of");
    predefined_tokens.insert("..");
    predefined_tokens.insert("otherwise");
    predefined_tokens.insert("repeat");
    predefined_tokens.insert("for");
    predefined_tokens.insert("until");
    predefined_tokens.insert("loop");
    predefined_tokens.insert("pool");
    predefined_tokens.insert("exit");
    predefined_tokens.insert("<=");
    predefined_tokens.insert("<>");
    predefined_tokens.insert("<");
    predefined_tokens.insert(">=");
    predefined_tokens.insert(">");
    predefined_tokens.insert("=");
    predefined_tokens.insert("mod");
    predefined_tokens.insert("and");
    predefined_tokens.insert("or");
    predefined_tokens.insert("not");
    predefined_tokens.insert("read");
    predefined_tokens.insert("succ");
    predefined_tokens.insert("pred");
    predefined_tokens.insert("chr");
    predefined_tokens.insert("ord");
    predefined_tokens.insert("eof");
    predefined_tokens.insert("{");
    predefined_tokens.insert(":");
    predefined_tokens.insert(";");
    predefined_tokens.insert(".");
    predefined_tokens.insert(",");
    predefined_tokens.insert("(");
    predefined_tokens.insert(")");
    predefined_tokens.insert("+");
    predefined_tokens.insert("-");
    predefined_tokens.insert("*");
    predefined_tokens.insert("/");
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
            if (type == IDENTIFIER) {
                if (predefined_tokens.find(token) != predefined_tokens.end()) {
                    cout << token << " ********* This a token *******" << endl;
                } else {
                    cout << token << endl;
                }
            } else {
                cout << token << endl;
            }
        }
    }
    fin->close();
}

Parser::~Parser() {
    delete fin;
}
