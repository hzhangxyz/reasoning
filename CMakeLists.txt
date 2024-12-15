# 因为我的VS中cmake的版本是这个，所以暂时设置为这个先
cmake_minimum_required(VERSION 3.29.5 FATAL_ERROR)
# 因为目前Linux平台只有Clang支持，所以暂时将使用Clang硬编码
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake)
# 这个神奇的字符串不知道是什么，但是要import std似乎必须加这个
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
project(reasoning CXX)
# 开启C++的模块功能
set(CMAKE_CXX_MODULE_STD ON)
# 设置C++标准，目前测试Linux上使用Clang必须打开GNU扩展
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
# 导出编译信息以供一些编辑器使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)
add_library(reasoning)
target_sources(reasoning PUBLIC FILE_SET CXX_MODULES FILES ${LIB_SRC})

find_package(GTest REQUIRED)
enable_testing()
file(GLOB TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cxx)
foreach(FILE ${TEST_SRC})
  get_filename_component(NAME ${FILE} NAME_WE)
  add_executable(${NAME} ${FILE})
  target_link_libraries(${NAME} reasoning)
  add_test(${NAME} ${NAME})
endforeach()
