file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../include/Nova/*.hpp")

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

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
    set(OPTIONS_MISC /Zc:preprocessor /MP)
    set(OPTIONS_WARNINGS /W4)

    if(NOVA_DIST)
        set(OPTIONS_OPTIMIZATION /O2)
        set(OPTIONS_LINK /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    elseif(NOVA_DEBUG)
        set(OPTIONS_OPTIMIZATION /Zi)
    elseif(NOVA_RELEASE)
        set(OPTIONS_OPTIMIZATION /O2 /Zi)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(OPTIONS_WARNINGS -Wall -Wpedantic)

    if(NOVA_DIST)
        set(OPTIONS_OPTIMIZATION -O2)
        if(WIN32)
			set(OPTIONS_LINK -mwindows)
		endif()
    elseif(NOVA_DEBUG)
        set(OPTIONS_OPTIMIZATION -g)
    elseif(NOVA_RELEASE)
        set(OPTIONS_OPTIMIZATION -O2 -g)
    endif()
endif()

if(NOVA_DEBUG OR NOVA_RELEASE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOVA_DEBUG)
elseif(NOVA_DIST)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOVA_DIST)
endif()

message(STATUS "OPTIONS_WARNINGS: ${OPTIONS_WARNINGS}")
message(STATUS "OPTIONS_OPTIMIZATION: ${OPTIONS_OPTIMIZATION}")
message(STATUS "OPTIONS_MISC: ${OPTIONS_MISC}")
message(STATUS "OPTIONS_LINK: ${OPTIONS_LINK}")

target_compile_options(${PROJECT_NAME} PUBLIC ${OPTIONS_WARNINGS} ${OPTIONS_OPTIMIZATION} ${OPTIONS_MISC})
target_link_options(${PROJECT_NAME} PUBLIC ${OPTIONS_LINK})
