cmake_minimum_required(VERSION 2.8.4)
project(analyzers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(lib/base)

include_directories(
    ${SUPER_AST_BASE_INCLUDE_DIRS}
)

set(SOURCE_FILES
    src/super_ast/analyzer/error.cpp
)

# Override add_executable macro to link libraries automatically
macro (add_executable _name)
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} super_ast-base)
    endif()
endmacro()

add_executable(printer src/super_ast/analyzer/printer.cpp)
add_executable(grapher src/super_ast/analyzer/grapher.cpp)
add_executable(useless_conditionals src/super_ast/analyzer/useless_conditionals.cpp)
add_executable(const_ref_vectors src/super_ast/analyzer/const_ref_vectors.cpp)
add_executable(simple_fors src/super_ast/analyzer/main_simple_fors.cpp
  # dependencies
  src/super_ast/analyzer/simple_fors.cpp
)
add_executable(variable_initialization src/super_ast/analyzer/main_variable_initialization.cpp
  # dependencies
  src/super_ast/analyzer/variable_initialization.cpp
)
add_executable(local_declaration src/super_ast/analyzer/main_local_declaration.cpp
  # dependencies
  src/super_ast/analyzer/liveness.cpp
  src/super_ast/analyzer/local_declaration.cpp
)
add_executable(dead_assignation src/super_ast/analyzer/main_dead_assignation.cpp
  # dependencies
  src/super_ast/analyzer/liveness.cpp
  src/super_ast/analyzer/dead_assignation.cpp
)
