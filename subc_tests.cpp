#include <iostream>
#include "parser.h"
using namespace std;

void test_tiny03() {
    try {
        Parser p;
        p.ReadFile("tiny_test_progs/tiny_03");
    } catch (...) {
        cout << "Error in tiny03" << endl;
    }
}

void test_tiny12() {
    try {
        Parser p;
        p.ReadFile("tiny_test_progs/tiny_12");
    } catch (...) {
        cout << "Error in tiny03" << endl;
    }
}

int main() {
    test_tiny03();
    test_tiny12();
    return 0;
}
