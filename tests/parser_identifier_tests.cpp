#include <sstream>
#include "../parser.h"
#include "./test_utils.h"
using namespace std;

class TestableParserIdentifier : public Parser {
    public:
        TestableParserIdentifier(istream* _fin);
};

TestableParserIdentifier::TestableParserIdentifier(istream* _fin) {
    fin = _fin;
    line = 0;
}

void test_IsIdentifier_1() {
    string id = "abc\000";
    istringstream iss(id);
    TestableParserIdentifier p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, true, "Success abc is identifier", "Error \'abc\' is not an identifier");
}

void test_IsIdentifier_2() {
    string id = "a$c\000";
    istringstream iss(id);
    TestableParserIdentifier p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, false, "Success a$b is NOT an identifier", "Error a$c should not be an identifier");
}

void test_IsIdentifier_3() {
    string id = "2ab\000";
    istringstream iss(id);
    TestableParserIdentifier p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, false, "Success 2ab is NOT an identifier", "Error 2ab should not be an identifier");
}

void test_IsIdentifier_4() {
    string id = "abd$\000";
    istringstream iss(id);
    TestableParserIdentifier p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, false, "Success abd$ is NOT an identifier", "Error abd$ shot not be an identifier");
}

void test_IsIdentifier_5() {
    string id = "  abd\000";
    istringstream iss(id);
    TestableParserIdentifier p(&iss);
    p.fin->seekg(0);
    p.fin->get(p.my_c);
    bool result = p._IsIdentifier();
    assertEqual(result, true, "Success abd is an identifier", "Error abd shot not be an identifier");   
}

int main() {
    test_IsIdentifier_1();
    test_IsIdentifier_2();
    test_IsIdentifier_3();
    test_IsIdentifier_4();
    test_IsIdentifier_5();
    return 0;
}