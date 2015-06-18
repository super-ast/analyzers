#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

#include "visitor/recursivity.cpp"
#include "visitor/simple_fors.hpp"
#include "visitor/liveness.hpp"
#include "visitor/dead_assignation.hpp"
#include "visitor/local_declaration.hpp"
#include "visitor/variable_initialization.hpp"
#include "rapidjson/document.h"

namespace {
typedef std::vector<Error> Errors;
typedef Errors (*Checker)(const super_ast::Block*, const std::string& argument);

struct Option {
  static const bool WITH_ARGUMENT = true;
  static const bool NO_ARGUMENT = false;

  std::string name;
  Checker checker;
  std::vector<std::string> description;
  bool with_argument;

  Option(const std::string& name, Checker checker, bool with_argument, const std::vector<std::string>& description) :
      name(name), description(description), checker(checker), with_argument(with_argument)
  { }

  Option(const std::string& name, Checker checker, const std::vector<std::string>& description) :
      Option(name, checker, NO_ARGUMENT, description)
  { }
};

struct Validator {
  Option* option;
  std::string argument;

  Validator(Option* option, const std::string& argument) : option(option), argument(argument)
  { }
};

/**
 * CHECKERS
 */
// Recursivity
Errors check_recursivity(const super_ast::Block* ast, const std::string& argument) {
  std::stringstream s(argument);
  std::string signature;
  int depth, base_cases;
  std::vector<RecursivityCheck> rec_checks;

  while (s >> signature >> depth >> base_cases) {
    RecursivityCheck rc(signature, depth, base_cases);
    rec_checks.push_back(rc);
  }

  Recursivity r;
  return r.check(ast, rec_checks);
}

// Simple fors
Errors check_simple_fors(const super_ast::Block* ast, const std::string& argument) {
  super_ast::SimpleFors simple_fors;
  ast->Accept(simple_fors);

  return simple_fors.GetErrors();
}

// Dead assignation
Errors check_dead_assign(const super_ast::Block* ast, const std::string& argument) {
  super_ast::Liveness liveness;
  ast->Accept(liveness);

  super_ast::DeadAssignation dead_assignation(&liveness);
  ast->Accept(dead_assignation);

  return dead_assignation.get_errors();
}

// Local declarations
Errors check_local_decl(const super_ast::Block* ast, const std::string& argument) {
  super_ast::Liveness liveness;
  ast->Accept(liveness);

  super_ast::LocalDeclaration local_declaration(&liveness);
  ast->Accept(local_declaration);

  return local_declaration.get_errors();
}

// Variable initialization
Errors check_variable_init(const super_ast::Block* ast, const std::string& argument) {
  super_ast::VariableInitialization var_ini;
  ast->Accept(var_ini);

  return var_ini.get_errors();
}


/**
 * OPTIONS
 */
std::map<std::string, Option> options = {
    {"--recursivity",   Option("recursivity", (Checker) check_recursivity, Option::WITH_ARGUMENT, {
        "runs recursivity check, input is a string of the form (func_signature depth base_cases)+.",
        "example: fibonacci 1 2",
        "depth 0 is the min"
    })},
    {"--fors",          Option("simple fors", (Checker) check_simple_fors, {"checks simple for conditions"})},
    {"--dead-assign",   Option("dead assignations", (Checker) check_dead_assign, {"checks dead assignations"})},
    {"--local-decl",    Option("local declarations", (Checker) check_local_decl, {"checks local declarations"})},
    {"--variable-init", Option("variable initialization", (Checker) check_variable_init,
                               {"checks variable initalization errors"})}
};


/**
 * HELP
 */
void help() {
  std::cout << "check" << " " << "<file> [args]" << std::endl;
  std::cout << "  Reads an AST in JSON format from file and runs the checks specified in args." << std::endl;
  std::cout << std::endl;
  std::cout << "  Available arguments:" << std::endl;

  for (std::map<std::string, Option>::const_iterator it = options.begin(); it != options.end(); ++it) {
    std::cout << "    " << it->first << std::endl;

    for (const std::string& description_line : it->second.description) {
      std::cout << "      " << description_line << std::endl;
    }

    std::cout << std::endl;
  }
}

/**
 * OPTION PARSER
 */
std::vector<Validator> parse_options(char** argv, int argc) {
  if (argc < 3) {
    help();
    exit(1);
  }

  std::vector<Validator> validators;
  std::vector<std::string> args(argv, argv + argc);

  int i = 2;
  while(i < args.size()) {
    if (args[i] == "--help") {
      help();
      exit(0);
    } else {
      const std::map<std::string, Option>::iterator it = options.find(args[i]);

      if (it != options.end()) {
        std::string argument = "";

        if (it->second.with_argument) {
          ++i;
          argument = args[i];
        }

        validators.push_back(Validator(&it->second, argument));
      } else {
        help();
        exit(1);
      }
    }

    ++i;
  }

  return validators;
}
}

rapidjson::Value error_to_json(const Error& e, rapidjson::Document doc) {
  rapidjson::Value v(rapidjson::kObjectType);
  rapidjson::Value line(e.line_number);
  v.AddMember("line", line, doc.GetAllocator());

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

void append_errors(std::string key, rapidjson::Value& v, std::vector<Error> errors, rapidjson::Document doc) {
  rapidjson::Value k;
  auto k_str = key.c_str();
  k.SetString(rapidjson::StringRef(k_str));
  rapidjson::Value a(rapidjson::kArrayType);
  rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
  for (Error e : errors) {
    //rapidjson::Value e_k = error_to_json(e,doc);
    //a.PushBack(error_to_json(e,doc), allocator);
  }
  v.AddMember(k, a, allocator);
}

void print_errors(std::string key, const Errors& errors) {
  std::cout << key << std::endl;
  for (const Error& e : errors) {
    std::cout << "  LINE: " << e.line_number << std::endl;
    std::cout << "  FUNCTION: " << e.function << std::endl;
    std::cout << "  ERROR: " << e.short_desc << " - " << e.long_desc << std::endl << std::endl;
  }
}

int main(int argc, char** argv) {
  const std::vector<Validator>& validators = parse_options(argv, argc);

  std::ifstream in_file;
  in_file.open(argv[1]);

  const super_ast::Block* ast = super_ast::Parse(in_file);

  for(const Validator& validator : validators) {
    print_errors(validator.option->name, validator.option->checker(ast, validator.argument));
  }

  return 0;
}
