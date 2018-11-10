#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <memory>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Solution {
    int target = 0;
    int actual = 0;
    int to_change = 0;
};

template<typename InputIterator, typename BinaryPredicate>
InputIterator find_diff(InputIterator first, InputIterator last,
                        BinaryPredicate cmp)
    // Finds the value that is different in a set of similar values.
    // If range is smaller than 3 then there cannot be 'one' different value
    // so last is returned. If all of the values are the same, last is
    // returned. If there are multiple 'different' values then the behaviour
    // is undefined.
{
    if (std::distance(first, last) < 3)     // too small to have 'diff'
        return last;

    if (cmp(*first, *(first + 1))) {
        auto it = first + 2;                // safe by 'too small' check
        while (it != last && cmp(*it, *first))
            ++it;
        return it;
    } else {
        return cmp(*first, *(first + 2)) ? first + 1 : first;
    }
}

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
    os << "] (" << p.above_weight << ')';
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
    void report_imbalance() const;

private:
    void establish_parentage();
    void set_base();
    void establish_weights(Program* p);
    void check_balance(Program* p);
    Program* find_imbalance(Program* p) const;

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
    std::cout << '\n'
              << "Tower base: " << *base << '\n';
}

void Tower::report_imbalance() const
{
    const auto p = find_imbalance(base);

    std::vector<const Program*> children;
    for (const auto& child : p->children)
        children.push_back(tower.at(child).get());

    std::cout << '\n'
              << "Unbalanced disc: " << p->name << '\n';
    for (const auto child : children)
        std::cout << child->name << ' ' << child->weight
                  << " (" << child->weight + child->above_weight << ")\n";
    auto it = find_diff(std::begin(children), std::end(children),
            [](const Program* p, const Program* q) {
                return (p->weight + p->above_weight) == (q->weight + q->above_weight);
            });
    std::cout << "Problem: " << **it << '\n';
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

Program* Tower::find_imbalance(Program* p) const
    // this seems bad
{
    for (const auto& child : p->children) {
        const auto q = tower.at(child).get();
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
