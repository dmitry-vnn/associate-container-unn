
set(LIB_NAME associate-container)
set(HEADERS PUBLIC include)

file(GLOB_RECURSE SOURCES src/*.c*)

add_library(${LIB_NAME} STATIC ${SOURCES})
target_include_directories(${LIB_NAME} ${HEADERS})
