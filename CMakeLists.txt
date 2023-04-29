cmake_minimum_required (VERSION 3.8)

project ("associate-container-unn")

add_subdirectory ("associate-container")

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
file(GLOB_RECURSE TESTS_FOLDER tests/*.cpp)

enable_testing()

add_executable(
	run_all_tests							#Name of executable file
											#Add source files (test files)
	${TESTS_FOLDER}
 "tests/hash_tests.cpp")

target_link_libraries(						#Connect libraries
  run_all_tests								

  GTest::gtest_main							#Google test library
  associate-container						#Associate container headers
)

include(GoogleTest)							#Include GoogleTest module
gtest_discover_tests(run_all_tests)			#