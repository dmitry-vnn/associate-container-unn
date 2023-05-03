cmake_minimum_required (VERSION 3.8)

set(AC associate-container)
set(AC_HEADER PUBLIC include)

file(GLOB_RECURSE AC_SOURCES src/*.c*)

add_library(${AC} STATIC ${AC_SOURCES})
target_include_directories(${AC} ${AC_HEADER})
