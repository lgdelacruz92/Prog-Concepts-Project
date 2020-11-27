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

int main() {
	return 0;
}