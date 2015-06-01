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
