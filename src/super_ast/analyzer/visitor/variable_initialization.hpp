#pragma once

#include <vector>
#include "super_ast.hpp"
#include "../error.hpp"

// all variables should have an initialization
namespace super_ast {
  class VariableInitialization : public Visitor {
  public:
    VariableInitialization();

    void Visit(const Node* node);
    void Visit(const FunctionDeclaration* node);
    void Visit(const VariableDeclaration* node);

    std::vector<Error> get_errors();

  private:
    const std::string SHORT_DESC = "Variable not initialized";
    const std::string LONG_DESC = " has to be initialized after its declaration";

    std::vector<Error> errors_;
    std::string current_func_;

    void Report(const VariableDeclaration& variable);

  };

} // namespace
