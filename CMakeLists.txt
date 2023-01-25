cmake_minimum_required(VERSION 3.14)
project(VulkanLaunchpadStarter)

#================================#
# Compiler                       #
#================================#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/VulkanLaunchpad/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


#================================#
# VulkanLaunchpad (dependency)   #
#================================#
add_subdirectory(VulkanLaunchpad)

#================================#
# VulkanLaunchpadStarter         #
#================================#
add_executable(${PROJECT_NAME} 
    src/Main.cpp 
    src/VulkanHelpers.h
    src/VulkanHelpers.cpp 
    src/Teapot.h 
    src/Teapot.cpp 
)
target_link_libraries(${PROJECT_NAME} PRIVATE VulkanLaunchpad)
add_dependencies(${PROJECT_NAME} VulkanLaunchpad)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

#================================#
# IDE specific setup              #
#================================#
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
elseif(CMAKE_GENERATOR MATCHES "Xcode")
    set(CMAKE_XCODE_GENERATE_SCHEME ON)
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_GENERATE_SCHEME ON)
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY XCODE_STARTUP_PROJECT ${PROJECT_NAME})
endif()