#pragma once

#include "function_errors.hpp"

namespace super_ast {
class UselessConditionals : public FunctionErrors {
public:
  UselessConditionals();

  void Visit(const Conditional* conditional);

private:
  bool HasReturn(const Block& block);
  bool HasVariableAssignment(const Block& block, std::string& variable_name);
};
}
