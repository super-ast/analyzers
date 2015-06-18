#include "function_ids.hpp"

FunctionIds::FunctionIds() : next_id_(0) {}

void FunctionIds::Visit(const super_ast::Node* node) {
    node->AcceptChildren(*this);
}

void FunctionIds::Visit(const super_ast::FunctionDeclaration* function_declaration) {
    std::string signature = function_declaration->name();
    if (ids_.find(signature) != ids_.end()) {
        std::cout << "Duplicate function signature: " << signature << std::endl;
        return;
    } else {
        
        ids_[signature] = FunctionInfo(next_id_++,
                                       function_declaration->line());
    }
    function_declaration->AcceptChildren(*this);
}

void FunctionIds::Visit(const super_ast::FunctionCall* function_call) {
    std::string signature = function_call->name();
    if (ids_.find(signature) != ids_.end()) {
        //std::cout << "Duplicate function signature: " << signature << std::endl;
        return;
    } else {
        
        ids_[signature] = FunctionInfo(next_id_++,
                                       function_call->line());
    }
    function_call->AcceptChildren(*this);
}


std::map<std::string, struct FunctionInfo> FunctionIds::get_ids() {
    return ids_;
}
