/*
 * Copyright 2021 TU Wien, Institute of Visual Computing & Human-Centered Technology.
 * This file is part of the VKL Lab Framework and must not be redistributed.
 *
 * Original version created by Lukas Gersthofer and Bernhard Steiner.
 * Vulkan edition created by Johannes Unterguggenberger (junt@cg.tuwien.ac.at).
 */
#include "VulkanLaunchpad.h"
#include <vulkan/vulkan.h>
#include <vector>


#define ADD_SHADER_DIR(x) ("assets/shaders/" + std::string(x)).c_str()

/* --------------------------------------------- */
// Helper Function Declarations
/* --------------------------------------------- */

/*!
 *	From the given list of physical devices, select the first one that satisfies all requirements.
 *	@param		physical_devices		A pointer which points to contiguous memory of #physical_device_count sequentially
										stored VkPhysicalDevice handles is expected. The handles can (or should) be those
 *										that are returned from vkEnumeratePhysicalDevices.
 *	@param		physical_device_count	The number of consecutive physical device handles there are at the memory location 
 *										that is pointed to by the physical_devices parameter.
 *	@param		surface					A valid VkSurfaceKHR handle, which is used to determine if a certain
 *										physical device supports presenting images to the given surface.
 *	@return		The index of the physical device that satisfies all requirements is returned.
 */
uint32_t selectPhysicalDeviceIndex(const VkPhysicalDevice* physical_devices, uint32_t physical_device_count, VkSurfaceKHR surface);

/*!
 *	From the given list of physical devices, select the first one that satisfies all requirements.
 *	@param		physical_devices	A vector containing all available VkPhysicalDevice handles, like those
 *									that are returned from vkEnumeratePhysicalDevices. 
 *	@param		surface				A valid VkSurfaceKHR handle, which is used to determine if a certain 
 *									physical device supports presenting images to the given surface.
 *	@return		The index of the physical device that satisfies all requirements is returned.
 */ 
uint32_t selectPhysicalDeviceIndex(const std::vector<VkPhysicalDevice>& physical_devices, VkSurfaceKHR surface);

/*!
 *	Based on the given physical device and the surface, select a queue family which supports both,
 *	graphics and presentation to the given surface. Return the INDEX of an appropriate queue family!
 *	@return		The index of a queue family which supports the required features shall be returned.
 */
