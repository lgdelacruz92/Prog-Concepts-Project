#include <iostream>
#include <sstream>
#include "../parser.h"
#include "./test_utils.h"
using namespace std;

class ParserStatementTests: public Parser {
public:
	ParserStatementTests(istream* _fin) {
		fin = _fin;
		line = 0;
	}
};

void tests_statement_1() {
	string program = "begin\n"
                     "  read(i)\n"
                     "end\000";
    istringstream iss(program);
    ParserStatementTests p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    try {
        p._Statement();
        assertEqual(true, true, "Success Statement should not throw an error", "");
    } catch(int e) {
        assertEqual(true, false, "", "Error, Statement threw an error");
    }
}

int main() {
	tests_statement_1();
	return 0;
}