cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_PARALLEL_LEVEL 20)

project(d2tweaks)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sources
        "src/*.cpp"
        "include/*.hpp"
        "src/*.cc"
        "include/*.h"
        "tests/*.cpp")

add_library(d2tweaks SHARED ${sources})

set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")
set(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /DNDEBUG")

set(D2_BASE_DIR D:/Games/Diablo\ II/)

add_compile_definitions(WIN32_LEAN_AND_MEAN)

target_compile_options(d2tweaks PRIVATE /wd4068)
target_compile_options(d2tweaks PRIVATE /wd4293)
target_compile_options(d2tweaks PRIVATE /wd4996)

target_include_directories(d2tweaks PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(d2tweaks PRIVATE wsock32 ws2_32)

#MinHook
add_subdirectory(vendor/minhook)
target_link_libraries(d2tweaks PRIVATE minhook)

#spdlog
find_package(spdlog CONFIG REQUIRED)
set_target_properties(spdlog::spdlog PROPERTIES
	MAP_IMPORTED_CONFIG_MINSIZEREL Release
	MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)
set_target_properties(fmt::fmt PROPERTIES
	MAP_IMPORTED_CONFIG_MINSIZEREL Release
	MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)
set_target_properties(fmt::fmt-header-only PROPERTIES
	MAP_IMPORTED_CONFIG_MINSIZEREL Release
	MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)
target_link_libraries(d2tweaks PRIVATE spdlog::spdlog)

#pugixml
find_package(pugixml CONFIG REQUIRED)
set_target_properties(pugixml PROPERTIES
	MAP_IMPORTED_CONFIG_MINSIZEREL Release
	MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)
target_link_libraries(d2tweaks PRIVATE pugixml)

#json
find_package(nlohmann_json CONFIG REQUIRED)
set_target_properties(nlohmann_json PROPERTIES
	MAP_IMPORTED_CONFIG_MINSIZEREL Release
	MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)
target_link_libraries(d2tweaks PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

set_target_properties(d2tweaks PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${D2_BASE_DIR}/MODS/test/)
#set_target_properties(d2tweaks PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${D2_BASE_DIR}/MODS/Zy-El\ v.4.5.1/)