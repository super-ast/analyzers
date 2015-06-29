#include "constant_references.hpp"
#include "modified_variable.hpp"

namespace super_ast {
super_ast::ConstantReferences::ConstantReferences() {

}

void ConstantReferences::Visit(const FunctionDeclaration* function_declaration) {
  super::Visit(function_declaration);

  const std::vector<VariableDeclaration*>& parameters = function_declaration->parameters();

  std::vector<VariableDeclaration*> vector_parameters;
  FilterByType(parameters, {Type::STRING, Type::VECTOR, Type::STRUCT}, vector_parameters);

  for(VariableDeclaration* vector_parameter : vector_parameters) {
    if(vector_parameter->is_constant()) {
      if(!vector_parameter->is_reference()) {
        Report(*vector_parameter, "Constant parameter by value", "Parameter is constant but not passed by reference");
      }
    } else if(!ModifiedVariable::IsModified(&function_declaration->body(), vector_parameter->name())) {
        Report(*vector_parameter, "Parameter not constant", "Parameter could be constant because is not modified");
    }
  }
}

void ConstantReferences::FilterByType(const std::vector<VariableDeclaration*>& variables,
                                      const std::vector<Type::Native>& types,
                                      std::vector<VariableDeclaration*>& result) {
  for(VariableDeclaration* variable : variables) {
    for(Type::Native type : types) {
      if(variable->data_type().referenced_type().is(type)) {
        result.push_back(variable);
        return;
      }
    }
  }
}
}
