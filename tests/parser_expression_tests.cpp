#include <iostream>
#include <sstream>
#include "../parser.h"
#include "./test_utils.h"
using namespace std;

class ParserExpressionTests: public Parser {
public:
	ParserExpressionTests(istream* _fin) {
		fin = _fin;
		line = 0;
	}
};

int main() {
	return 0;
}