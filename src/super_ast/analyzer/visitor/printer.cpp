#include "printer.hpp"

namespace super_ast {
Printer::Printer() : depth_(0) {
}

void Printer::Visit(const super_ast::Node* node) {
  std::cout << std::left << std::setw(5) << node->id() <<
  std::string(depth_ * 2, ' ') << node->Representation() << std::endl;

  depth_++;
  node->AcceptChildren(*this);
  depth_--;
}
}
