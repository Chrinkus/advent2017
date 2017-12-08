#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Tower {
    Tower(const string& n, int w, vector<string>& c)
        : name{n}, weight{w}, child_names{c} { }

    Tower(const string& n, int w)
        : name{n}, weight{w} { }

    Tower()
        : name{""}, weight{0}, child_names{vector<string>()} { }

    string name;
    int weight;
    vector<string> child_names;
};

istream& operator>>(istream& is, Tower& tt)
{
    string line;
    getline(is, line);
    istringstream iss {line};

    string nn;
    int ww;
    char ch1, ch2;
    vector<string> v;

    iss >> nn >> ch1 >> ww >> ch2;
    if (ch1 != '(' || ch2 != ')') {
        is.clear(ios_base::failbit);
        return is;
    }

    string a;
    if (iss >> a && a == "->") {
        for (string s; iss >> s; ) {
            if (s.back() == ',') s.pop_back();
            v.push_back(s);
        }
        tt = Tower{nn, ww, v};
    } else {
        tt = Tower{nn, ww};
    }

    return is;
}

ostream& operator<<(ostream& os, Tower& tt)
{
    return os << tt.name << '\t'
              << tt.weight << '\t'
              << tt.child_names.size() << '\n';
}

void fill_from_file(istream& is, vector<Tower>& v)
{
    for (Tower t; is >> t; )
        v.push_back(t);
}

void fill_from_file(istream& is, unordered_map<string, Tower>& u)
{
    for (Tower t; is >> t; )
        u[t.name] = t;
}

Tower* filter_out_children(const vector<Tower*>& pv) {
    vector<Tower*> vt;

    for (const auto& a : pv) {
        bool match = false;

        for (const auto& b : a->child_names)
            for (const auto& c : pv)
                if (c->name == b)
                    match = true;

        if (match) vt.push_back(a);
    }
    if (vt.size() > 1)
        return filter_out_children(vt);
    else
        return vt[0];
}

Tower* get_root(vector<Tower>& v)
{
    vector<Tower*> pvt;
    for (auto& a : v)
        pvt.push_back(&a);

    return filter_out_children(pvt);
}

Tower* get_root(unordered_map<string, Tower>& u)
{
    vector<Tower*> pvt;
    for (auto& a : u)
        pvt.push_back(&a.second);

    return filter_out_children(pvt);
}

struct Node {
    Node(Tower* p, vector<Node> v)
        : tower{p}, children{v} { }

    Tower* tower;
    vector<Node> children;
};

Node get_node(Tower* p, unordered_map<string, Tower>& u)
{
    vector<Node> vn;

    for (const auto& a : p->child_names) {
        Tower* t = &u[a];
        Node n = get_node(t, u);
        vn.push_back(n);
    }

    Node nn {p, vn};
    return nn;
}

int get_child_sums(Node& n)
{
    int sum = 0;
    for (auto& a : n.children)
        sum += a.tower->weight + get_child_sums(a);

    return sum;
}

vector<int> get_weight_dist(Node& n)
{
    vector<int> weights;
    for (auto& a : n.children) {
        int w = a.tower->weight + get_child_sums(a);
        weights.push_back(w);
    }
    return weights;
}

int find_diff(vector<int>& vi)
    // return the index where the different value is found or -1 if all the
    // same
{
    for (int i = 0; i < vi.size(); ++i) {
        if (vi[i] != vi.back()) {
            return vi[i+1] == vi.back() ? i : vi.size() - 1;
        }
    }
    return -1;
}

void track_it_down(Node& n)
{
    Node nn = n;

    while(nn.children.size() > 0) {
        vector<int> weights = get_weight_dist(nn);

        for (auto& a : weights)
            cout << a << '\t';
        cout << '\n';

        int diff = find_diff(weights);
        cout << diff << '\n';

        if (diff == -1) return;
        cout << nn.tower->name << '\n';
        nn = nn.children[diff];
        cout << nn.tower->name << '\n';
    }
}

int main()
try {
    cout << "Which input shall be run?\n";
    string file;
    cin >> file;
    string iname = "../inputs/day7_" + file + ".txt";
    ifstream ifs {iname};
    if (!ifs) throw runtime_error("Could not read from file " + iname);

    unordered_map<string, Tower> ut;
    fill_from_file(ifs, ut);

    Tower* pr = get_root(ut);
    Node tree = get_node(pr, ut);

    track_it_down(tree);
    /*
    vector<int> test_w = get_weight_dist(tree);
    for (const auto& a : test_w)
        cout << a << '\n';
        */
}
catch(exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch(...) {
    cerr << "Unknown exception\n";
    return 2;
}
