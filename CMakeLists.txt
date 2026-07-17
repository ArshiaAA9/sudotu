# NOTE TO SELF:
# GLOB_RECURSE also picks up child directories
# GLOB only picks up the selected directory


cmake_minimum_required(VERSION 3.16)
project(sudotu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Critical for clangd
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

find_package(Catch2 3 REQUIRED)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9
  GIT_SHALLOW TRUE
  FIND_PACKAGE_ARGS NAMES ftxui
)
FetchContent_MakeAvailable(ftxui)

list(APPEND CMAKE_MODULE_PATH "${Catch2_DIR}")

# ==================== Suodku library ====================
file(GLOB SUDOKU_SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c")
list(REMOVE_ITEM SUDOKU_SOURCES "${SOURCE_DIR}/main.cpp")

add_library(libSudoku STATIC ${SUDOKU_SOURCES})

target_include_directories(libSudoku PUBLIC
    ${SOURCE_DIR}
)

# ==================== tui library ====================
file(GLOB_RECURSE TUI_SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/tui/*.cpp")
add_library(libTui STATIC ${TUI_SOURCES})

target_include_directories(libTui PUBLIC
    ${SOURCE_DIR}/tui
)

target_link_libraries(libTui PUBLIC
    libSudoku
    ftxui::screen
    ftxui::component
    ftxui::dom
)

# ==================== MAIN EXECUTABLE ====================
add_executable(sudotu
    ${SOURCE_DIR}/main.cpp
)

target_link_libraries(sudotu PRIVATE
    libSudoku
    libTui
)

target_compile_options(sudotu PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
# ==================== TESTS ====================
add_executable(tests
    ${CMAKE_SOURCE_DIR}/tests/test.cpp
)


target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    libSudoku
    ftxui::screen
    ftxui::dom
    ftxui::component
)

target_include_directories(tests PRIVATE
    ${SOURCE_DIR}
)
add_library(project_warnings INTERFACE)

target_compile_options(project_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(libSudoku PRIVATE project_warnings)
target_link_libraries(libTui PRIVATE project_warnings)
target_link_libraries(sudotu PRIVATE project_warnings)
target_link_libraries(tests PRIVATE project_warnings)

# Auto-discover tests with ctest
include(CTest)
include(Catch)
catch_discover_tests(tests)

