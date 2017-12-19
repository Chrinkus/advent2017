#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "";
    string iname = "./dayXX" + input + ".txt";

    //cout << "The answer to part 1 is: " << part_1 << '\n';
    //cout << "The answer to part 2 is: " << part_2 << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
