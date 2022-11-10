cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
    vkl
    GIT_REPOSITORY      https://github.com/cg-tuwien/VulkanLaunchpad
    GIT_TAG             deps #currently set to deps branch, change to main later
)

FetchContent_MakeAvailable(vkl)