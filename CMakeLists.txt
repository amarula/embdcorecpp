cmake_minimum_required(VERSION 3.14)

project(
  embdcorecpp
  VERSION 0.1
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests." OFF)

file(GLOB_RECURSE PROJECT_HEADERS include/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES source/*.cpp)
add_library(${PROJECT_NAME} SHARED ${PROJECT_HEADERS} ${PROJECT_SOURCES})
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:include>
  PRIVATE source)

target_compile_definitions(${PROJECT_NAME} PRIVATE EMBDCORECPP_LIBRARY)

if(BUILD_TESTS)
  enable_testing()

  # GoogleTest v1.12.1
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
  )
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)

  set(TEST_MAIN_NAME testmain)
  set(TEST_MAIN_SOURCES tests/main.cpp)
  file(GLOB_RECURSE TEST_HEADERS tests/*.hpp)

  add_executable(${TEST_MAIN_NAME} ${TEST_MAIN_SOURCES} ${TEST_HEADERS})
  target_link_libraries(${TEST_MAIN_NAME} PRIVATE gtest_main ${PROJECT_NAME})

  include(GoogleTest)
  gtest_discover_tests(${TEST_MAIN_NAME})
endif()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-config
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT ${PROJECT_NAME}-config DESTINATION cmake)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}-config.cmake)
