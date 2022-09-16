[![CMake](https://github.com/amarula/embdcorecpp/actions/workflows/cmake.yml/badge.svg)](https://github.com/amarula/embdcorecpp/actions/workflows/cmake.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# embdcorecpp

embdcorecpp is a library containing usefull classes and logics usually used in embedded C++ applications.

## Build

Building the library is pretty easy. Configure and build the project with CMake:
```bash
cmake -S . -B build
cmake --build build
```

If you want to run the test just enable BUILD_TESTS option:
```bash
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
```

## Usage in CMake

Just add the following snippet and update the sha to the version you prefer:
```cmake
find_package(embdcorecpp QUIET)
if(embdcorecpp_FOUND)
  message(STATUS "embdcorecpp version: ${embdcorecpp_VERSION}")
else()
  FetchContent_Declare(
    embdcorecpp
    URL https://github.com/amarula/embdcorecpp/archive/298db96175b039a9558c505f6072d7e2a6694d47.zip
  )
  FetchContent_MakeAvailable(embdcorecpp)
  message(STATUS "Downloaded embdcorecpp")
endif()
target_link_libraries(${PROJECT_NAME} embdcorecpp)
```