cmake_minimum_required (VERSION 3.8)

set(TP text-parser)
set(TP_HEADER PUBLIC include)

file(GLOB_RECURSE TP_SOURCES src/*.c*)

add_library(${TP} STATIC ${TP_SOURCES})
target_include_directories(${TP} ${TP_HEADER})
