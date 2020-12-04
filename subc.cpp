#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

int main(int argc, char **argv) {
    ifstream fin;
    fin.open(argv[argc-1], ios::in);
    if (fin.fail()) {
        cout << "Error opening the file." << endl;
        fin.close();
        return -1;
    }
    Parser p(&fin);
    p.Tiny();
    fin.close();
    return 0;
}