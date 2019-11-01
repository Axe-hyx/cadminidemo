cmake_minimum_required(VERSION 3.1)
project(caddemo)
add_executable(main
    src/main.cc
    src/Solids.cpp
    src/operator.cpp
    src/Face.cpp
)
set_property(TARGET main PROPERTY CXX_STANDARD 11)
