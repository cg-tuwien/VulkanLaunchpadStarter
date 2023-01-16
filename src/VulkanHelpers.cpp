/*
 * Copyright 2023 TU Wien, Institute of Visual Computing & Human-Centered Technology.
 */
#include "VulkanHelpers.h"
#include "VulkanLaunchpad.h"

/* --------------------------------------------- */
// Vulkan-Specific Helper Function Definitions
/* --------------------------------------------- */

bool hlpIsInstanceExtensionSupported(const char* extension_name) {
	static std::vector<VkExtensionProperties> supportedExtensions = []() {
		// Get the extensions which are supported:
		uint32_t numSupportedExtensions;
		VkResult result;
		result = vkEnumerateInstanceExtensionProperties(nullptr, &numSupportedExtensions, nullptr);
		VKL_CHECK_VULKAN_RESULT(result);
		std::vector<VkExtensionProperties> supExt(numSupportedExtensions);
		result = vkEnumerateInstanceExtensionProperties(nullptr, &numSupportedExtensions, supExt.data());
		VKL_CHECK_VULKAN_ERROR(result);
		return supExt;
	}();

	// Check if the queried extension name is among the supported extension names:
	for (const auto& exProp : supportedExtensions) {
		if (strncmp(extension_name, exProp.extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0) {
			return true;
		}
	}
	return false;
}

bool hlpIsInstanceLayerSupported(const char* layer_name) {
	static std::vector<VkLayerProperties> supportedLayers = []() {
		// Get the layers which are supported:
		uint32_t numSupportedLayers;
		VkResult result;
		result = vkEnumerateInstanceLayerProperties(&numSupportedLayers, nullptr);
		VKL_CHECK_VULKAN_ERROR(result);
		std::vector<VkLayerProperties> supLay(numSupportedLayers);
		result = vkEnumerateInstanceLayerProperties(&numSupportedLayers, supLay.data());
		VKL_CHECK_VULKAN_ERROR(result);
		return supLay;
	}();

	// Check if the queried extension name is among the supported extension names:
	for (const auto& layerProps : supportedLayers) {
		if (strncmp(layer_name, layerProps.layerName, VK_MAX_EXTENSION_NAME_SIZE) == 0) {
			return true;
		}
	}
	return false;
}

uint32_t hlpSelectPhysicalDeviceIndex(const VkPhysicalDevice* physical_devices, uint32_t physical_device_count, VkSurfaceKHR surface) {
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

uint32_t hlpSelectPhysicalDeviceIndex(const std::vector<VkPhysicalDevice>& physical_devices, VkSurfaceKHR surface) {
	return hlpSelectPhysicalDeviceIndex(physical_devices.data(), static_cast<uint32_t>(physical_devices.size()), surface);
}

VkSurfaceCapabilitiesKHR hlpGetPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities);
    VKL_CHECK_VULKAN_ERROR(result);
    return surface_capabilities;
}

VkSurfaceFormatKHR hlpGetSurfaceImageFormat(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
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

VkSurfaceTransformFlagBitsKHR hlpGetSurfaceTransform(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    return hlpGetPhysicalDeviceSurfaceCapabilities(physical_device, surface).currentTransform;
}
