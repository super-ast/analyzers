#include <iostream>
#include <iomanip>
#include "super_ast.hpp"

#ifndef _FUNCID_H
#define _FUNCID_H
struct FunctionInfo {
    int id;
    int line;
    FunctionInfo() : id(-1), line(-1) {}
    FunctionInfo(int id, int line) : id(id), line(line) {}
};

class FunctionIds : public super_ast::Visitor {
public:
    FunctionIds();
    void Visit(const super_ast::Node* node);
    void Visit(const super_ast::FunctionDeclaration* function_declaration);
    void Visit(const super_ast::FunctionCall* function_call);
    std::map<std::string, struct FunctionInfo> get_ids();

private:
    std::map<std::string, struct FunctionInfo> ids_;
    int next_id_;
};

#endif
