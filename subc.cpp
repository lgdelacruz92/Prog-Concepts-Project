#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

void printAST(AST* ast, int n) {
    for (int i = 0; i < n; i++) {
        cout << ". ";
    }
    cout << ast->value << endl;
    if (ast->left) {
        printAST(ast->left, n+1);
    }
    if (ast->right) {
        printAST(ast->right, n);
    }
}

void testPrintAST() {
    AST root("a", nullptr, nullptr);
    root.left = new AST("b", nullptr, nullptr);
    root.left->right = new AST("c", nullptr, nullptr);
    root.left->right->left = new AST("e", nullptr, nullptr);
    root.left->right->left->right = new AST("f", nullptr, nullptr);
    root.left->right->left->right->right = new AST("g", nullptr, nullptr);
    root.left->right->right = new AST("d", nullptr, nullptr);
    printAST(&root, 0);
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