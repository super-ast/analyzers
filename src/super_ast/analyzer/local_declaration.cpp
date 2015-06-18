#include <iostream>
#include <vector>
#include "super_ast.hpp"
#include "error.hpp"
#include "visitor/liveness.hpp"
#include "visitor/local_declaration.hpp"

using namespace std;

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  super_ast::Liveness liveness;
  ast->Accept(liveness);

  super_ast::LocalDeclaration local_declaration(&liveness);
  ast->Accept(local_declaration);
  
  vector<Error> err = local_declaration.get_errors();
  for (Error e : err) {
    cout << "LINE: " << e.line_number << endl;
    cout << "FUNCTION: " << e.function << endl;
    cout << "ERROR: " << e.short_desc << " - " << e.long_desc << endl << endl;
  }

  return 0;
}
