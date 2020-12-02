#include <iostream>
#include <sstream>
#include "../parser.h"
#include "./test_utils.h"
using namespace std;

class ParserStringNodeTests: public Parser {
public:
	ParserStringNodeTests(istream* _fin) {
		fin = _fin;
		line = 0;
	}
};

void test_string_node_1() {
	string program = "a :=: b";
    istringstream iss(program);
    ParserStringNodeTests p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    try {
        p._StringNode();
        assertEqual(true, true, "Success Statement should not throw an error", "", "Test 2");
    } catch(int e) {
        assertEqual(true, false, "", "Error, Statement threw an error", "Test 2");
    }	
}

int main() {
	test_string_node_1();
	return 0;
}