#include "ast.h";

AST::AST(string _value, AST* _left, AST* _right) {
    value = _value;
    left = _left;
    right = _right;
}