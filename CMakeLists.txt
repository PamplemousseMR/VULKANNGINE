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
#            SETTINGS
#=======================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#=======================================
#            MACROS
#=======================================

macro(vkngine_add_executable)
    set(options)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES SHADERS LINKS)
    cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${arg_TARGET} ${arg_SOURCES} ${arg_SHADERS})

    target_link_libraries(${arg_TARGET} PRIVATE ${arg_LINKS})

    if(MSVC)
	    add_custom_command(TARGET ${arg_TARGET} POST_BUILD
					       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${arg_TARGET}> $<TARGET_FILE_DIR:${arg_TARGET}>
					       COMMAND_EXPAND_LISTS)
    endif()

    target_compile_options(${arg_TARGET} 
    PRIVATE 
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
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
        -Wlogical-op>
        
        >)

    foreach(source ${arg_SHADERS})
        add_custom_command(
            OUTPUT ${source}.bin
            DEPENDS ${source}
            COMMAND
                ${Vulkan_GLSLC_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}/${source}
                -o ${source}.bin -Werror
        )
        target_sources(${arg_TARGET} PRIVATE ${source}.bin)
    endforeach()
endmacro()

#=======================================
#                ENGINE
#=======================================

add_subdirectory("VulkanNgine")
add_subdirectory("App")
