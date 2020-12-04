#include <string>
using namespace std;

class AST {
    public:
        AST(string _value, AST* _left, AST* _right);
        AST* left;
        AST* right;
        string value;
};