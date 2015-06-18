#include <iostream>
#include <iomanip>
#include "super_ast.hpp"
#include "../error.hpp"

#ifndef _RECU_H
#define _RECU_H

struct RecursivityCheck {
    std::string signature;
    int depth;
    int base_cases;
    RecursivityCheck() {}
    RecursivityCheck(std::string s, int d, int b) : signature(s), depth(d), base_cases(b) {}
};

class Recursivity {
public:
    Recursivity();
    std::vector<Error> check(const super_ast::Block* ast,
                             std::vector<struct RecursivityCheck> checks);

private:
    std::vector<int> get_recursivity(); 
    int get_cycle_depth(int current, int goal, int depth);

    std::vector<std::vector<int>> call_graph_;
    std::vector<int> call_count_;
    std::vector<bool> seen_;
};

#endif
