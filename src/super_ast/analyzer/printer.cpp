#include <iostream>
#include <iomanip>
#include "super_ast.hpp"

class Printer : public super_ast::Visitor {
public:
  Printer() {
    depth_ = 0;
  }

  void Visit(const super_ast::Node* node) {
    std::cout << std::left << std::setw(5) << node->id() <<
        std::string(depth_ * 2, ' ') << node->Representation() << std::endl;

    depth_++;
    node->AcceptChildren(*this);
    depth_--;
  }

private:
  unsigned int depth_;
};

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  Printer printer;
  ast->Accept(printer);

  return 0;
}
