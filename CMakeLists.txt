cmake_minimum_required(VERSION 3.22)
project(CLCS
        VERSION 0.2.1

        )

# 只有c++20的版本才可以编译通过
set(CMAKE_CXX_STANDARD 20 REQUIRED)

set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_GLOBAL_AUTOGEN_TARGET_NAME}/)

add_subdirectory(public)
add_subdirectory(CLCS-client)
add_subdirectory(CLCS-server)

target_link_libraries(CLCS-client ${lib_public_var})
target_link_libraries(CLCS-server ${lib_public_var})

find_library(lib_pthread pthread REQUIRED)
target_link_libraries(CLCS-client ${lib_pthread})
target_link_libraries(CLCS-server ${lib_pthread})

# 使用了openssl加密库
find_package(OpenSSL REQUIRED)
target_link_libraries(CLCS-client OpenSSL::Crypto)
target_link_libraries(CLCS-server OpenSSL::Crypto)

# 使用了libuuid库
find_library(libuuid uuid REQUIRED)
target_link_libraries(CLCS-client ${libuuid})
target_link_libraries(CLCS-server ${libuuid})