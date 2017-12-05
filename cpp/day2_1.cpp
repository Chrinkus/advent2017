#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Row_data {
    Row_data(vector<int>& v);
    int min = 0;
    int max = 0;
};

Row_data::Row_data(vector<int>& v)
{
    for (auto i : v) {
        if (min == 0 && max == 0) {
            min = i;
            max = i;
        }
        else if (i < min)
            min = i;
        else if (i > max)
            max = i;
    }
}

void fill_from_file(istream& is, vector<Row_data>& v)
{
    for (string line; getline(is, line); ) {
        stringstream ss {line};
        vector<int> row;

        for (int i; ss >> i; )
            row.push_back(i);

        Row_data rd {row};
        v.push_back(rd);
    }
}

int get_checksum(vector<Row_data>& vrd)
{
    int checksum = 0;

    for (auto r : vrd)
        checksum += r.max - r.min;

    return checksum;
}

int main()
try {
    string iname = "./day2_input.txt";
    ifstream ifs {iname};
    if (!ifs)
        throw runtime_error("Could not read from file " + iname);

    vector<Row_data> vrd;
    fill_from_file(ifs, vrd);

    int checksum = get_checksum(vrd);

    cout << "Checksum of input is " << checksum << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
