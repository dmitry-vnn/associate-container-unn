
set(LIB_NAME efficiency-tester)
set(HEADERS PUBLIC include)

file(GLOB_RECURSE SOURCES src/*.c*)

file(GLOB_RECURSE EXECUTABLE_SOURCES executable_src/*.c*)

add_library(${LIB_NAME} STATIC ${SOURCES})
target_include_directories(${LIB_NAME} ${HEADERS})

target_link_libraries(
	${LIB_NAME} 

	"associate-container"
	"text-parser"
)


set(EXE_NAME "eff-start")

add_executable(${EXE_NAME} ${EXECUTABLE_SOURCES})
target_link_libraries(
	${EXE_NAME} 

	${LIB_NAME}	#itself
)
