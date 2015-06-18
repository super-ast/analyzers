#include "simple_fors.hpp"

namespace super_ast {
void SimpleFors::Visit(const super_ast::Node* node) {
  node->AcceptChildren(*this);
}

std::vector<Error> SimpleFors::GetErrors() {
  return errors_;
}

void SimpleFors::Visit(const For* forstmt) {
  // Check that init is a declaration or an assignment
  bool valid_init = false;
  std::string var_name;
  if (const VariableDeclaration* var_decl = 
      dynamic_cast<const VariableDeclaration*>(&forstmt->initialization())) {
    var_name = var_decl->name();
    valid_init = true;
  }
  if (const BinaryOperator* bop =
      dynamic_cast<const BinaryOperator*>(&forstmt->initialization())) {
    if (const Identifier* id = dynamic_cast<const Identifier*>(&bop->left())) {
      var_name = id->value();
      valid_init = true;
    }
  }

  // Check simple condition
  if (!IsSimpleCondition(&(forstmt->condition()))) {
    Report(forstmt->condition(), "Complex condition", 
        "Condition in loop contains || or && operator");
  }

  if (!valid_init) {
    Report(forstmt->initialization(), "Invalid init",
        "For init is not a simple assignment or variable declaration");
  } else {
    const Expression* post = &forstmt->post_iteration();
    // Check the postincrement
    if (const UnaryOperator* uop = dynamic_cast<const UnaryOperator*>(post)) {
      if (const Identifier* id = 
          dynamic_cast<const Identifier*>(&uop->expression())) {
        if (id->value() == var_name) {
          if (ModifiedVariable::IsModified(&forstmt->body(), var_name)) {
            Report(*post, "Invalid for post iteration",
                "Variable in post iteration is modified in the for body");
          }
        } else {
          Report(*post, "Invalid for post iteration",
              "Init var is different from post iteration var");
        }
      } else {
        Report(*post, "Invalid for post iteration",
            "Complex post iteration in loop");
      }
    }
    else if (const BinaryOperator* bop =
        dynamic_cast<const BinaryOperator*>(&forstmt->post_iteration())) {
      // Get the id that is assigned to and check if it is the same as
      // the variable in the init
      if (const Identifier* id = 
          dynamic_cast<const Identifier*>(&bop->left())) {
        if (id->value() == var_name) {
          // Check the type  'var = var + k'
          if (bop->type() == BinaryOperator::Type::ASSIGNMENT) {
            CheckAssign(&(bop->right()), var_name, &(forstmt->body()));
          }
          // Check the type  'var += k'
          else if (bop->type() == BinaryOperator::Type::ASSIGN_ADDITION ||
              bop->type() == BinaryOperator::Type::ASSIGN_SUBTRACTION) {
            CheckAssignSum(&(bop->right()), var_name, &(forstmt->body()));
          } else {
            Report(*post, "Invalid for post iteration", 
                "Complex post iteration");
          }
        } else {
          Report(*post, "Invalid for post iteration",
              "Init variable is different from post iteration variable");
        }
      } else {
        Report(*post, "Invalid for post iteration",
            "Left part of assignement in post iteration is not an identifier");
      }
    } else {
      Report(*post, "Invalid for post iteration", 
          "Complex post iteration in loop");
    }
  }

  forstmt->AcceptChildren(*this);
}

bool SimpleFors::IsSimpleCondition(const Expression* expr) {
  if (const BinaryOperator* bop = dynamic_cast<const BinaryOperator*>(expr)) {
    return (bop->type() != BinaryOperator::Type::AND &&
            bop->type() != BinaryOperator::Type::OR);
  }
  return false;
}

bool SimpleFors::IsValidBinaryOperator(const BinaryOperator* bop) {
  return bop->type() == BinaryOperator::Type::ASSIGNMENT ||
         bop->type() == BinaryOperator::Type::ASSIGN_ADDITION ||
         bop->type() == BinaryOperator::Type::ASSIGN_SUBTRACTION;
}

void SimpleFors::CheckAssign(const Expression* expr, const std::string& initName,
    const Block* body) {
  if (const BinaryOperator* bop = dynamic_cast<const BinaryOperator*>(expr)) {
    // Check if initVar is in both sides
    if (const Identifier* id = dynamic_cast<const Identifier*>(&bop->left())) {
      if (id->value() == initName && IsSimpleConstValue(&bop->right(), body))
        return;
    }
    if (const Identifier* id = dynamic_cast<const Identifier*>(&bop->right())) {
      if (id->value() == initName && IsSimpleConstValue(&bop->left(), body))
        return;
    }
  }
  Report(*expr, "Invalid for post iteration",
      "Post iteration variable not incremented by a constant");
}

void SimpleFors::CheckAssignSum(const Expression* expr, const std::string& initName,
    const Block* body) {
  bool valid = true;
  // Check for initVar += initVar
  if (const Identifier* id = dynamic_cast<const Identifier*>(expr)) {
    valid = id->value() != initName;
  }
  if (!IsSimpleConstValue(expr, body))
    valid = false;
  if (!valid) 
    Report(*expr, "Invalid for post iteration",
        "Post iteration variable not incremented by a constant");
}

// Returns true if the value node is a literal or a variable not
// modified in the body block.
bool SimpleFors::IsSimpleConstValue(const Node* value, const Block* body) {
  // Return true if it is already a literal
  if (dynamic_cast<const Integer*>(value) ||
      dynamic_cast<const Double*>(value) ||
      dynamic_cast<const Boolean*>(value)) {
    return true;
  }
  // Check if it is an unmodified identifier
  if (const Identifier* id = dynamic_cast<const Identifier*>(value)) {
    return !ModifiedVariable::IsModified(body, id->value());
  }
  return false;
}

void SimpleFors::Report(const Node& node, const std::string short_message, 
    const std::string long_message) {
  int line = -1;
  if (const Statement* stmt = dynamic_cast<const Statement*>(&node)) {
    line = stmt->line();
  }
  errors_.push_back(Error(line, "", short_message, long_message));
}
};

