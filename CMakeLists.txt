cmake_minimum_required(VERSION 2.8.4)
project(analyzers)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(lib/base)

include_directories(
    ${SUPER_AST_BASE_INCLUDE_DIRS}
    vendor/rapidjson/include
)

# To make the binaries static
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#SET(BUILD_SHARED_LIBRARIES OFF)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")

# Override add_executable macro to link libraries automatically
macro (add_executable _name)
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} super_ast-base)
    endif()
endmacro()

add_executable(printer
  src/super_ast/analyzer/printer.cpp
  src/super_ast/analyzer/visitor/printer.cpp
)

add_executable(grapher
  src/super_ast/analyzer/grapher.cpp
)

add_executable(check
  src/super_ast/analyzer/check.cpp
  src/super_ast/analyzer/visitor/function_ids.cpp
  src/super_ast/analyzer/visitor/call_graph.cpp
  src/super_ast/analyzer/visitor/simple_fors.cpp
  src/super_ast/analyzer/visitor/liveness.cpp
  src/super_ast/analyzer/visitor/dead_assignation.cpp
  src/super_ast/analyzer/visitor/local_declaration.cpp
  src/super_ast/analyzer/visitor/variable_initialization.cpp
)

add_executable(simple_fors
  src/super_ast/analyzer/simple_fors.cpp
  src/super_ast/analyzer/visitor/simple_fors.cpp
)

add_executable(variable_initialization
  src/super_ast/analyzer/variable_initialization.cpp
  src/super_ast/analyzer/visitor/variable_initialization.cpp
)

add_executable(local_declaration
  src/super_ast/analyzer/local_declaration.cpp
  src/super_ast/analyzer/visitor/liveness.cpp
  src/super_ast/analyzer/visitor/local_declaration.cpp
)

add_executable(dead_assignation
  src/super_ast/analyzer/dead_assignation.cpp
  src/super_ast/analyzer/visitor/liveness.cpp
  src/super_ast/analyzer/visitor/dead_assignation.cpp
)
