cmake_minimum_required(VERSION 3.22)
project(CLCS
        VERSION 0.2.1

        )

# 只有c++20的版本才可以编译通过
set(CMAKE_CXX_STANDARD 20 REQUIRED)

set(CMAKE_BUILD_TYPE "Release")

# 添加头文件
include_directories(include)

aux_source_directory(resource resources)
aux_source_directory(include includes)

add_executable(CLCS ${resources} ${includes})

find_library(lib_pthread pthread REQUIRED)
target_link_libraries(CLCS ${lib_pthread})

# 使用了openssl加密库
find_package(OpenSSL REQUIRED)
target_link_libraries(CLCS OpenSSL::Crypto)

# 使用了libuuid库
find_library(libuuid uuid REQUIRED)
target_link_libraries(CLCS ${libuuid})

