#include <iostream>
#include <iomanip>
#include <set>
#include <map>
#include "super_ast.hpp"
#include "function_ids.hpp"

class CallGraph : public super_ast::Visitor {
public:
    CallGraph();
    void Visit(const super_ast::Node* node);
    void Visit(const super_ast::FunctionCall* function_call);
    void Visit(const super_ast::FunctionDeclaration* function_declaration);
    std::vector<std::vector<int>> get_graph();
    std::vector<int> get_num_calls();
    void set_ids(std::map<std::string, struct FunctionInfo> ids);

private:
    std::map<std::string, struct FunctionInfo> ids_;
    std::vector<std::vector<int>> call_graph_;
    std::map<int, std::set<int>> calls_made_;
    std::vector<int> num_calls_;
    int current_func_;
};
