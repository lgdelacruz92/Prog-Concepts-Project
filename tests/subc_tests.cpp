#include <iostream>
#include "../parser.h"
using namespace std;

void showError(string message) {
    cout << "\033[1;31m" << message << "\033[0m" << endl;
}

void test_tiny03() {
    try {
        Parser p;
        p.ReadFile("tiny_test_progs/tiny_03");
    } catch (...) {
        showError("Error in tiny_03");
    }
}

int main() {
    test_tiny03();
    return 0;
}
