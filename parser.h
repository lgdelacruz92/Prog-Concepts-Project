#include <iostream>
#include <fstream>
#include <string>
#include <stack>
using namespace std;

class Parser {
    public:
        Parser();
        ~Parser();
        void ReadFile(string codeFile);
    private:
        void _ReadComment();
        ifstream* fin;
        char my_c;
        int numberLines;
        static const int T_COMMENT = 0;
};