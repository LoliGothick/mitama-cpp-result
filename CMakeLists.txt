cmake_minimum_required(VERSION 3.1)
enable_language(CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "-std=c++2a -w -pedantic-errors -ftemplate-backtrace-limit=0")

## Set our project name
project(mitama-cpp-result)

# Find Boost
# Find Boost
find_package(Boost 1.67.0 REQUIRED
  COMPONENTS
  stacktrace_addr2line
)

include_directories(
  PUBLIC ${Boost_INCLUDE_DIRS}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Catch2/single_include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mitama-utest-utilities/include
  )

add_executable(result-tests test/Result_Test.cpp)
add_executable(match-tests test/Match_Test.cpp)
add_executable(maybe-tests test/Maybe_Test.cpp)

target_link_libraries(result-tests ${Boost_LIBRARIES} dl)
target_link_libraries(match-tests ${Boost_LIBRARIES} dl)
target_link_libraries(maybe-tests ${Boost_LIBRARIES} dl)

add_test(result-tests "${EXECUTABLE_OUTPUT_PATH}/result-tests")
add_test(match-tests "${EXECUTABLE_OUTPUT_PATH}/match-tests")
add_test(maybe-tests "${EXECUTABLE_OUTPUT_PATH}/maybe-tests")
enable_testing()