uint32_t selectQueueFamilyIndex(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

/*!
 *	Based on the given physical device and the surface, a the physical device's surface capabilites are read and returned.
 *	@return		VkSurfaceCapabilitiesKHR data
 */
VkSurfaceCapabilitiesKHR getPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

/*!
 *	Based on the given physical device and the surface, a supported surface image format
 *	which can be used for the framebuffer's attachment formats is searched and returned.
 *	@return		A supported format is returned.
 */
VkSurfaceFormatKHR getSurfaceImageFormat(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

/*!
 *	Based on the given physical device and the surface, return its surface transform flag.
 *	This can be used to set the swap chain to the same configuration as the surface's current transform.
 *	@return		The surface capabilities' currentTransform value is returned, which is suitable for swap chain config.
 */
VkSurfaceTransformFlagBitsKHR getSurfaceTransform(VkPhysicalDevice physical_device, VkSurfaceKHR surface);


/*!
 *	This callback function gets invoked by GLFW whenever a GLFW error occured.
 */
void errorCallbackFromGlfw(int error, const char* description) {
	std::cout << "GLFW error " << error << ": " << description << std::endl;
}

/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
	/* --------------------------------------------- */
	// Subtask 1.1: Load Settings From File
	/* --------------------------------------------- */

	int window_width = 800;
	int window_height = 800;
	bool fullscreen = false;
	std::string window_title = "Task 0";
	// Install a callback function, which gets invoked whenever a GLFW error occurred.
	glfwSetErrorCallback(errorCallbackFromGlfw);

	/* --------------------------------------------- */
	// Subtask 1.2: Create a Window with GLFW
	/* --------------------------------------------- */
	if (!glfwInit()) {
		VKL_EXIT_WITH_ERROR("Failed to init GLFW");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No need to create a graphics context for Vulkan
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	// Open window
	GLFWmonitor* monitor = nullptr;

	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title.c_str(), monitor, nullptr);
	
	if (!window) {
		VKL_EXIT_WITH_ERROR("Failed to create window");
	}

	VkResult result;
	VkInstance vk_instance              = VK_NULL_HANDLE; // To be set during Subtask 1.3
	VkSurfaceKHR vk_surface             = VK_NULL_HANDLE; // To be set during Subtask 1.4
	VkPhysicalDevice vk_physical_device = VK_NULL_HANDLE; // To be set during Subtask 1.5
	VkDevice vk_device                  = VK_NULL_HANDLE; // To be set during Subtask 1.7
	VkQueue vk_queue                    = VK_NULL_HANDLE; // To be set during Subtask 1.7
	VkSwapchainKHR vk_swapchain         = VK_NULL_HANDLE; // To be set during Subtask 1.8

	/* --------------------------------------------- */
	// Subtask 1.3: Create a Vulkan Instance
	/* --------------------------------------------- */
	VkApplicationInfo application_info = {}; // Zero-initialize every member
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // Set the struct's type
	application_info.pEngineName = "VKL_VK_Library";
	application_info.engineVersion = VK_MAKE_API_VERSION(0, 2021, 1, 0);
	application_info.pApplicationName = "VKL_VK_Solution";
	application_info.applicationVersion = VK_MAKE_API_VERSION(0, 2021, 1, 1);
	application_info.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo instance_create_info = {}; // Zero-initialize every member
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // Set the struct's type
	instance_create_info.pApplicationInfo = &application_info;


	/* --------------------------------------------- */
	// Subtask 1.4: Create a Vulkan Window Surface
	/* --------------------------------------------- */

	/* --------------------------------------------- */
	// Subtask 1.5: Pick a Physical Device
	/* --------------------------------------------- */
	
	/* --------------------------------------------- */
	// Subtask 1.6: Select a Queue Family
	/* --------------------------------------------- */

	/* --------------------------------------------- */
	// Subtask 1.7: Create a Logical Device and Get Queue
	/* --------------------------------------------- */

	/* --------------------------------------------- */
	// Subtask 1.8: Create a Swapchain
	/* --------------------------------------------- */
	uint32_t queueFamilyIndexCount = 0u;
	std::vector<uint32_t>queueFamilyIndices;
    VkSurfaceCapabilitiesKHR surface_capabilities = getPhysicalDeviceSurfaceCapabilities(vk_physical_device, vk_surface);
	// Build the swapchain config struct:
	VkSwapchainCreateInfoKHR swapchain_create_info = {};
	swapchain_create_info.surface = vk_surface;
	swapchain_create_info.minImageCount = surface_capabilities.minImageCount;
	swapchain_create_info.imageArrayLayers = 1u;
	swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
	{
		swapchain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}
	else
	{
		std::cout << "Warning: Automatic Testing might fail, VK_IMAGE_USAGE_TRANSFER_SRC_BIT image usage is not supported" << std::endl;
	}
	swapchain_create_info.preTransform = surface_capabilities.currentTransform;
	swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info.clipped = VK_TRUE;
	swapchain_create_info.queueFamilyIndexCount = queueFamilyIndexCount;
	swapchain_create_info.pQueueFamilyIndices = queueFamilyIndices.data();

	/* --------------------------------------------- */
	// Subtask 1.9: Init VKL Framework
	/* --------------------------------------------- */

	// Gather swapchain config as required by the framework:
	VklSwapchainConfig swapchain_config = {};

	// Init the framework:
	if (!vklInitFramework(vk_instance, vk_surface, vk_physical_device, vk_device, vk_queue, swapchain_config)) {
		VKL_EXIT_WITH_ERROR("Failed to init framework");
	}


	/* --------------------------------------------- */
	// Subtask 1.10: Set-up the Render Loop
	// Subtask 1.11: Register a Key Callback
	/* --------------------------------------------- */

	// Wait for all GPU work to finish before cleaning up:
	vkDeviceWaitIdle(vk_device);

	/* --------------------------------------------- */
	// Subtask 1.12: Cleanup
	/* --------------------------------------------- */
	vklDestroyFramework();

	return EXIT_SUCCESS;
}

/* --------------------------------------------- */
// Helper Function Definitions
/* --------------------------------------------- */


uint32_t selectPhysicalDeviceIndex(const VkPhysicalDevice* physical_devices, uint32_t physical_device_count, VkSurfaceKHR surface) {
	// Iterate over all the physical devices and select one that satisfies all our requirements.
	// Our requirements are:
	//  - Must support a queue that must have both, graphics and presentation capabilities
	for (uint32_t physical_device_index = 0u; physical_device_index < physical_device_count; ++physical_device_index) {
		// Get the number of different queue families:
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[physical_device_index], &queue_family_count, nullptr);

		// Get the queue families' data:
		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[physical_device_index], &queue_family_count, queue_families.data());

		for (uint32_t queue_family_index = 0u; queue_family_index < queue_family_count; ++queue_family_index) {
			// If this physical device supports a queue family which supports both, graphics and presentation
			//  => select this physical device
			if ((queue_families[queue_family_index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
				// This queue supports graphics! Let's see if it also supports presentation:
				VkBool32 presentation_supported;
				vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices[physical_device_index], queue_family_index, surface, &presentation_supported);

				if (VK_TRUE == presentation_supported) {
					// We've found a suitable physical device
					return physical_device_index;
				}
			}
		}
	}
	VKL_EXIT_WITH_ERROR("Unable to find a suitable physical device that supports graphics and presentation on the same queue.");
}

uint32_t selectPhysicalDeviceIndex(const std::vector<VkPhysicalDevice>& physical_devices, VkSurfaceKHR surface) {
	return selectPhysicalDeviceIndex(physical_devices.data(), static_cast<uint32_t>(physical_devices.size()), surface);
}

uint32_t selectQueueFamilyIndex(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
	// Get the number of different queue families for the given physical device:
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

	// Get the queue families' data:
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());
	VKL_EXIT_WITH_ERROR("Unable to find a suitable queue family that supports graphics and presentation on the same queue.");
}

VkSurfaceFormatKHR getSurfaceImageFormat(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
	VkResult result;

	uint32_t surface_format_count;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, nullptr);
	VKL_CHECK_VULKAN_ERROR(result);

	std::vector<VkSurfaceFormatKHR> surface_formats(surface_format_count);
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, surface_formats.data());
	VKL_CHECK_VULKAN_ERROR(result);

	if (surface_formats.empty()) {
		VKL_EXIT_WITH_ERROR("Unable to find supported surface formats.");
	}

	// Prefer a RGB8/sRGB format; If we are unable to find such, just return any:
	for (const VkSurfaceFormatKHR& f : surface_formats) {
		if ((  f.format == VK_FORMAT_B8G8R8A8_SRGB || f.format == VK_FORMAT_R8G8B8A8_SRGB )
			&& f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return f;
		}
	}

	return surface_formats[0];
}

VkSurfaceCapabilitiesKHR getPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities);
    VKL_CHECK_VULKAN_ERROR(result);
    return surface_capabilities;
}

VkSurfaceTransformFlagBitsKHR getSurfaceTransform(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    return getPhysicalDeviceSurfaceCapabilities(physical_device, surface).currentTransform;
}
