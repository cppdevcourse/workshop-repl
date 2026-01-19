cmake_minimum_required(VERSION 3.10)
project(repl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

set(HEADERS
  command.hpp
  argument_parsers.hpp
  repl.hpp
)

add_executable(calculator)
target_sources(calculator PRIVATE
  ${HEADERS}
  calculator.cpp
)

add_executable(strings)
target_sources(strings PRIVATE
  ${HEADERS}
  strings.cpp
)
