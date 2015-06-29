/*
 * Checks for a simple for.
 * That is, it needs to initialize a variable which is used in the post
 * iteration. The condition must not be too cumplex: it cannot use && or ||.
 * The post iteration should be a increment of a constant value, ie
 * ++i, i--, i += k, i = i - k;
 */
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include "super_ast.hpp"
#include "../error.hpp"
#include "modified_variable.hpp"

namespace super_ast {
class SimpleFors : public Visitor {
public:
  void Visit(const super_ast::Node* node);
  std::vector<Error> GetErrors();

private:
  void Visit(const For* forstmt);

  // Is a simple condition, without OR and ANDs
  bool IsSimpleCondition(const Expression* expr);

  bool IsValidBinaryOperator(const BinaryOperator* bop);

  // Check the post loop with the form: initVar = X
  // This could be: initVar = initVar +- k
  void CheckAssign(const Expression* expr, const std::string& initName,
      const Block* body);

  // Check the post loop with the form: initVar += K
  void CheckAssignSum(const Expression* expr, const std::string& initName,
      const Block* body);

  // Returns true if the value node is a literal or a variable not
  // modified in the body block.
  bool IsSimpleConstValue(const Node* value, const Block* body);

  void Report(const Node& node, const std::string short_message, 
      const std::string long_message);

  std::vector<Error> errors_;
};
}

