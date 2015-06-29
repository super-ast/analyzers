#include "super_ast.hpp"

namespace super_ast {
class FindIdentifier : public Visitor {
public:
  FindIdentifier(const std::string& name) : name_(name), found_(false) {

  }

  void Visit(const Node* node) {
    if (!found_) {
      node->AcceptChildren(*this);
    }
  }

  void Visit(const Identifier* identifier) {
    found_ = (identifier->value() == name_);
  }

  bool found() {
    return found_;
  }

private:
  std::string name_;
  bool found_;
};
}
