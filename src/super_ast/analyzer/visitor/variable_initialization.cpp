#include <iostream>
#include "variable_initialization.hpp"

using namespace super_ast;

VariableInitialization::VariableInitialization() {
  current_func_ = "main";
}

void VariableInitialization::Visit(const Node* node) {
  node->AcceptChildren(*this);
}

void VariableInitialization::Visit(const FunctionDeclaration* node) {
  current_func_ = node->name();
  node->body().Accept(*this);
}

void VariableInitialization::Visit(const VariableDeclaration* node) {
  if (not node->HasInitialization()) {
    Report(*node);
  }
}

std::vector<Error> VariableInitialization::get_errors() {
  return errors_;
}

void VariableInitialization::Report(const VariableDeclaration& variable) {
  errors_.push_back(Error(variable.line(), current_func_, SHORT_DESC, variable.name() + LONG_DESC));
}
