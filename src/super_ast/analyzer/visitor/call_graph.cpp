#include "call_graph.hpp"


CallGraph::CallGraph() : current_func_(0) {
}

void CallGraph::Visit(const super_ast::Node* node) {
    node->AcceptChildren(*this);
}

void CallGraph::Visit(const super_ast::FunctionDeclaration* function_declaration) {
    std::string signature = function_declaration->name();
    struct FunctionInfo finf = ids_[signature];
    current_func_ = finf.id;
    function_declaration->AcceptChildren(*this);
}

void CallGraph::Visit(const super_ast::FunctionCall* function_call) {
    std::string signature = function_call->name();

    struct FunctionInfo finf = ids_.at(signature);

    // check if function has been called before
    std::set<int> func_calls = calls_made_[current_func_];
    if (func_calls.find(finf.id) == func_calls.end()) {
        call_graph_[current_func_].push_back(finf.id); 
        calls_made_[current_func_].insert(finf.id);
    }
    
    // increase function call count
    ++num_calls_[current_func_];

    function_call->AcceptChildren(*this);
}

void CallGraph::set_ids(std::map<std::string, struct FunctionInfo> ids) {
    ids_ = ids;

    int size = ids.size();
    call_graph_.resize(size);
    num_calls_.resize(size);
}

std::vector<std::vector<int>> CallGraph::get_graph() {
    return call_graph_;
}

std::vector<int> CallGraph::get_num_calls() {
    return num_calls_;
}
