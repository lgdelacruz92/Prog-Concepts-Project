#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

void printAST(AST* ast, int n) {
    for (int i = 0; i < n; i++) {
        cout << ". ";
    }
    cout << ast->value << endl;
    if (ast->right) {
        while (ast->right != nullptr) {
            ast = ast->right;
            n++;
        }
        printAST(ast, n);
    } else if (ast->left) {
        printAST(ast->left, n+1);
    }
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
    return 0;
}