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
        virtual void Assignment();
        virtual void Body();
        virtual void Caseclause();
        virtual void Caseclauses();
        virtual void CaseExpression();
        virtual void Const();
        virtual void Consts();
        virtual void ConstValue();
        virtual bool Dcln();
        virtual void Dclns();
        virtual void Expression();
        virtual void Factor();
        virtual void ForExp();
        virtual void ForStat();
        virtual void Fcn();
        virtual bool IsChar();
        virtual bool IsExpression();
        virtual bool IsFactor();
        virtual bool IsIdentifier();
        virtual bool IsInteger();
        virtual bool IsPrimary();
        virtual bool IsTerm();
        virtual bool IsToken(string token);
        virtual void LitList();
        virtual void Name();
        virtual void OutExp();
        virtual void OtherwiseClause();
        virtual void Params();
        virtual void Primary();
        virtual void ReadChar();
        virtual void ReadComment();
        virtual void ReadIdentifier();
        virtual void ReadInteger();
        virtual void ReadWhitespace();
        virtual void ReadToken(string token);
        virtual void Statement();
        virtual void StringNode();
        virtual void SubProgs();
        virtual void Term();
        virtual void Tiny();
        virtual void Type();
        virtual void Types();
        istream* fin;
        char my_c;
        int line;
};
        