#include "parser.h"
#include "utils.h"

Parser::Parser() {
    fin = new ifstream();
    line = 0;
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
 * Grammar for Name
 * @return void
 */
void Parser::_Name() {
    _ReadIdentifier();
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
    fin->open(_codeFile, ios::in);
    if (fin->fail()) {
        cout << "Error opening the file." << endl;
        fin->close();
        return;
    }
    _Tiny();
    fin->close();
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

        // If the character is not alphanumeric and not whitespace
        // throw and error
        if (!isAlphabet(my_c) && !isDigit(my_c) && !isWhiteSpace(my_c)) {
            cout << "Invalid identifier at line" << endl;
            throw "";
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
            throw "";
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

        if (isWhiteSpace(my_c)) {
            break;
        }

        fin->get(my_c);
    } while(isWhiteSpace(my_c));
}

/**
 * Method that checks if the following characters is
 * a token
 * @return bool
 */
bool Parser::_IsToken(string token) {
    _ReadWhitespace();
    for (int i = 0; i < token.size(); i++) {
        if (token[i] != my_c)
            return false;
        fin->get(my_c);
    }
    return true;
}

void Parser::_Types() {

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
}

Parser::~Parser() {
    delete fin;
}
