#include <iostream>
#include <stack>
#include "super_ast.hpp"

class Grapher : public super_ast::Visitor {
public:
  Grapher() {
    counter_ = 0;
  }

  void Visit(const super_ast::Node* node) {
    counter_++;

    std::cout << "    " << counter_ << " [label=\"" << node->Representation() << "\"]" << std::endl;

    if(!path_.empty()) {
      std::cout << "    " << path_.top() << " -> " << counter_ << std::endl;
    }

    path_.push(counter_);
    node->AcceptChildren(*this);
    path_.pop();
  }

private:
  std::stack<int> path_;
  int counter_;
};

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  std::cout << "digraph G {" << std::endl;

  Grapher grapher;
  ast->Accept(grapher);

  std::cout << "}" << std::endl;

  return 0;
}
