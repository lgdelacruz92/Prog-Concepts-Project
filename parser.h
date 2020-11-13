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
        void _Const();
        void _Consts();
        bool _IsToken(string token);
        void _Name();
        void _ReadComment();
        void _ReadIdentifier();
        void _ReadWhitespace();
        void _ReadToken(string token);
        void _Tiny();
        void _Types();
        ifstream* fin;
        char my_c;
        int line;
};
