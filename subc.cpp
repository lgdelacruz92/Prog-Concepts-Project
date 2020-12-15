#include <iostream>
#include <fstream>
#include "parser.h"
#include "printAST.h"

// void testPrintAST() {
//     AST* d = new AST("d(0)", nullptr, nullptr);
//     AST* g = new AST("g(0)", nullptr, nullptr);
//     AST* f = new AST("f(0)", nullptr, g);
//     AST* e = new AST("e(0)", nullptr, f);
//     AST* c = new AST("c(3)", e, d);
//     AST* b = new AST("b(0)", nullptr, c);
//     AST* a = new AST("a(3)", b, nullptr);
//     printAST(a, 0);
// }

void file_error(int argc, char **argv)
{
    if (strcmp(argv[argc - 1], "-ast") == 0)
    {
        cout << "File name should be last argument, not '-ast'." << endl;
    }
    else
    {
        cout << "No such file '" << argv[argc - 1] << "'" << endl;
    }
}


int main(int argc, char **argv)
{
    if (argc == 1)
    {
        cout << "No file specified." << endl;
        return -1;
    }
    ifstream fin;
    fin.open(argv[argc - 1], ios::in);
    if (fin.fail())
    {
        file_error(argc, argv);
        fin.close();
        return -1;
    }
    Parser p(&fin);
    p.Tiny();
    int ast_count = 0;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-ast") != 0) {
            cout << "Unrecognized argument " << argv[i] << endl;
            return -1;
        } else {
            ast_count++;
        }
    }

    if (ast_count == 1) {
        printAST(p.ast_stack.top(), 0);
    } else if (ast_count > 1) {
        cout << "There is more than one -ast paramater." << endl;
        printAST(p.ast_stack.top(), 0);
    }
    fin.close();
    return 0;
}