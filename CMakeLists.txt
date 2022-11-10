cmake_minimum_required(VERSION 3.14)
project(VulkanLaunchpad_Starter)

add_executable(VulkanLaunchpad_Starter src/main.cpp)

# VulkanLaunchpad
include(cmake/vkl.cmake)
target_link_libraries(${PROJECT_NAME} PUBLIC VulkanLaunchpad)
