#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include "recursivity.cpp"
#include "simple_fors.hpp"
#include "rapidjson/document.h"

void help() {
    std::cout << "check" << " " << "in_file out_file [args]\n";
    std::cout << "--test\n" << "\truns all tests\n";
    std::cout << "--recursivity\n" << "\truns recursivity check, input is a string of the form" <<
                 " (func_signature depth base_cases)+\n\texample: fibonacci 1 2\n\tdepth 0 is the min\n";
    std::cout << "--fors\n\tchecks for simple for conditions\n";

}

rapidjson::Value error_to_json(const Error &e, rapidjson::Document doc) {
    rapidjson::Value v(rapidjson::kObjectType);
    rapidjson::Value line(e.line_number); v.AddMember("line", line, doc.GetAllocator());
    
    rapidjson::Value func; 
    auto c_func = e.function.c_str();
    func.SetString(rapidjson::StringRef(c_func));
    v.AddMember("function", func, doc.GetAllocator());

    rapidjson::Value short_d; 
    auto s_desc = e.short_desc.c_str();
    short_d.SetString(rapidjson::StringRef(s_desc));
    v.AddMember("short description", short_d, doc.GetAllocator());

    rapidjson::Value long_d;
    auto l_desc = e.long_desc.c_str();
    long_d.SetString(rapidjson::StringRef(l_desc));
    v.AddMember("long description", long_d, doc.GetAllocator());
    return v;
}

void append_errors(std::string key, rapidjson::Value &v, std::vector<Error> errors, rapidjson::Document doc) {
    rapidjson::Value k;
    auto k_str = key.c_str();
    k.SetString(rapidjson::StringRef(k_str));
    rapidjson::Value a(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    for (Error e : errors) {
        //rapidjson::Value e_k = error_to_json(e,doc);
        //a.PushBack(error_to_json(e,doc), allocator);
    }
    v.AddMember(k,a, allocator);
}

void print_errors(std::string key, std::vector<Error> errors) {
    std::cout << key << std::endl;
    for (Error e : errors) {
        std::cout << "\tLINE: " << e.line_number << std::endl;
        std::cout << "\tFUNCTION: " << e.function << std::endl;
        std::cout << "\tERROR: " << e.short_desc << " - " << e.long_desc << std::endl << std::endl;
      }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        help();
        exit(1);
    }
    std::vector<std::string> args(argv, argv+argc);

    std::ifstream in_file;
    in_file.open(argv[1]);
    const super_ast::Block* ast = super_ast::Parse(in_file);
    
    rapidjson::Document document;
    for (int i = 0; i < argc; ++i) {
        if (args[i] == "--test") {
            //run tests
        } else if (args[i] == "--recursivity") {
            std::stringstream s(args[i+1]);
            std::string signature, depth, base_cases;
            std::vector<struct RecursivityCheck> rec_checks;
            while (s >> signature >> depth >> base_cases) {
                struct RecursivityCheck rc(signature,atoi(depth.c_str()),atoi(base_cases.c_str()));
                rec_checks.push_back(rc);
            }
            
            Recursivity r;
            std::vector<Error> rec_err = r.check(ast, rec_checks);
            print_errors("recursivity", rec_err);
            
        } else if (args[i] == "--fors") {
            super_ast::SimpleFors simple_fors;
            ast->Accept(simple_fors);
            std::vector<Error> errors = simple_fors.GetErrors();
            print_errors("simple fors", errors);

        } else if (args[i] == "--conditionals") {
            

        } else if (args[i] == "--help") {
            help();
            exit(0);

        } else if (args[i] == "--conditionals") {


        }
    }



    // In varlife entry

    // In Dumb return
    
    // In for check

    // In const check
    
    // In ???
}
