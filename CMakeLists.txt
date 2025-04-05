cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

project(NikiScript VERSION 0.0.1 LANGUAGES C)

option(BUILD_64BITS "x64 or x86" ON)
option(BUILD_SHARED "Whether make it shared or static" ON)
option(BUILD_EXPORT "Whether dllexport/dllimport or not" OFF)

if (BUILD_64BITS)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
endif()

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

if (BUILD_EXPORT)
	set(NIKI_COMPILE_DEFINITIONS ${NIKI_COMPILE_DEFINITIONS} BUILD_EXPORT=1)
endif()

if(BUILD_SHARED)
	add_library(NikiScript SHARED ${SOURCES})
	set_target_properties(NikiScript PROPERTIES VERSION ${PROJECT_VERSION} POSITION_INDEPENDENT_CODE ON)

	set(NIKI_COMPILE_DEFINITIONS ${NIKI_COMPILE_DEFINITIONS} BUILD_SHARED=1)
	target_compile_definitions(NikiScript PRIVATE ${NIKI_COMPILE_DEFINITIONS})
else()
    add_library(NikiScript STATIC ${SOURCES})

	set(NIKI_COMPILE_DEFINITIONS ${NIKI_COMPILE_DEFINITIONS} BUILD_SHARED=0)
	target_compile_definitions(NikiScript PRIVATE ${NIKI_COMPILE_DEFINITIONS})
endif()

target_include_directories(NikiScript PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_directories(NikiScript PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(NikiScript PRIVATE sds)

add_subdirectory(${PROJECT_SOURCE_DIR}/example)