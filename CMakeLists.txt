cmake_minimum_required(VERSION 3.13)
project(tipo)

set(CMAKE_C_STANDARD 11)
aux_source_directory(src source_list)

add_executable(tipo ${source_list})
