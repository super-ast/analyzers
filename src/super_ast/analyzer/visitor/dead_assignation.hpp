#pragma once

#include <vector>
#include "super_ast.hpp"
#include "../error.hpp"
#include "liveness.hpp"

namespace super_ast {
  class DeadAssignation : public Visitor {
  public:
    DeadAssignation(Liveness* liveness);

    void Visit(const Node* node);
    void Visit(const FunctionDeclaration* node);
    void Visit(const VariableDeclaration* node);
    void Visit(const BinaryOperator* node);

    std::vector<Error> get_errors();

  private:
    const std::string SHORT_DESC = "Useless definition";
    const std::string LONG_DESC = " is defined but its current value isn't used";

    super_ast::Liveness* liveness_;
    std::vector<Error> errors_;
    std::string current_func_;

    void Report(int line, std::string var_name);

  };

} 
