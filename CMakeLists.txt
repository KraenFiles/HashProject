cmake_minimum_required(VERSION 3.10)
project (HashProject VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if(NOT DEFINED TABLE_SIZE)
  set(TABLE_SIZE 2500)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Release mode")
  set(RELEASE 1)
  set(TEST_MODE 0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug mode")
  set(RELEASE 0)
  set(TEST_MODE 0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
  message("Test mode")
  set(RELEASE 0)
  set(TEST_MODE 1)
else()
  message("Default mode")
  set(RELEASE 0)
  set(TEST_MODE 0)
endif()
      

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
  "include/**.h"
  "include/**.hpp"
  "src/**.cpp"
  "src/**.c"
)
file(GLOB_RECURSE TESTS RELATIVE ${CMAKE_SOURCE_DIR}
"tests/**.h"
"tests/**.hpp"
"tests/**.cpp"
)

include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY "https://github.com/google/googletest"
  GIT_TAG "main"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

include(GoogleTest)
include(Dart)

enable_testing()
add_executable(HashProject ${SOURCES} ${TESTS})
target_link_libraries(HashProject GTest::gtest_main)

configure_file( cmake/Config.h.in ${CMAKE_SOURCE_DIR}/include/Config.h @ONLY )

if(RELEASE EQUAL 1)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wextra -Wpedantic -Wall")
  add_definitions(-DRELEASE)
else()
  add_definitions(-DDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wextra -Wpedantic -Wall")
  if(TEST_MODE EQUAL 1)
  set(CTEST_OUTPUT_ON_FAILURE ON)
  gtest_add_tests(TARGET  HashProject
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
  )
  endif()
endif()
      
include_directories(include)
set(SOURCE_DIR src)
set(BUILD_DIR build)
set_target_properties(HashProject PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
