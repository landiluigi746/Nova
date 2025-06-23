add_library(glad STATIC "glad/include/glad/glad.h" "glad/include/KHR/khrplatform.h" "glad/src/glad.c")
target_include_directories(glad PUBLIC "glad/include")

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE "stb_image")

add_library(reasings INTERFACE)
target_include_directories(reasings INTERFACE "reasings")