set(CMAKE_CXX_STANDARD 20)

#======================= ADD GOOOGLE TEST LIB SUPPORT ========================

include(FetchContent) #CMake module FetchContent (integrated)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# ========================== CREATE TESTS EXECUTABLE =========================
file(GLOB_RECURSE TEST_FILES src/*.cpp)	#TEST_FOLDER var

enable_testing()

set(EXE_NAME "tests-start")

add_executable(
	${EXE_NAME}								#Name of executable file
											#Add source files (test files)
	${TEST_FILES}
)

target_link_libraries(						#Connect libraries
  ${EXE_NAME}							

  GTest::gtest_main							#Google test library
  associate-container						#Associate container lib
  text-parser						        #Text parser lib
)

include(GoogleTest)							#Include GoogleTest module
gtest_discover_tests(${EXE_NAME})			#
