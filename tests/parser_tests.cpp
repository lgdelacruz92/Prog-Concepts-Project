#include <iostream>
#include <fstream>
#include "../parser.h"
using namespace std;

void showError(string message) {
    cout << "\033[1;31m" << message << "\033[0m" << endl;
}

void test_IsIdentifier() {
}

int main() {
    test_IsIdentifier();
    return 0;
}