cmake_minimum_required(VERSION 3.14)
project(VulkanLaunchpadStarter)

#================================#
# VulkanLaunchpad (dependency)   #
#================================#
add_subdirectory(VulkanLaunchpad)

#================================#
# VulkanLaunchpadStarter         #
#================================#
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE VulkanLaunchpad)
add_dependencies(${PROJECT_NAME} VulkanLaunchpad)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

#================================#
# # IDE specific setup           #
#================================#
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
else()
endif()