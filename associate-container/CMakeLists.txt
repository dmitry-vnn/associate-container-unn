cmake_minimum_required (VERSION 3.8)

set(AC associate-container)
set(AC_HEADER PUBLIC include)

add_library(${AC} STATIC src/calculator.cpp)
target_include_directories(${AC} ${AC_HEADER})
