#include <iostream>
#include <sstream>
#include "../parser.h"
#include "./test_utils.h"
using namespace std;

class TestableBodyParser: public Parser {
    public:
        TestableBodyParser(istream* _fin) {
            fin = _fin;
            line = 0;
        }
};

/**
 * Make sure body starts with begin
 * @return void
 */
void test_Body_1() {
    string program = "begin\n"
                     "  read(i)\n"
                     "end\000";
    istringstream iss(program);
    TestableBodyParser p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    try {
        p._Body();
        assertEqual(true, true, "Success Body should not throw an error", "");
    } catch(int e) {
        assertEqual(true, false, "", "Error, Body threw an error");
    }
}

int main() {
    test_Body_1();
    return 0;
}