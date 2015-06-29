#pragma once

#include "super_ast.hpp"
#include "../error.hpp"

namespace super_ast {
class FunctionErrors : public Visitor {
public:
  FunctionErrors() {

  }

  void Visit(const super_ast::Node* node) {
    node->AcceptChildren(*this);
  }

  void Visit(const FunctionDeclaration* function_declaration) {
    current_function_ = function_declaration->name();
  }

  std::string current_function() const {
    return current_function_;
  }

  std::vector<Error> errors() const {
    return errors_;
  }

protected:
  void Report(const Statement& statement, const std::string& short_description, const std::string& long_description) {
    errors_.push_back(Error(statement.line(), current_function_, short_description, long_description));
  }

private:
  std::string current_function_;
  std::vector<Error> errors_;
};
}
