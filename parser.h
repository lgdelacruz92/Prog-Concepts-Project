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
        bool _Dcln();
        void _Dclns();
        void _Fcn();
        bool _IsToken(string token);
        void _LitList();
        void _Name();
        void _Params();
        void _ReadComment();
        void _ReadIdentifier();
        void _ReadWhitespace();
        void _ReadToken(string token);
        void _SubProgs();
        void _Tiny();
        void _Type();
        void _Types();
        ifstream* fin;
        char my_c;
        int line;
};
        