cmake_minimum_required(VERSION 3.10)
project(grounder)

set(CMAKE_CXX_STANDARD 17)

# Release
set(CMAKE_CXX_FLAGS "-O0 -g -DNDEBUG -fomit-frame-pointer")

# Debug and Profile
#set(CMAKE_CXX_FLAGS "-O0 -D_GLIBCXX_DEBUG -pg")


add_executable(grounder
        main.cc
        logic_program.cc logic_program.h
        parser.cc parser.h
        fact.cc fact.h
        object.cc object.h
        rules.cc rules.h
        atom.cc atom.h
        grounders/grounder_factory.h
        grounders/grounder.h
        grounders/fast_downward_grounder.cc grounders/fast_downward_grounder.h)