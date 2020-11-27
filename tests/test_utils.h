#include <iostream>
using namespace std;

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