#include "super_ast.hpp"
#include "visitor/printer.hpp"

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  super_ast::Printer printer;
  ast->Accept(printer);

  return 0;
}
