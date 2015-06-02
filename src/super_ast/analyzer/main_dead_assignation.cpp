#include <iostream>
#include <vector>
#include "super_ast.hpp"
#include "error.hpp"
#include "liveness.hpp"
#include "dead_assignation.hpp"

using namespace std;

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  super_ast::Liveness liveness;
  ast->Accept(liveness);

  super_ast::DeadAssignation dead_assignation(&liveness);
  ast->Accept(dead_assignation);
  
  vector<Error> err = dead_assignation.get_errors();
  for (Error e : err) {
    cout << "LINE: " << e.line_number << endl;
    cout << "FUNCTION: " << e.function << endl;
    cout << "ERROR: " << e.short_desc << " - " << e.long_desc << endl << endl;
  }

  return 0;
}
