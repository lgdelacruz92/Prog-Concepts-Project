#include <iostream>
#include <sstream>
#include "../parser.h"
#include "./test_utils.h"
using namespace std;

class ParserAssignmentTests: public Parser {
public:
	ParserAssignmentTests(istream* _fin) {
		fin = _fin;
		line = 0;
	}
};

void test_assignment_1() {
	string program = "a := 2";
    istringstream iss(program);
    ParserAssignmentTests p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    try {
        p._Assignment();
        assertEqual(true, true, "Success Statement should not throw an error", "");
    } catch(int e) {
        assertEqual(true, false, "", "Error, Statement threw an error");
    }
}

void test_assignment_2() {
	string program = "a :=: b";
    istringstream iss(program);
    ParserAssignmentTests p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    try {
        p._Assignment();
        assertEqual(true, true, "Success Statement should not throw an error", "");
    } catch(int e) {
        assertEqual(true, false, "", "Error, Statement threw an error");
    }	
}

int main() {
	test_assignment_1();
	test_assignment_2();
	return 0;
}