#include "parser.h"

Parser::Parser() {
    fin = new ifstream();
    numberLines = 0;
}

void Parser::_ReadComment() {
    int openBraceCount = 0;
    do {
        if (my_c == '{') {
            openBraceCount++;
        } else if (my_c == '}') {
            openBraceCount--;
        }
        cout << my_c;
        fin->get(my_c);
    } while (openBraceCount > 0);
    cout << endl;
}

void Parser::ReadFile(string _codeFile) {
    fin->open(_codeFile, ios::in);
    if (fin->fail()) {
        cout << "Error opening the file." << endl;
        fin->close();
        return;
    }

    while (!fin->eof()) {
        fin->get(my_c);
        switch(my_c) {
            case '{':
                _ReadComment();
                break;
        }

        if (my_c == '\n') {
            ++numberLines;
        }
    }
    fin->close();
}

Parser::~Parser() {
    delete fin;
}