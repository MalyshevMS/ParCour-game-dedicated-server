cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(project_name_ Parcour-game-server)

project(${project_name_})

add_executable(${project_name_} main.cpp)

target_compile_features(${project_name_} PUBLIC cxx_std_17)

set_target_properties(${project_name_} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)