#include <iostream>
#include <stdexcept>

using namespace std;

void run_test()
{
    // test
}

int main()
try {
    run_test();
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
