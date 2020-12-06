#include <iostream>
#include <fstream>
#include "parser.h"
#include "printAST.h"

void testPrintAST() {
    AST* d = new AST("d(0)", nullptr, nullptr);
    AST* g = new AST("g(0)", nullptr, nullptr);
    AST* f = new AST("f(0)", nullptr, g);
    AST* e = new AST("e(0)", nullptr, f);
    AST* c = new AST("c(3)", e, d);
    AST* b = new AST("b(0)", nullptr, c);
    AST* a = new AST("a(3)", b, nullptr);
    printAST(a, 0);
}

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