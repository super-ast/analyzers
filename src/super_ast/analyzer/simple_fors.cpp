#include "visitor/simple_fors.hpp"

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  super_ast::SimpleFors simple_fors;
  ast->Accept(simple_fors);

  std::vector<Error> errors = simple_fors.GetErrors();
  for (const Error& error : errors) {
    std::cout << "line:" << error.line_number << 
        " - function: " << error.function <<
        " - short description: " << error.short_desc <<
        " - long description: " << error.long_desc << std::endl;
  }

  return 0;
}
