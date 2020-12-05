#pragma once
#include <iostream>
#include "ast.h"
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