#include <iostream>
#include <iomanip>
#include "super_ast.hpp"

namespace super_ast {
class ConstRefVectors : public Visitor {
public:
  ConstRefVectors() {

  }

  void Visit(const Node* node) {
    node->AcceptChildren(*this);
  }

  void Visit(const FunctionDeclaration* function_declaration) {
    const std::vector<VariableDeclaration*>& parameters = function_declaration->parameters();

    std::vector<VariableDeclaration*> vector_parameters;
    FilterByType(parameters, Type::VECTOR, vector_parameters);

    for(VariableDeclaration* vector_parameter : vector_parameters) {
      if(vector_parameter->is_constant()) {
        if(!vector_parameter->is_reference()) {
          Report(*vector_parameter, "Vector parameter is constant but not passed by reference");
        }
      } else {
        // Check function block for writes
      }
    }
  }

  void FilterByType(const std::vector<VariableDeclaration*>& variables, Type::Native type,
                    std::vector<VariableDeclaration*>& result) {
    for(VariableDeclaration* variable : variables) {
      if(variable->data_type().referenced_type().is(type)) {
        result.push_back(variable);
      }
    }
  }

  void Report(const Node& node, std::string message) {
    std::cout << std::left << std::setw(5) << node.id() << message << std::endl;
  }
};
}

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  super_ast::ConstRefVectors const_ref_vectors;
  ast->Accept(const_ref_vectors);

  return 0;
}
