cmake_minimum_required(VERSION 3.22)
project(CLCS
        VERSION 0.1.0

        )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Release")

include_directories(include)

aux_source_directory(resource resources)
aux_source_directory(include includes)

add_executable(CLCS ${resources} ${includes})

find_library(pthread lib_pthread)
target_link_libraries(CLCS ${lib_pthread})
