#pragma once

#include <set>
#include <map>
#include <stack>
#include "super_ast.hpp"

// compute the liveness of each statement
namespace super_ast {
  class Liveness : public Visitor {
  public:
    Liveness();

    // traversal of the super_ast
    void Visit(const Node* node);
    void Visit(const Block* node);
    void Visit(const For* node);
    void Visit(const While* node);
    void Visit(const Conditional* node);
    void Visit(const BinaryOperator* node);
    void Visit(const Identifier* node);
    void Visit(const Atom* node);
    void Visit(const VariableDeclaration* node);
    void Visit(const FunctionDeclaration* node);
    void Visit(const FunctionCall* node);

    void print_live();

    bool isLiveIn(int id, std::string var);
    bool isLiveOut(int id, std::string var);
    bool isUsedSameScope(int id, std::string var);

  private:
    // in the liveness analysis, the string is used to identify the 
    // variable name and the set to store the scopes where is live
    
    // used to accumulate the previous live variables
    std::stack<std::map<std::string, std::set<int>>> live_stack_;
    // live variables just before the statement
    std::map<int, std::map<std::string, std::set<int>>> live_vars_in_;
    // live variables just after the statement
    std::map<int, std::map<std::string, std::set<int>>> live_vars_out_;
    // scopes of every node
    std::map<int, int> scope_;
    unsigned int curr_scope_;
    bool parameters = false;

    void print_live_vars(const std::map<std::string, std::set<int>>& vars);
    void add_from_top(std::map<std::string, std::set<int>>& vars);
    void remove_live_vars(std::map<std::string, std::set<int>>& vars, 
        const std::map<std::string, std::set<int>>& vars_to_remove);
  };

} // namespace
