cmake_minimum_required(VERSION 3.22)
project(CLCS
        VERSION 0.5.3
        )

# 只有c++20的版本才可以编译通过
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE "Release")

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

# 使用了json库
find_package(nlohmann_json REQUIRED)
target_link_libraries(CLCS-client nlohmann_json::nlohmann_json)
target_link_libraries(CLCS-server nlohmann_json::nlohmann_json)
