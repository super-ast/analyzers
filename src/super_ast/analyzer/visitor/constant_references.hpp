#pragma once

#include <iomanip>
#include "function_errors.hpp"

namespace super_ast {
class ConstantReferences : public FunctionErrors {
public:
  typedef FunctionErrors super;

  ConstantReferences();

  void Visit(const FunctionDeclaration* function_declaration);

private:
  void FilterByType(const std::vector<VariableDeclaration*>& variables, const std::vector<Type::Native>& types,
                    std::vector<VariableDeclaration*>& result);
};
}
