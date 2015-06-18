#include <stack>
#include <iostream>
#include <sstream>
#include "recursivity.hpp"
#include "function_ids.hpp"
#include "call_graph.hpp"

Recursivity::Recursivity() {}

std::vector<Error> Recursivity::check(const super_ast::Block* ast,
                                std::vector<struct RecursivityCheck> checks) {
    FunctionIds fids;
    ast->Accept(fids);
    std::map<std::string, struct FunctionInfo> ids = fids.get_ids();

    CallGraph cg;
    cg.set_ids(ids);
    ast->Accept(cg);
    call_graph_ = cg.get_graph();
    call_count_ = cg.get_num_calls();

    call_count_.resize(call_graph_.size());
    std::vector<int> recursivity = get_recursivity();

    //for (int i = 0; i < call_graph_.size(); ++i) {
    //    std::cout << "[" << i << "] ";
    //    for (int j = 0; j < call_graph_[i].size(); ++j) {
    //        std::cout << " " << call_graph_[i][j];
    //    }
    //    std::cout << std::endl;
    //}

    //for (int i = 0; i < recursivity.size(); ++i) {
    //    std::cout << "[" << i << "] " << recursivity[i] << std::endl;
    //}

    //Error checking
    std::vector<Error> errors;
    for (struct RecursivityCheck c : checks) {
        struct FunctionInfo finf = ids[c.signature];
        if (c.depth != -1 and recursivity[finf.id] != c.depth) {
            // Error encountered 
            std::ostringstream ss;
            ss << "expected depth " << c.depth << " found " << recursivity[finf.id];
            std::string detail = ss.str();
            struct Error e(finf.line, c.signature, "RECURSIVITY ERROR",
                            detail);
            errors.push_back(e);
        }
    }
    return errors;
}

std::vector<int> Recursivity::get_recursivity() {
    std::vector<int> depth(call_graph_.size());
    seen_.assign(call_graph_.size(),false);
    for (int i = 0; i < call_graph_.size(); ++i) {
        depth[i] = get_cycle_depth(i, i, 0);
        seen_.assign(call_graph_.size(),false);
    }
    return depth;
}

int Recursivity::get_cycle_depth(int current, int goal, int depth) {
    for (int f : call_graph_[current]) {
        if (seen_[f]) {
            continue;
        }
        seen_[f] = true;
        if (f == goal) {
            return depth;
        } 
        else {
            int depth_found = get_cycle_depth(f, goal, depth + 1);
            if (depth_found != -1) {
                return depth_found;
            }
        }
    }
    std::cout << std::endl;
    return -1;
}
