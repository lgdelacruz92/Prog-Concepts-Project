#include <iostream>
#include <sstream>
#include "../parser.h"
using namespace std;

class TestableParser : public Parser {
    public:
        TestableParser(istream* _fin);
};

TestableParser::TestableParser(istream* _fin) {
    fin = _fin;
    line = 0;
}



void showError(string message) {
    cout << "\033[1;31m" << message << "\033[0m" << endl;
}

void assertEqual(bool a, bool b, string message) {
    if (a == b) {
        cout << "Success " << a << " equals " << b << endl;
    } else {
        showError(message);
    }
}

void test_IsIdentifier_1() {
    string id = "abc";
    istringstream iss(id);
    TestableParser p(&iss);
    bool result = p._IsIdentifier();
    assertEqual(result, true, "Error \'abc\' is not an identifier");
    cout << "end of tests" << endl;
}

int main() {
    test_IsIdentifier_1();
    return 0;
}