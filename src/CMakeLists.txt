file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../include/Nova/*.hpp")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include" "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/stb_image")

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    fmt::fmt
    ${OPENGL_LIBRARIES}
    glad
    glfw
    imgui
    glm::glm
)

if(MSVC)
    set(OPTIONS_MISC /Zc:preprocessor)
    set(OPTIONS_WARNINGS /W4)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(OPTIONS_OPTIMIZATION /O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPTIONS_OPTIMIZATION /Zi)
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(OPTIONS_OPTIMIZATION /O2 /Zi)
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(OPTIONS_OPTIMIZATION /Os)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(OPTIONS_WARNINGS -Wall -Wpedantic)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(OPTIONS_OPTIMIZATION -O3)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPTIONS_OPTIMIZATION -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(OPTIONS_OPTIMIZATION -O2 -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(OPTIONS_OPTIMIZATION -Os)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOVA_DEBUG)
endif()

message(STATUS "Compiler options: " ${OPTIONS_WARNINGS} ${OPTIONS_OPTIMIZATION} ${OPTIONS_MISC})

target_compile_options(${PROJECT_NAME} PUBLIC ${OPTIONS_WARNINGS} ${OPTIONS_OPTIMIZATION} ${OPTIONS_MISC})
