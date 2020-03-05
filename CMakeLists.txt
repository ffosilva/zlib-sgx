cmake_minimum_required (VERSION 3.10.2)
project(zlib_sgx "C" "CXX")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(SGX REQUIRED)

set(COMPILE_EXAMPLES NO CACHE BOOL "Build examples")

message(STATUS "SGX_PATH: ${SGX_PATH}")
message(STATUS "SGX_MODE: ${SGX_MODE}")
message(STATUS "SGX_HW: ${SGX_HW}")

set(ZLIB_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/trusted/zlib-src)

add_subdirectory(trusted)

if(COMPILE_EXAMPLES)
  add_subdirectory(examples)
endif(COMPILE_EXAMPLES)
