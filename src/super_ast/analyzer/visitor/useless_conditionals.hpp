#pragma once

#include "super_ast.hpp"
#include "../error.hpp"

namespace super_ast {
class UselessConditionals : public Visitor {
public:
  UselessConditionals();

  void Visit(const super_ast::Node* node);
  void Visit(const Conditional* conditional);
  void Visit(const FunctionDeclaration* function_declaration);

  std::vector<Error> errors() const;

private:
  void Report(const Statement& node, std::string message);

  bool HasReturn(const Block& block);
  bool HasVariableAssignment(const Block& block, std::string& variable_name);

  std::string current_function_;
  std::vector<Error> errors_;
};
}
