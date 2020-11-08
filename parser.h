#include <iostream>
#include <fstream>
#include <string>
#include <stack>
using namespace std;
static const int COMMENT = 1;
static const int IDENTIFIER = 2;
static const int INTEGER = 3;

class Parser {
    public:
        Parser();
        ~Parser();
        void ReadFile(string codeFile);
    private:
        int _ReadComment(string& comment);
        int _ReadToken(string& token);
        int _ReadIdentifier(string& identifier);
        int _ReadInteger(string& integer);
        ifstream* fin;
        char my_c;
        int numberLines;
        static const int T_COMMENT = 0;
};
