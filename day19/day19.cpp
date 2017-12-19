#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<vector<char>> load_diagram(const string& s)
{
    ifstream ifs {s};
    if (!ifs) throw runtime_error("Could not open file " + s);

    vector<vector<char>> vvc;
    for (string line; getline(ifs, line) && line.size() > 0; ) {
        vector<char> vc;
        for (auto& ch : line)
            vc.push_back(ch);
        vvc.push_back(vc);
    }

    return vvc;
}

int get_start(vector<char>& v)
{
    for (int i = 0; i < v.size(); ++i)
        if (!isspace(v[i]))
            return i;

    throw runtime_error("row is all whitespace");
}

char move(int& x, int& y, const char d, vector<vector<char>>& vv)
{
    switch(d) {
        case 'u': --y; break;
        case 'd': ++y; break;
        case 'l': --x; break;
        case 'r': ++x; break;
        default:
            throw runtime_error("Unknown direction");
    }
    return vv[y][x];
}

char turn(int x, int y, char d, vector<vector<char>>&vv)
{
    if (d == 'u' || d == 'd')
        return isspace(vv[y][x-1]) ? 'r' : 'l';
    else if (d == 'l' || d == 'r')
        return isspace(vv[y-1][x]) ? 'd' : 'u';

    throw runtime_error("Unknown direction");
}

int traverse(vector<vector<char>>& vv, string& letters)
{
    int x = get_start(vv.front());
    int y = 0;
    char direction = 'd';
    int steps = 0;
    while (++steps) {
        char next = move(x, y, direction, vv);

        if (isspace(next)) {            // end! return result
            return steps;
        }
        else if (isalpha(next)) {       // found letter! save it and keep on
            letters += next;
        }
        else if (next == '+') {         // only change direction if '+'
            direction = turn(x, y, direction, vv);
        }
    }
    
    throw runtime_error("Somehow false..");
}

int main(int argc, char* argv[])
try {
    string input = argc > 1 ? argv[1] : "";
    string iname = "./day19" + input + ".txt";

    vector<vector<char>> diagram = load_diagram(iname);

    string part_1 = "";
    int part_2 = traverse(diagram, part_1);

    cout << "The answer to part 1 is: " << part_1 << '\n';
    cout << "The answer to part 2 is: " << part_2 << '\n';
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
