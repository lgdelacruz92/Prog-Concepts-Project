#include <iostream>
#include <istream>
#include <string>
#include <stack>
#include <set>

using namespace std;

class Parser {
    public:
        Parser();
        Parser(istream* _fin);
        virtual ~Parser();
        virtual void _Assignment();
        virtual void _Body();
        virtual void _Const();
        virtual void _Consts();
        virtual bool _Dcln();
        virtual void _Dclns();
        virtual void _Expression();
        virtual void _Factor();
        virtual void _Fcn();
        virtual bool _IsChar();
        virtual bool _IsIdentifier();
        virtual bool _IsInteger();
        virtual bool _IsToken(string token);
        virtual void _LitList();
        virtual void _Name();
        virtual void _OutExp();
        virtual void _Params();
        virtual void _Primary();
        virtual void _ReadChar();
        virtual void _ReadComment();
        virtual void _ReadIdentifier();
        virtual void _ReadInteger();
        virtual void _ReadWhitespace();
        virtual void _ReadToken(string token);
        virtual void _Statement();
        virtual void _StringNode();
        virtual void _SubProgs();
        virtual void _Term();
        virtual void _Tiny();
        virtual void _Type();
        virtual void _Types();
        istream* fin;
        char my_c;
        int line;
};
        