#
# BloomFilter/tests/CMakeLists.txt
#

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (BINARY ${CMAKE_PROJECT_NAME}_tests)

file (GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.hpp *.c *.cpp)
set (SOURCES ${TEST_SOURCES})

include_directories (${PROJECT_SOURCE_DIR}/include)

add_executable(${BINARY} ${TEST_SOURCES})
