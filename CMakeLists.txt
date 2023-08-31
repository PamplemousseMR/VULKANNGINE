#=======================================
#                PROJECT                
#=======================================

cmake_minimum_required(VERSION 3.8)
project("VULKANNGINE")

#=======================================
#               LIBRARIES
#=======================================

#find_package(GLM REQUIRED)
find_package(glfw3 REQUIRED CONFIG)

#=======================================
#                ENGINE
#=======================================

add_subdirectory("VulkanNgine")
