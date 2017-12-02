#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>

using namespace std;

int char_to_int(const char ch)
    // reads a positive numeric character and converts it to the corresponding
    // integer (not the character code).
{
    int n = ch - '0';

    if (n < 0 || 9 < n)
        throw runtime_error("char_to_int(): char not digit");

    return n;
}

int sum_of_matches(istream& is)
    // Reads an input one char at a time and converts to digit. If the current
    // digit equals the previous digit, add it to the sum.
{
    char ch1 = 0;
    while (!ch1) is >> ch1;                 // filter out null chars

    int prev = char_to_int(ch1);

    int sum = 0;
    int cur = 0;
    for (char ch; is >> ch; ) {
        if (ch) {                           // test against end of stream
            cur = char_to_int(ch);
            if (cur == prev) sum += prev;
            prev = cur;
        }
    }

    if (prev == char_to_int(ch1)) sum += prev;

    return sum;
}

void run_test()
{
    int sum = 0;
    vector<string> vs {"1122", "1111", "1234", "9121212129"};

    for (string s : vs) {
        istringstream iss {s};
        sum = sum_of_matches(iss);
        cout << "The captcha sum of " << s << " is " << sum << '\n';
    }
}

int main()
try {
    //run_test();

    string iname = "./day1_input.txt";
    ifstream ifs {iname};
    if (!ifs)
        throw runtime_error("Could not read from file " + iname);

    int sum = sum_of_matches(ifs);

    cout << "The captcha sum is: " << sum << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
