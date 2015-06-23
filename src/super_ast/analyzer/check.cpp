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
#include "../../../lib/base/vendor/rapidjson/include/rapidjson/writer.h"

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
    {"--fors",          Option("simple_fors", (Checker) check_simple_fors, {"checks simple for conditions"})},
    {"--dead-assign",   Option("dead_assignations", (Checker) check_dead_assign, {"checks dead assignations"})},
    {"--local-decl",    Option("local_declarations", (Checker) check_local_decl, {"checks local declarations"})},
    {"--variable-init", Option("variable_initialization", (Checker) check_variable_init,
                               {"checks variable initalization errors"})}
};


/**
 * HELP
 */
void help() {
  std::cout << "check" << " " << "[args]" << std::endl;
  std::cout << "  Reads an AST in JSON format from stdin and runs the checks specified in args." << std::endl;
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
  if (argc < 2) {
    help();
    exit(1);
  }

  std::vector<Validator> validators;
  std::vector<std::string> args(argv, argv + argc);

  int i = 1;
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

rapidjson::Value error_to_json(const Error& e, rapidjson::Document::AllocatorType& allocator) {
  rapidjson::Value v(rapidjson::kObjectType);

  rapidjson::Value line(e.line_number);
  v.AddMember("line", line, allocator);

  rapidjson::Value func(e.function.c_str(), allocator);
  v.AddMember("function", func, allocator);

  rapidjson::Value short_d(e.short_desc.c_str(), allocator);
  v.AddMember("short_description", short_d, allocator);

  rapidjson::Value long_d(e.long_desc.c_str(), allocator);
  v.AddMember("long_description", long_d, allocator);

  return v;
}

void append_errors(rapidjson::Value& main, std::string errors_key, const std::vector<Error>& errors,
                   rapidjson::Document::AllocatorType& allocator)
{
  rapidjson::Value json_errors(rapidjson::kArrayType);

  for (Error e : errors) {
    json_errors.PushBack(error_to_json(e, allocator), allocator);
  }

  rapidjson::Value key(errors_key.c_str(), allocator);
  main.AddMember(key, json_errors, allocator);
}

void print_json(const rapidjson::Value& json) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

  json.Accept(writer);

  std::cout << buffer.GetString() << std::endl;
}

int main(int argc, char** argv) {
  const std::vector<Validator>& validators = parse_options(argv, argc);
  const super_ast::Block* ast = super_ast::Parse(std::cin);

  rapidjson::Document json;
  rapidjson::Document::AllocatorType& allocator = json.GetAllocator();

  rapidjson::Value main(rapidjson::kObjectType);

  for(const Validator& validator : validators) {
    append_errors(main, validator.option->name, validator.option->checker(ast, validator.argument), allocator);
  }

  print_json(main);

  return 0;
}
