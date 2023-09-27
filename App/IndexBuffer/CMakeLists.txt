#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME IndexBuffer)

add_executable(${VKNGINE_APP_NAME} main.cpp)

target_link_libraries(${VKNGINE_APP_NAME} PRIVATE VKNgine)

target_compile_options(${VKNGINE_APP_NAME} PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wmisleading-indentation
    -Wdouble-promotion
    -Wformat=2
    -Wno-format-y2k
    -Werror
    -Wno-unused-parameter
    $<$<NOT:$<BOOL:${APPLE}>>:
    -Wduplicated-cond
    -Wduplicated-branches
    -Wuseless-cast
    -Wlogical-op>>)

add_shader(${VKNGINE_APP_NAME} SOURCES indexBuffer.vert indexBuffer.frag)
