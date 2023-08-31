#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME VKViewer)

set(VKNGINE_SRC
        App.cpp
        main.cpp)

set(VKNGINE_HEADER
        App.hpp)

add_executable(${VKNGINE_APP_NAME} ${VKNGINE_SRC} ${VKNGINE_HEADER})

target_link_libraries(${VKNGINE_APP_NAME} PRIVATE glfw glm::glm Vulkan::Vulkan)

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
