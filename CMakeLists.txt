cmake_minimum_required(VERSION 3.20)
project(22s_final_proj)
if (CMAKE_BUILD_TYPE MATCHES release)
    add_compile_options("-O3")
    add_compile_options("-march=native")
    add_compile_options("-flto")
endif ()

if (CMAKE_BUILD_TYPE MATCHES debug)
    add_compile_options("-wall")
    add_compile_options("-fanalyzer")
endif ()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -pthread)
add_executable(22s_final_proj main.cpp catch_setup.cpp Query.cpp Query.h Parser.cpp Parser.h Article.h thread_pool.h porter2_stemmer.cpp porter2_stemmer.h util/hash.h util/string_view.h AvlTree.h Pair.h)
