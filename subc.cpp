#include <iostream>
#include "parser.h"

using namespace std;

int main(int argc, char **argv) {
    Parser p;
    p.ReadFile(argv[argc-1]);
    return 0;
}