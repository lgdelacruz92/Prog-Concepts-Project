#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <set>

using namespace std;

class Parser {
    public:
        Parser();
        ~Parser();
        void ReadFile(string codeFile);
    private:
        void _Name();
        void _ReadComment();
        void _ReadIdentifier();
        void _ReadWhitespace();
        void _ReadToken(string token);
        void _Tiny();
        ifstream* fin;
        char my_c;
        int line;
};
