#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void fill_from_file(istream& is, vector<string>& v)
{
    for (string s; is >> s; )
        v.push_back(s);
}

int main()
try {
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day8_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
