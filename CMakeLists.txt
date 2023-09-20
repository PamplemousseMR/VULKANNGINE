#=======================================
#                PROJECT                
#=======================================

cmake_minimum_required(VERSION 3.8)
project("VULKANNGINE" C CXX)

#=======================================
#               LIBRARIES
#=======================================

find_package(glfw3 REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
find_package(Vulkan REQUIRED)

#=======================================
#            GLSL COMPILER
#=======================================
function(add_shader target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "" "SOURCES")
    foreach(source ${arg_SOURCES})
        add_custom_command(
            OUTPUT ${source}.bin
            DEPENDS ${source}
            COMMAND
                ${Vulkan_GLSLC_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}/${source}
                -o ${source}.bin -Werror
        )
        target_sources(${target} PRIVATE ${source}.bin)
    endforeach()
endfunction()

#=======================================
#                ENGINE
#=======================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("VulkanNgine")
