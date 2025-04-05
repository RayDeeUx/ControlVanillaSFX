cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
	set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
	set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(ControlVanillaSFX VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    # Add your cpp files here
)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    target_sources(${PROJECT_NAME} PRIVATE src/main.mm)
    set_source_files_properties(src/main.mm PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
