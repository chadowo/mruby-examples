cmake_minimum_required(VERSION 3.10...3.22)

project(mruby-examples
        DESCRIPTION "Collection of sample C programs for MRuby"
        LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(WARNINGS -pedantic -Wall -Wshadow -Wpointer-arith
             -Wno-missing-braces -Wcast-qual -Wstrict-prototypes)

find_package(SDL2 REQUIRED)

include("src/CMakeLists.txt")
