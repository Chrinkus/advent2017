#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

struct Test_case {
    Test_case(const string& t, int a)
        :test{t}, answer{a} { }

    string test;
    int answer;
};

int char_to_int(const char ch)
    // reads a positive numeric character and converts it to the corresponding
    // integer (not the character code).
{
    int n = ch - '0';

    if (n < 0 || 9 < n)
        throw runtime_error("char_to_int(): char not digit");

    return n;
}

int sum_of_distant_matches(istream& is)
    // read a string of digits one at a time and add to sum if digit matches
    // the digit exactly halfway around the circular list.
{
    string input;
    is >> input;

    int sum = 0;
    int half = input.size() / 2;

    for (int i = 0; i < half; ++i)
        if (input[i] == input[i + half])
            sum += 2 * char_to_int(input[i]);

    return sum;
}

void run_test()
{
    // test
    vector<Test_case> vtc { {"1212", 6}, {"1221", 0}, {"123425", 4},
        {"123123", 12}, {"12131415", 4} };

    int sum = 0;
    for (auto tc : vtc) {
        istringstream iss {tc.test};
        sum = sum_of_distant_matches(iss);
        cout << "Does " << sum << " equal " << tc.answer << '\n';
    }
}

int main()
try {
    run_test();

    string iname = "./day1_input.txt";
    ifstream ifs {iname};
    if (!ifs)
        throw runtime_error("Could not read from file " + iname);

    int sum = sum_of_distant_matches(ifs);
    cout << "The answer to the captcha is " << sum << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
