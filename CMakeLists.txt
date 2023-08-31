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
#                ENGINE
#=======================================

add_subdirectory("VulkanNgine")
