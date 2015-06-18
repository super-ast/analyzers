#include <iostream>
#include <vector>
#include "super_ast.hpp"
#include "error.hpp"
#include "visitor/variable_initialization.hpp"

using namespace std;

int main() {
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  super_ast::VariableInitialization var_ini;
  ast->Accept(var_ini);

  vector<Error> err = var_ini.get_errors();
  for (Error e : err) {
    cout << "LINE: " << e.line_number << endl;
    cout << "FUNCTION: " << e.function << endl;
    cout << "ERROR: " << e.short_desc << " - " << e.long_desc << endl << endl;
  }

  return 0;
}
