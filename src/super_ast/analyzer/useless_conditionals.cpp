#include <iostream>
#include <iomanip>
#include "super_ast.hpp"

namespace super_ast {
class UselessConditionals : public Visitor {
public:
  UselessConditionals() {

  }

  void Visit(const super_ast::Node* node) {
    node->AcceptChildren(*this);
  }

  void Visit(const Conditional *conditional) {
    if(!conditional->has_else_block())
      return;

    const Block& then_block = conditional->then_block();
    const Block& else_block = conditional->else_block();

    if(HasReturn(then_block) && HasReturn(else_block)) {
      Report(*conditional, "Unnecessary conditional with returns");
    } else {
      std::string assigned_variable_then;
      std::string assigned_variable_else;

      if(HasVariableAssignment(then_block, assigned_variable_then) &&
          HasVariableAssignment(else_block, assigned_variable_else) &&
          assigned_variable_then == assigned_variable_else) {
        Report(*conditional, "Unnecessary conditional with variable assignment");
      }
    }
  }

  bool HasReturn(const Block& block) {
    return block.statements().size() == 1 &&
        dynamic_cast<Return*>(block.statements()[0]);
  }

  bool HasVariableAssignment(const Block& block, std::string& variable_name) {
    if(block.statements().size() != 1)
      return false;

    const Statement* statement = block.statements()[0];

    if(const BinaryOperator* bop = dynamic_cast<const BinaryOperator*>(statement)) {
      if(bop->type() == BinaryOperator::Type::ASSIGNMENT) {
        if(const Identifier* identifier = dynamic_cast<const Identifier*>(&bop->left())) {
          if(dynamic_cast<const Boolean*>(&bop->right())) {
            // If a boolean is assigned to a variable
            variable_name = identifier->value();
            return true;
          }
        }
      }
    } else if(const VariableDeclaration* variable = dynamic_cast<const VariableDeclaration*>(statement)) {
      if(variable->data_type().referenced_type() == *Type::Boolean()) {
        // If a boolean variable is declared
        variable_name = variable->name();
        return true;
      }
    }

    return false;
  }

  void Report(const Node& node, std::string message) {
    std::cout << std::left << std::setw(5) << node.id() << message << std::endl;
  }
};
}

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  super_ast::UselessConditionals useless_conditionals;
  ast->Accept(useless_conditionals);

  return 0;
}
