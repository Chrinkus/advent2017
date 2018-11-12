#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <memory>               // std::make_unique

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

template<typename InputIterator>
InputIterator find_diff(InputIterator first, InputIterator last)
    // find the different value in a range of otherwise equal values.
    // requires a range of 3 or more values, if more than one value is 'diff'
    // behaviour is undefined.
    // Returns last if all values are equal.
{
    if (std::distance(first, last) < 3)     // too small to have 'diff'
        return last;
    if (*first == *(first + 1)) {
        auto it = first + 2;                // safe
        while (it != last && *it == *first)
            ++it;
        return it;
    } else {
        return *first == *(first + 2) ? first + 1 : first;
    }
}

template<typename InputIterator, typename BinaryCompare>
InputIterator find_diff(InputIterator first, InputIterator last,
                        BinaryCompare cmp)
{
    if (std::distance(first, last) < 3)
        return last;
    if (cmp(*first, *(first + 1))) {
        auto it = first + 2;
        while (it != last && cmp(*it, *first))
            ++it;
        return it;
    } else {
        return cmp(*first, *(first + 2)) ? first + 1 : first;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Program_data {
    explicit Program_data(const std::string& line);

    std::string name;
    int weight = 0;
    std::vector<std::string> children;
};

Program_data::Program_data(const std::string& line)
{
    static const std::regex name_age_pat {R"(^(\w+)\s\((\d+)\))"};
    static const std::regex child_pat {R"((\w+))"};

    std::smatch matches;
    std::regex_search(line, matches, name_age_pat);

    auto it = ++matches.begin();        // skip full match
    name = *it++;
    weight = std::stoi(*it++);
    std::string rest = matches.suffix();

    // could avoid this work if end of string (no children)
    auto rit = std::regex_iterator<std::string::iterator>{std::begin(rest),
                                                          std::end(rest),
                                                          child_pat};
    auto sentinel = std::regex_iterator<std::string::iterator>{};
    while (rit != sentinel) {
        children.push_back(rit->str());
        ++rit;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

struct Program {
    explicit Program(const std::string& n) : name{n} { }

    int total_weight() const { return weight + above_weight; }

    std::string name;
    int weight = 0;
    int above_weight = 0;
    bool is_balanced = false;
    Program* parent = nullptr;
    std::vector<Program*> children;
};

std::ostream& operator<<(std::ostream& os, const Program& p)
{
    os << p.name << '\t' << p.weight << '\t' << p.above_weight << '\t'
       << p.total_weight() << "\t[ ";
    for (const auto child : p.children)
        os << child->name << ' ';
    os << ']';
    return os;
}

bool operator==(const Program& a, const Program& b)
{
    return a.total_weight() == b.total_weight();
}

bool operator!=(const Program& a, const Program& b)
{
    return !(a == b);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

class Tower {
public:
    explicit Tower(std::istream& is);

    void print_base() const;
    void report_imbalance() const;
private:
    void add_program(const std::string& line);
    Program* retrieve_program(const std::string& key);
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
        add_program(line);
    }

    set_base();
    establish_weights(base);
    check_balance(base);
}

void Tower::print_base() const
{
    std::cout << '\n'
              << "Part 1: Tower base is " << base->name << '\n';
}

void Tower::report_imbalance() const
{
    const auto p_unbal = find_imbalance(base);

    auto p_prob = *find_diff(std::begin(p_unbal->children),
                              std::end(p_unbal->children),
                              [](Program* a, Program* b)
                              { return *a == *b; });
    auto p_targ = p_prob == p_unbal->children.front()
                                            ? ++p_unbal->children.front()
                                            : p_unbal->children.front();
    std::cout << '\n'
              << "Part 2: " << p_prob->name << " needs to be "
              << p_prob->weight + p_targ->total_weight() -
                                  p_prob->total_weight() << '\n';
}

void Tower::add_program(const std::string& line)
{
    auto data = Program_data{line};

    auto p = retrieve_program(data.name);
    p->weight = data.weight;
    for (const auto& child : data.children) {
        auto q = retrieve_program(child);
        q->parent = p;
        p->children.push_back(q);
    }
}

Program* Tower::retrieve_program(const std::string& key)
{
    auto& program = tower[key];
    if (program)
        return program.get();

    program = std::make_unique<Program>(key);
    return program.get();
}

void Tower::set_base()
{
    for (const auto& program : tower)
        if (!program.second->parent) {
            base = program.second.get();
            return;
        }
}

void Tower::establish_weights(Program* program)
{
    for (const auto child : program->children) {
        establish_weights(child);
        program->above_weight += child->total_weight();
    }
}

void Tower::check_balance(Program* program)
    // requires that above weights have already been calculated
{
    std::vector<int> above_weights;

    for (const auto child : program->children) {
        if (child->above_weight == 0)
            child->is_balanced = true;
        else
            check_balance(child);

        above_weights.push_back(child->total_weight());
    }

    if (program->above_weight == above_weights.front() * above_weights.size())
        program->is_balanced = true;
}

Program* Tower::find_imbalance(Program* program) const
{
    for (const auto child : program->children) {
        if (!child->is_balanced) {
            return find_imbalance(child);
        }
    }
    return program;
}

int main()
{
    std::cout << "AoC 2017 Day 7 Revisited\n";

    Tower t {std::cin};
    t.print_base();
    t.report_imbalance();
}
