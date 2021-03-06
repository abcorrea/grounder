cmake_minimum_required(VERSION 3.10)
project(grounder)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug Release)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wnon-virtual-dtor")

# Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -DNDEBUG -fomit-frame-pointer")

# Debug and Profile
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -D_GLIBCXX_DEBUG -pg")

add_executable(grounder
        main.cc
        logic_program.cc logic_program.h
        parser.cc parser.h
        fact.cc fact.h
        object.cc object.h
        rules/rule_base.cc rules/rule_base.h
        atom.cc atom.h
        grounders/grounder_factory.h
        grounders/grounder.h
        grounders/fast_downward_grounder.cc grounders/fast_downward_grounder.h
        rule_matcher.cc rule_matcher.h
        term.h arguments.h rules/join.h rules/project.h rules/product.h
        utils/language.h
        utils/system.cc utils/system.h
        utils/system_unix.cc utils/system_unix.h
        utils/system_windows.cc utils/system_windows.h grounders/new_grounder.cc grounders/new_grounder.h utils/scc.h utils/scc.cc)
