#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <memory>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Program {
    std::string name;
    int weight = 0;
    Program* parent;
    std::vector<std::string> children;

    // Part 2
    int above_weight = 0;
    bool is_balanced = false;
};

std::ostream& operator<<(std::ostream& os, const Program& p)
{
    os << p.name << ' ' << p.weight << " [ ";
    for (const auto& child : p.children)
        os << child << ' ';
    os << "] (" << p.above_weight << ") Balance: " << p.is_balanced;
    return os;
}

std::unique_ptr<Program> program_factory(const std::string& line)
{
    auto p = std::make_unique<Program>();

    static const std::regex name_age_pat{R"(^(\w+)\s\((\d+)\))"};
    std::smatch matches;
    std::regex_search(line, matches, name_age_pat);

    auto it = ++matches.begin();
    p->name = *it++;
    p->weight = std::stoi(*it++);

    static const std::regex child_pat {R"((\w+))"};
    std::string rest = matches.suffix();
    auto rit = std::regex_iterator<std::string::iterator> {std::begin(rest),
                                                           std::end(rest),
                                                           child_pat};
    auto sentinel = std::regex_iterator<std::string::iterator>{};
    while (rit != sentinel) {
        p->children.push_back(rit->str());
        ++rit;
    }

    return p;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Tower {
public:
    explicit Tower(std::istream& is);

    void print_base() const;
    void report_imbalance();

private:
    void establish_parentage();
    void set_base();
    void establish_weights(Program* p);
    void check_balance(Program* p);
    Program* find_imbalance(Program* p);

    std::unordered_map<std::string,std::unique_ptr<Program>> tower;
    Program* base = nullptr;
};

Tower::Tower(std::istream& is)
{
    std::string line;
    while (getline(is, line)) {
        auto p = program_factory(line);
        tower[p->name] = std::move(p);
    }

    establish_parentage();
    set_base();
    establish_weights(base);
    check_balance(base);
}

void Tower::print_base() const
{
    std::cout << "Tower base: " << *base << '\n';
}

void Tower::report_imbalance() 
{
    const auto p = find_imbalance(base);

    std::vector<Program*> children;
    for (const auto& child : p->children)
        children.push_back(tower[child].get());

    std::cout << "Unbalanced disc: " << p->name << '\n';
    for (const auto child : children)
        std::cout << child->name << ' ' << child->weight
                  << " (" << child->weight + child->above_weight << ")\n";
}

void Tower::establish_parentage()
{
    for (const auto& program : tower)
        for (const auto& child : program.second->children)
            tower[child]->parent = program.second.get();
}

void Tower::set_base()
{
    for (const auto& program : tower)
        if (!program.second->parent) {
            base = program.second.get();
            return;
        }
}

void Tower::establish_weights(Program* p)
{
    for (const auto& child : p->children) {
        auto q = tower[child].get();
        establish_weights(q);
        p->above_weight += q->weight + q->above_weight;
    }
}

void Tower::check_balance(Program* p)
{
    std::vector<int> above_weights;

    for (const auto& child : p->children) {
        auto q = tower[child].get();

        if (q->above_weight == 0)
            q->is_balanced = true;
        else
            check_balance(q);

        above_weights.push_back(q->weight + q->above_weight);
    }

    if (p->above_weight == above_weights.front() * above_weights.size())
        p->is_balanced = true;
}

Program* Tower::find_imbalance(Program* p) 
    // this seems bad
{
    for (const auto& child : p->children) {
        auto q = tower[child].get();
        if (!q->is_balanced) {
            return find_imbalance(q);
        }
    }
    return p;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

int main()
{
    std::cout << "AoC 2017 Day 7 Revisited\n";

    Tower t {std::cin};
    t.print_base();
    t.report_imbalance();

    std::cout << '\n'
              << "Size of Tower: " << sizeof(Tower) << '\n'
              << "Size of Program: " << sizeof(Program) << '\n';
}
