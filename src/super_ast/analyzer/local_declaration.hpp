#pragma once

#include <vector>
#include "super_ast.hpp"
#include "error.hpp"
#include "liveness.hpp"

namespace super_ast {
  class LocalDeclaration : public Visitor {
  public:
    LocalDeclaration(Liveness* liveness);
    
    void Visit(const Node* node);
    void Visit(const FunctionDeclaration* node);
    void Visit(const VariableDeclaration* node);

    std::vector<Error> get_errors();

  private:
    const std::string SHORT_DESC = "Bad declaration";
    const std::string LONG_DESC = " is declared too far from its use";

    super_ast::Liveness* liveness_;
    std::vector<Error> errors_;
    std::string current_func_;

    void Report(int line, std::string var_name);

  };

}
