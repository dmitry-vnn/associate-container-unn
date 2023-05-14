set(CMAKE_CXX_STANDARD 11)


set(HEADERS PUBLIC include)
file(GLOB_RECURSE SOURCES src/*.c*)

set(EXE_NAME "test-avl")

add_executable(${EXE_NAME} ${SOURCES})
target_include_directories(
	${EXE_NAME} 

	${HEADERS}
)
