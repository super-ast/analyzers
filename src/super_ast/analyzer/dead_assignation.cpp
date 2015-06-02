#include "dead_assignation.hpp"

using namespace super_ast;

DeadAssignation::DeadAssignation(Liveness* liveness) {
  current_func_ = "main";
  liveness_ = liveness;
}

void DeadAssignation::Visit(const Node* node) {
  node->AcceptChildren(*this);
}

void DeadAssignation::Visit(const FunctionDeclaration* node) {
  current_func_ = node->name();
  node->body().Accept(*this);
}

void DeadAssignation::Visit(const VariableDeclaration* node) {
  if (not liveness_->isLiveOut(node->id(), node->name())) {
    Report(node->line(), node->name());
  }
}

void DeadAssignation::Visit(const BinaryOperator* node) {
  if (node->type() == BinaryOperator::Type::ASSIGNMENT) {
    const Identifier& id = dynamic_cast<const Identifier&>(node->left());
    if (not liveness_->isLiveOut(node->id(), id.value())) {
      Report(node->line(), id.value());
    }
  }
}

std::vector<Error> DeadAssignation::get_errors() {
  return errors_;
}

void DeadAssignation::Report(int line, std::string var_name) {
  errors_.push_back(Error(line, current_func_, SHORT_DESC, var_name + LONG_DESC));
}
