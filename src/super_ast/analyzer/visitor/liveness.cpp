#include <iostream>
#include "liveness.hpp"

namespace super_ast {
Liveness::Liveness() {
  curr_scope_ = 0;
}

void Liveness::Visit(const Node* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  std::map<std::string, std::set<int>> vars;

  node->AcceptChildren(*this);

  add_from_top(vars);
  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const Block* node) {
  curr_scope_++;
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  std::map<std::string, std::set<int>> vars;

  for (int i = node->statements().size() - 1; i >= 0; --i) {
    node->statements()[i]->Accept(*this);
  }

  add_from_top(vars);
  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;

  curr_scope_--;
}

void Liveness::Visit(const For* node) {
  curr_scope_++;
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  node->post_iteration().Accept(*this);
  for (int i = 0; i < 2; i++) {
    node->body().Accept(*this);
  }
  node->condition().Accept(*this);
  node->initialization().Accept(*this);

  std::map<std::string, std::set<int>> vars;
  if (not live_stack_.empty()) vars = live_stack_.top();
  live_vars_in_[node->id()] = vars;

  curr_scope_--;
}

void Liveness::Visit(const While* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  for (int i = 0; i < 2; i++) {
    node->body().Accept(*this);
  }

  node->condition().Accept(*this);

  std::map<std::string, std::set<int>> vars;
  if (not live_stack_.empty()) vars = live_stack_.top();
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const Conditional* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  if (node->has_else_block()) {
    node->else_block().Accept(*this);
  }
  node->then_block().Accept(*this);
  node->condition().Accept(*this);

  std::map<std::string, std::set<int>> vars;
  if (not live_stack_.empty()) vars = live_stack_.top();
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const BinaryOperator* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  std::map<std::string, std::set<int>> vars;
  add_from_top(vars);

  node->left().Accept(*this);
  std::map<std::string, std::set<int>> left;
  add_from_top(left);
  if (node->type() == BinaryOperator::Type::ASSIGNMENT) {
    remove_live_vars(vars, left);
  }
  else {
    vars.insert(left.begin(), left.end());
  }

  node->right().Accept(*this);
  add_from_top(vars);

  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const Identifier* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  std::map<std::string, std::set<int>> vars;
  vars[node->value()].insert(curr_scope_);
  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const Atom* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  std::map<std::string, std::set<int>> vars;
  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const VariableDeclaration* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  std::map<std::string, std::set<int>> vars;
  add_from_top(vars);

  vars.erase(node->name());
  if (node->HasInitialization()) {
    node->initialization().Accept(*this);
    add_from_top(vars);
  }

  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const FunctionDeclaration* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  node->body().Accept(*this);

  parameters = true;
  for (int i = 0; i < node->parameters().size(); ++i) {
    node->parameters()[i]->Accept(*this);
  }
  parameters = false;

  std::map<std::string, std::set<int>> vars;
  add_from_top(vars);
  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;
}

void Liveness::Visit(const FunctionCall* node) {
  scope_[node->id()] = curr_scope_;

  if (not live_stack_.empty()) {
    live_vars_out_[node->id()] = live_stack_.top();
  }
  else {
    live_vars_out_[node->id()] = std::map<std::string, std::set<int>>();
  }

  std::map<std::string, std::set<int>> vars;
  add_from_top(vars);

  for (int i = 0; i < node->arguments().size(); ++i) {
    node->arguments()[i]->Accept(*this);
  }

  add_from_top(vars);
  live_stack_.push(vars);
  live_vars_in_[node->id()] = vars;
}

void Liveness::print_live() {
  for (auto var : live_vars_in_) {
    std::cout << "ID:" << var.first << " - ";
    print_live_vars(var.second);
  }
}

bool Liveness::isLiveIn(int id, std::string var) {
  return live_vars_in_[id].find(var) != live_vars_in_[id].end();
}

bool Liveness::isLiveOut(int id, std::string var) {
  return live_vars_out_[id].find(var) != live_vars_out_[id].end();
}

bool Liveness::isUsedSameScope(int id, std::string var) {
  return live_vars_out_[id].find(var)->second.find(scope_[id]) != live_vars_out_[id].find(var)->second.end();
}

// privates

void Liveness::print_live_vars(const std::map<std::string, std::set<int>>& vars) {
  for (auto v : vars) {
    std::cout << " " << v.first << " {";
    for (auto s : v.second) {
      std::cout << " " << s;
    }
    std::cout << " } --";
  }
  std::cout << std::endl;
}

void Liveness::add_from_top(std::map<std::string, std::set<int>>& vars) {
  if (not live_stack_.empty()) {
    auto child_vars = live_stack_.top();
    for (auto v : child_vars) {
      vars[v.first].insert(v.second.begin(), v.second.end());
    }
    live_stack_.pop();
  }
}

void Liveness::remove_live_vars(std::map<std::string, std::set<int>>& vars,
                                const std::map<std::string, std::set<int>>& vars_to_remove) {

  for (auto v : vars_to_remove) {
    if (vars.find(v.first) != vars.end()) {
      int scope = *v.second.begin();
      std::set<int> aux;
      for (auto s : vars[v.first]) {
        if (s >= scope) aux.insert(s);
      }
      for (auto s : aux) {
        vars[v.first].erase(s);
      }
      if (vars[v.first].empty()) vars.erase(v.first);
    }
  }

}
}
