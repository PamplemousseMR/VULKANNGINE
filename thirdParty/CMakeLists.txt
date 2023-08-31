#=======================================
#                PROJECT                
#=======================================

cmake_minimum_required(VERSION 3.20)
project("VULKANNGINE_thirdparty" C CXX)

#=======================================
#               OPTIONS
#=======================================

option(VKNGINE_BUILD_GLFW "Build glfw" ON)

#=======================================
#                GLFW
#=======================================

include(ExternalProject)

set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/external")

set(CMAKE_CORE_BUILD_FLAGS
    -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_DO_STRIP=${CMAKE_INSTALL_DO_STRIP}
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON)

if(VKNGINE_BUILD_GLFW)
    ExternalProject_Add(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.8
        PREFIX ${BUILD_DIR}
        BUILD_IN_SOURCE 0
        BUILD_ALWAYS 0
        SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/glfw
        BINARY_DIR ${BUILD_DIR}/glfw_build
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        CONFIGURE_COMMAND
            ${CMAKE_COMMAND}
            ${CMAKE_CORE_BUILD_FLAGS}
            -DCMAKE_INSTALL_RPATH=$ORIGIN
            -DGLFW_BUILD_EXAMPLES=OFF
            -DGLFW_BUILD_TESTS=OFF
            -DGLFW_BUILD_DOCS=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> <SOURCE_DIR>)
endif()
