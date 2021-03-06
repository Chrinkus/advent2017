#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// map functions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
vector<vector<char>> load_map(const string& s)
{
    ifstream ifs {s};
    if (!ifs) throw runtime_error("Could not read from file " + s);

    vector<vector<char>> map;
    for (string line; getline(ifs, line); ) {

        vector<char> vc;
        for (auto& a : line)
            vc.push_back(a);

        map.push_back(vc);
    }

    return map;
}

void expand(vector<vector<char>>& vvc)
{
    vector<vector<char>> vv;
    int new_size = vvc.size() + 2;

    vector<char> v_dot (new_size, '.');
    vv.push_back(v_dot);

    for (auto& a : vvc) {
        vector<char> v {'.'};
        for (auto& b : a)
            v.push_back(b);
        v.push_back('.');
        vv.push_back(v);
    }
    vv.push_back(v_dot);

    vvc = vv;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Facing enum (first enum design!)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
enum class Facing {
    u, r, d, l
};

Facing operator++(Facing& f)
{
    f = (f==Facing::l) ? Facing::u : Facing(int(f) + 1);
    return f;
}

Facing operator--(Facing& f)
{
    f = (f==Facing::u) ? Facing::l : Facing(int(f) - 1);
    return f;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Carrier class
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
struct Carrier {
    Carrier(int xx, int yy, Facing ff = Facing::u)
        : x{xx}, y{yy}, f{ff} { }

    void burst(vector<vector<char>>&, int&);
    void turn(vector<vector<char>>&);
    void move(vector<vector<char>>&);

    int x;
    int y;
    Facing f;
};

void Carrier::burst(vector<vector<char>>& vvc, int& count)
{
    // updated turn
    turn(vvc);

    // flip node: MOAR
    char ch;
    switch (vvc[y][x]) {
        case '.': ch = 'W'; break;
        case 'W': ch = '#'; break;
        case '#': ch = 'F'; break;
        case 'F': ch = '.'; break;
        default:
            throw runtime_error("Carrier::burst(): unexpected state");
    }
    vvc[y][x] = ch;

    if (ch == '#') ++count;

    move(vvc);
}

void Carrier::turn(vector<vector<char>>& m)
{
    switch (m[y][x]) {
        case '.': --f;      break;
        case 'W':           break;
        case '#': ++f;      break;
        case 'F': ++f; ++f; break;
        default:
            throw runtime_error("Carrier::turn(): unexpected state");
    }
}

void Carrier::move(vector<vector<char>>& vvc)
{
    // detect limits of map
    if ((x == 0 || x == vvc.size() - 1) ||
        (y == 0 || y == vvc.size() - 1)) {
        expand(vvc);
        ++x;
        ++y;
    }

    switch (f) {
        case Facing::u: --y; break;
        case Facing::d: ++y; break;
        case Facing::l: --x; break;
        case Facing::r: ++x; break;
        default:
            throw runtime_error("Carrier::move(): unexpected facing");
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Challenge function
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int mass_burst(Carrier& c, vector<vector<char>>& m, int n)
{
    int count = 0;
    for (int i = 0; i < n; ++i) {
        c.burst(m, count);
    }

    cout << "m size: " << m.size() << ' '
         << "mf size: " << m.front().size() << ' '
         << "mb size: " << m.back().size() << '\n';

    //int infected = count_infected(m);
    return count;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
// Main
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
int main(int argc, char* argv[])
try {
    string input1 = argc > 1 ? argv[1] : "";
    string input2 = argc > 2 ? argv[2] : "10000000";
    string iname = "./day22" + input1 + ".txt";

    vector<vector<char>> map = load_map(iname);

    int center = floor(map.size() / 2.0);
    Carrier virus {center, center};

    int n = stoi(input2);
    int part_2 = mass_burst(virus, map, n);

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
