cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(NikiScript VERSION 0.0.1 LANGUAGES CXX)

option(BUILD_64BITS "x64 or x86" ON)
option(BUILD_SHARED "Whether make it shared or static" ON)
option(BUILD_EXPORT "Whether dllexport/dllimport or not" OFF)

if (BUILD_64BITS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
endif()

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if (BUILD_EXPORT)
	set(JIKARU_COMPILE_DEFINITIONS ${JIKARU_COMPILE_DEFINITIONS} BUILD_EXPORT=1)
endif()

if(BUILD_SHARED)
	add_library(NikiScript SHARED ${SOURCES})
	set_target_properties(NikiScript PROPERTIES VERSION ${PROJECT_VERSION} POSITION_INDEPENDENT_CODE ON)

	set(JIKARU_COMPILE_DEFINITIONS ${JIKARU_COMPILE_DEFINITIONS} BUILD_SHARED=1)
	target_compile_definitions(NikiScript PRIVATE ${JIKARU_COMPILE_DEFINITIONS})
else()
    add_library(NikiScript STATIC ${SOURCES})

	set(JIKARU_COMPILE_DEFINITIONS ${JIKARU_COMPILE_DEFINITIONS} BUILD_SHARED=0)
	target_compile_definitions(NikiScript PRIVATE ${JIKARU_COMPILE_DEFINITIONS})
endif()


target_include_directories(NikiScript PRIVATE ${PROJECT_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/example)
