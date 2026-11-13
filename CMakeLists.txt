cmake_minimum_required(VERSION 3.22)

project(ExportManager)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(Formatters INTERFACE src/formatters/Formatter.hpp src/formatters/Json.hpp src/formatters/XML.hpp src/formatters/AnyFormatter.hpp)
target_include_directories(Formatters INTERFACE src)

add_library(ExportManager STATIC src/ExportManager.cpp src/ExportManager.hpp)
target_include_directories(ExportManager PUBLIC src)
target_link_libraries(ExportManager PRIVATE Formatters)

add_executable(export-manager src/main.cpp)
target_link_libraries(export-manager PRIVATE Formatters)
target_link_libraries(export-manager PRIVATE ExportManager)