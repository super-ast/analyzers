#include "useless_conditionals.hpp"

namespace super_ast {
UselessConditionals::UselessConditionals() {

}

void UselessConditionals::Visit(const Conditional* conditional) {
  if(!conditional->has_else_block())
    return;

  const Block& then_block = conditional->then_block();
  const Block& else_block = conditional->else_block();

  if(HasReturn(then_block) && HasReturn(else_block)) {
    Report(*conditional, "Unnecessary conditional", "Unnecessary conditional with returns");
  } else {
    std::string assigned_variable_then;
    std::string assigned_variable_else;

    if(HasVariableAssignment(then_block, assigned_variable_then) &&
       HasVariableAssignment(else_block, assigned_variable_else) &&
       assigned_variable_then == assigned_variable_else) {
      Report(*conditional,  "Unnecessary conditional", "Unnecessary conditional with variable assignment");
    }
  }
}

bool UselessConditionals::HasReturn(const Block& block) {
  return block.statements().size() == 1 &&
         dynamic_cast<Return*>(block.statements()[0]);
}

bool UselessConditionals::HasVariableAssignment(const Block& block, std::string& variable_name) {
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
}
