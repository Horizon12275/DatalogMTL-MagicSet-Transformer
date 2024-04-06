cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(DMT)

# add source files
set(SOURCES 
    main.cpp
    lib/atom.cpp
    lib/interval.cpp
    lib/literal.cpp
    lib/rule.cpp
    lib/term.cpp
)

# add include directories
include_directories(classes)
include_directories(utils)

# generate exe
add_executable(DMT ${SOURCES})

