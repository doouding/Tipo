cmake_minimum_required(VERSION 3.13)

project(tipo)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(. source_list)
add_executable(tipo src/main.c ${source_list})

target_link_libraries(tipo LINK_PUBLIC ${CHUNK_LIBRARY} ${MEMORY_LIBRARY})
