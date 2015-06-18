#pragma once
#include "super_ast.hpp"

namespace super_ast {
class ModifiedVariable : public Visitor {
public:
  ModifiedVariable(const std::string& name)
      : is_modified_(false), name_(name) {}

  // Returns whether the variable with given name is modified in the node given,
  // assuming no function calls will modify it except read, and no variable
  // shadowing.
  static bool IsModified(const super_ast::Node* root, const std::string& name) {
    ModifiedVariable modified_variable(name);
    root->Accept(modified_variable);
    return modified_variable.is_modified_;
  }

  bool is_modified_;

private:
  void Visit(const super_ast::Node* node) {
    if (!is_modified_)
      node->AcceptChildren(*this);
  }

  // Search for the identifier as an argument of read function
  void Visit(const super_ast::FunctionCall* fcall) {
    if (fcall->name() == "read") {
      for (const Expression* expression : fcall->arguments()) {
        if (const Identifier* identifier = dynamic_cast<const Identifier*>(expression)) {
          if (identifier->value() == name_) {
            is_modified_ = true;
          }
        }
      }
    }
    if (!is_modified_)
      fcall->AcceptChildren(*this);
  }

  // Search identifier inside unary increment/decrement
  void Visit(const super_ast::UnaryOperator* uop) {
    if (IsUnaryModifying(uop->type())) {
      if (const Identifier* id = dynamic_cast<const Identifier*>(&uop->expression())) {
        if (id->value() == name_) {
          is_modified_ = true;
        }
      }
    }
    if (!is_modified_)
      uop->AcceptChildren(*this);
  }

  // Search for the identifier in an assignment
  void Visit(const super_ast::BinaryOperator* bop) {
    if (IsAssign(bop->type())) {
      if (const Identifier* identifier = dynamic_cast<const Identifier*>(&bop->left())) {
        if (identifier->value() == name_) {
          is_modified_ = true;
        }
      }
    }
    if (!is_modified_)
      bop->AcceptChildren(*this);
  }

  bool IsUnaryModifying(UnaryOperator::Type type) {
    return type == UnaryOperator::Type::POST_DECREMENT ||
      type == UnaryOperator::Type::PRE_DECREMENT ||
      type == UnaryOperator::Type::POST_INCREMENT ||
      type == UnaryOperator::Type::PRE_INCREMENT;
  }

  bool IsAssign(BinaryOperator::Type type) {
    return type == BinaryOperator::Type::ASSIGNMENT ||
        type == BinaryOperator::Type::ASSIGN_MODULO ||
        type == BinaryOperator::Type::ASSIGN_RSHIFT ||
        type == BinaryOperator::Type::ASSIGN_LSHIFT ||
        type == BinaryOperator::Type::ASSIGN_ADDITION ||
        type == BinaryOperator::Type::ASSIGN_DIVISION ||
        type == BinaryOperator::Type::ASSIGN_SUBTRACTION ||
        type == BinaryOperator::Type::ASSIGN_MULTIPLICATION;
  }

  const std::string& name_;
};
}

