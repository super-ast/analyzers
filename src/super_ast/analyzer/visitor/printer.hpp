#pragma once

#include <iostream>
#include <iomanip>
#include "super_ast.hpp"

namespace super_ast {
class Printer : public Visitor {
public:
  Printer();
  void Visit(const super_ast::Node* node);

private:
  unsigned int depth_;
};
}
