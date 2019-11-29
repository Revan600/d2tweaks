cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_PARALLEL_LEVEL 20)

project(d2_item_mover)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sources
        "src/*.cpp"
        "include/*.hpp"
        "src/*.cc"
        "include/*.h"
        "tests/*.cpp")

add_library(d2_item_mover SHARED ${sources})

set(D2_BASE_DIR D:/Games/Diablo\ II/)

add_compile_definitions(WIN32_LEAN_AND_MEAN)

target_compile_options(d2_item_mover PRIVATE /wd4068)
target_compile_options(d2_item_mover PRIVATE /wd4293)
target_compile_options(d2_item_mover PRIVATE /wd4996)

target_include_directories(d2_item_mover PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(d2_item_mover PRIVATE wsock32 ws2_32)

#MinHook
add_subdirectory(vendor/minhook)
target_link_libraries(d2_item_mover PRIVATE minhook)

#spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(d2_item_mover PRIVATE spdlog::spdlog)

#fmt
find_package(fmt CONFIG REQUIRED)
target_link_libraries(d2_item_mover PRIVATE fmt::fmt fmt::fmt-header-only)

set_target_properties(d2_item_mover PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${D2_BASE_DIR}/MODS/Zy-El\ v.4.5.1/)