#include <iostream>
#include <sstream>
#include "../parser.h"
#include "./test_utils.h"
using namespace std;

class TestableBodyParser: public Parser {
    public:
        void _Statement() override {

        }
};

void test_Body_1() {
    string id = "abc\000";
    istringstream iss(id);
    TestableBodyParser p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, true, "Success abc is identifier", "Error \'abc\' is not an identifier");

}

int main() {
    test_Body_1();
    return 0;
}