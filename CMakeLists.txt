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
        $<IF:$<NOT:$<CXX_COMPILER_ID:MSVC>>, 
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
            -Wduplicated-cond
            -Wduplicated-branches
            -Wuseless-cast
            -Wlogical-op,
            /Wall
            /WX
            /wd4514
            /wd4820
            /wd4623
            /wd4626
            /wd5027
            /wd5045>)

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

macro(vkngine_add_library)
    set(_options)
    set(_oneValueArgs TARGET TYPE)
    set(_multiValueArgs SOURCES HEADERS PUBLIC_LINKS PRIVATE_LINKS)
    cmake_parse_arguments(arg "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" ${ARGN})

    add_library(${arg_TARGET} ${arg_TYPE} ${arg_SOURCES} ${arg_HEADERS})

    set(_includeType PUBLIC)
    set(_compileType PRIVATE)
    if(${arg_TYPE} STREQUAL INTERFACE)
        set(_includeType INTERFACE)
        set(_compileType INTERFACE)
    endif()

    target_include_directories(${arg_TARGET} ${_includeType} ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(${arg_TARGET} PUBLIC ${arg_PUBLIC_LINKS} PRIVATE ${arg_PRIVATE_LINKS})

    target_compile_options(${arg_TARGET} 
        ${_compileType} 
        $<IF:$<NOT:$<CXX_COMPILER_ID:MSVC>>, 
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
            -Wduplicated-cond
            -Wduplicated-branches
            -Wuseless-cast
            -Wlogical-op,
            /Wall
            /WX
            /wd4514
            /wd4820
            /wd4623
            /wd4626
            /wd5027
            /wd5045
            /wd4191
            /wd4061>)
endmacro()

#=======================================
#                ENGINE
#=======================================

add_subdirectory("stb")
add_subdirectory("VulkanNgine")
add_subdirectory("App")
