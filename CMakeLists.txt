cmake_minimum_required(VERSION 3.30)
project(nue C)
set(CMAKE_C_STANDARD 17)

add_subdirectory(external/linenoise)

add_executable(nue main.c
        nue/token.h
        nue/tokeniser.h
        nue/tokeniser.c
)

target_include_directories(nue PRIVATE external/linenoise)
target_link_libraries(nue PRIVATE linenoise)