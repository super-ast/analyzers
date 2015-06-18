#include <iostream>
#include <iomanip>
#include "super_ast.hpp"

namespace super_ast {
class FindIdentifier : public Visitor {
public:
  FindIdentifier(const std::string& name) : name_(name), found_(false) {

  }

  void Visit(const Node* node) {
    if(!found_) {
      node->AcceptChildren(*this);
    }
  }

  void Visit(const Identifier* identifier) {
    found_ = (identifier->value() == name_);
  }

  bool found() {
    return found_;
  }

private:
  std::string name_;
  bool found_;
};

class CheckWrite : public Visitor {
public:
  CheckWrite(const std::string& name) : find_identifier(name), written_(false) {

  }

  void Visit(const Node* node) {
    if(!written_) {
      node->AcceptChildren(*this);
    }
  }

  void Visit(const BinaryOperator* binary_operator) {
    if(!written_ and IsAssignmentOperator(binary_operator)) {
      find_identifier.Visit(&binary_operator->left());
      written_ = find_identifier.found();
    }
  }

  bool written() {
    return written_;
  }

private:
  FindIdentifier find_identifier;
  bool written_;

  bool IsAssignmentOperator(const BinaryOperator* binary_operator) {
    BinaryOperator::Type type = binary_operator->type();

    return
        type == BinaryOperator::Type::ASSIGN_ADDITION ||
        type == BinaryOperator::Type::ASSIGN_SUBTRACTION ||
        type == BinaryOperator::Type::ASSIGN_MULTIPLICATION ||
        type == BinaryOperator::Type::ASSIGN_DIVISION ||
        type == BinaryOperator::Type::ASSIGN_MODULO ||
        type == BinaryOperator::Type::ASSIGN_LSHIFT ||
        type == BinaryOperator::Type::ASSIGN_RSHIFT;
  }
};

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
        CheckWrite check_write(vector_parameter->name());
        check_write.Visit(&function_declaration->body());

        if(!check_write.written()) {
          Report(*vector_parameter, "Vector parameter could be constant");
        }
      }
    }
  }

private:
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
