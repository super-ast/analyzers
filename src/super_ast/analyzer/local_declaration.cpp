#include "local_declaration.hpp"

using namespace super_ast;

LocalDeclaration::LocalDeclaration(Liveness* liveness) {
  current_func_ = "main";
  liveness_ = liveness;
}

void LocalDeclaration::Visit(const Node* node) {
  node->AcceptChildren(*this);
}

void LocalDeclaration::Visit(const FunctionDeclaration* node) {
  current_func_ = node->name();
  node->body().Accept(*this);
}

// TODO check if lives
void LocalDeclaration::Visit(const VariableDeclaration* node) {
  if (not liveness_->isLiveOut(node->id(), node->name()) or
    not liveness_->isUsedSameScope(node->id(), node->name())) {
    Report(node->line(), node->name());
  }
}

std::vector<Error> LocalDeclaration::get_errors() {
  return errors_;
}

void LocalDeclaration::Report(int line, std::string var_name) {
  errors_.push_back(Error(line, current_func_, SHORT_DESC, var_name + LONG_DESC));
}
