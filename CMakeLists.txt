cmake_minimum_required(VERSION 3.12)

project(Nova LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "
  _   _                    
 | \\ | |                   
 |  \\| |  ___ __   __ __ _ 
 | . ` | / _ \\\\ \\ / // _` |
 | |\\  || (_) |\\ V /| (_| |
 |_| \\_| \\___/  \\_/  \\__,_|
")

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" NOVA_STANDALONE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(NOVA_BUILD_TYPE "Debug" CACHE STRING "")
	set(NOVA_DEBUG ON CACHE BOOL "")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(NOVA_BUILD_TYPE "Dist" CACHE STRING "")
    set(NOVA_DIST ON CACHE BOOL "")
else()
    if(NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        message(AUTHOR_WARNING "Unknown CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}. Defaulting to RelWithDebInfo.")
    endif()

    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
	set(NOVA_BUILD_TYPE "Release" CACHE STRING "")
    set(NOVA_RELEASE ON CACHE BOOL "")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${NOVA_BUILD_TYPE} CACHE INTERNAL "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${NOVA_BUILD_TYPE} CACHE INTERNAL "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${NOVA_BUILD_TYPE} CACHE INTERNAL "")

message(STATUS "CMake compiler id (version): ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "NOVA_BUILD_TYPE: ${NOVA_BUILD_TYPE}")
message(STATUS "NOVA_STANDALONE: ${NOVA_STANDALONE}")

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/vendor/FetchContentArtifacts" CACHE STRING "Dependencies directory")
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.2.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/landiluigi746/imgui-cmake.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

set(GLM_ENABLE_CXX_20 ON CACHE BOOL "")

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
	GIT_TAG v3.15.0
	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(entt)

add_subdirectory(vendor)
add_subdirectory(src)

if(NOVA_STANDALONE)
    add_subdirectory(sandbox)
endif()
