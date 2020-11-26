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

void showError(string error) {
    cout << "\033[1;31m" << error << "\033[0m" << endl;
}

void showSuccess(string success) {
    cout << "\033[1;32m" << success << "\033[0m" << endl;
}

void assertEqual(bool a, bool b, string success, string error) {
    if (a == b) {
        showSuccess(success);
    } else {
        showError(error);
    }
}

void test_IsIdentifier_1() {
    string id = "abc\000";
    istringstream iss(id);
    TestableParser p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, true, "Success abc is identifier", "Error \'abc\' is not an identifier");
}

void test_IsIdentifier_2() {
    string id = "a$c\000";
    istringstream iss(id);
    TestableParser p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, false, "Success a$b is NOT an identifier", "Error a$c should not be an identifier");
}

int main() {
    test_IsIdentifier_1();
    test_IsIdentifier_2();
    return 0;
}