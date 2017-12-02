#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int get_even_div(const vector<int>& v)
{
    for (auto a : v)
        for (auto b : v)
            if (a != b && a % b == 0)
                return a / b;

    throw runtime_error("No even division found");
}

int get_sum_of_divs(istream& is)
{
    int sum = 0;
    for (string line; getline(is, line); ) {
        istringstream iss {line};
        vector<int> row;

        for (int i; iss >> i; )
            row.push_back(i);

        sum += get_even_div(row);
    }

    return sum;
}

int main()
try {
    string iname = "./day2_input.txt";
    ifstream ifs {iname};
    if (!ifs)
        throw runtime_error("Could not open file " + iname);

    int sum = get_sum_of_divs(ifs);

    cout << "The sum of even divisions is " << sum << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
