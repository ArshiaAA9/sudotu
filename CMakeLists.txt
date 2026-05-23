cmake_minimum_required(VERSION 3.16)
project(sudoku VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Critical for clangd
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

find_package(Catch2 3 REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${Catch2_DIR})

# ==================== GAME LIBRARY ====================

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c")
list(REMOVE_ITEM SOURCES "${SOURCE_DIR}/main.cpp")

add_library(libSudoku STATIC ${SOURCES})

target_include_directories(libSudoku PUBLIC
    ${SOURCE_DIR}
)

# ==================== MAIN EXECUTABLE ====================
add_executable(bin
    ${SOURCE_DIR}/main.cpp
)

target_link_libraries(bin PRIVATE
    libSudoku
)
# ==================== TESTS ====================
add_executable(tests
    ${CMAKE_SOURCE_DIR}/tests/test.cpp
)


target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    libSudoku
)

target_include_directories(tests PRIVATE
    ${SOURCE_DIR}
)

# Auto-discover tests with ctest
include(CTest)
include(Catch)
catch_discover_tests(tests)
