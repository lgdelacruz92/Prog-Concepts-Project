#include <iostream>
#include <fstream>
using namespace std;

void Tiny() {

}

int main() {
    ifstream fin;
    
    fin.open("testfile.txt", ios::in);
    if (fin.fail()) {
        cout << "Error opening the file." << endl;
        fin.close();
        return -1;
    }

    char my_c;
    int numberLines = 0;

    while (!fin.eof()) {
        fin.get(my_c);
        cout << my_c;
        if (my_c == '\n') {
            ++numberLines;
        }
    }
    cout << endl;
    cout << "Number of lines: " << numberLines + 1 << endl;
    fin.close();
    return 0;
}