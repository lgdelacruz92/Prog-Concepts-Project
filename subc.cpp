#include <iostream>
#include <fstream>
#include "parser.h"
#include "printAST.h"


int main(int argc, char **argv) {
    ifstream fin;
    fin.open(argv[argc-1], ios::in);
    if (fin.fail()) {
        cout << "Error opening the file." << endl;
        fin.close();
        return -1;
    }
    Parser p(&fin);
    p.Tiny();
    printAST(p.ast_stack.top(), 0);
    fin.close();
    // testPrintAST();
    return 0;
}