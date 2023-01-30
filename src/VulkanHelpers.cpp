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

void hlpRecordPipelineBarrierWithImageLayoutTransition(
	VkCommandBuffer            command_buffer,
	VkPipelineStageFlags       src_stage_mask,
	VkPipelineStageFlags       dst_stage_mask,
	VkAccessFlags              src_access_mask,
	VkAccessFlags              dst_access_mask,
	VkImage                    image,
	VkImageLayout              old_layout,
	VkImageLayout              new_layout)
{
	VkImageMemoryBarrier image_memory_barrier = {};
	image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	image_memory_barrier.srcAccessMask = src_access_mask;
	image_memory_barrier.dstAccessMask = dst_access_mask;
	image_memory_barrier.oldLayout = old_layout;
	image_memory_barrier.newLayout = new_layout;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.image = image;
	image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_memory_barrier.subresourceRange.baseMipLevel = 0;
	image_memory_barrier.subresourceRange.levelCount = 1;
	image_memory_barrier.subresourceRange.baseArrayLayer = 0;
	image_memory_barrier.subresourceRange.layerCount = 1;
	vkCmdPipelineBarrier(command_buffer,
		src_stage_mask, dst_stage_mask,
		0,
		0, nullptr,
		0, nullptr,
		1, &image_memory_barrier
	);
}

void hlpRecordCopyBufferToImage(
	VkCommandBuffer            command_buffer,
	VkBuffer                   buffer,
	VkImage                    image,
	uint32_t                   image_width,
	uint32_t                   image_height,
	VkImageLayout              image_layout)
{
	VkBufferImageCopy buffer_image_copy_region = {};
	buffer_image_copy_region.bufferOffset = 0;
	buffer_image_copy_region.bufferRowLength = 0;
	buffer_image_copy_region.bufferImageHeight = 0;
	buffer_image_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	buffer_image_copy_region.imageSubresource.mipLevel = 0;
	buffer_image_copy_region.imageSubresource.baseArrayLayer = 0;
	buffer_image_copy_region.imageSubresource.layerCount = 1;
	buffer_image_copy_region.imageOffset = VkOffset3D{ 0, 0, 0 };
	buffer_image_copy_region.imageExtent = VkExtent3D{ image_width, image_height, 1 };
	vkCmdCopyBufferToImage(command_buffer, buffer, image, image_layout, 1, &buffer_image_copy_region);
}

VkImageView hlpCreateImageView(VkDevice device, VkImage image, VkFormat image_format)
{
	VkImageViewCreateInfo image_view_create_info = {};
	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.image = image;
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_create_info.format = image_format;
	image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_R;
	image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_G;
	image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_B;
	image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_A;
	image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_create_info.subresourceRange.baseMipLevel = 0u;
	image_view_create_info.subresourceRange.levelCount = 1u;
	image_view_create_info.subresourceRange.baseArrayLayer = 0u;
	image_view_create_info.subresourceRange.layerCount = 1u;

	VkImageView image_view;
	VkResult result = vkCreateImageView(device, &image_view_create_info, nullptr, &image_view);
	VKL_CHECK_VULKAN_RESULT(result);

	return image_view;
}

void hlpDestroyImageView(VkDevice device, VkImageView image_view)
{
	vkDestroyImageView(device, image_view, nullptr);
}

VkSampler hlpCreateSampler(VkDevice device, VkFilter mag_filter, VkFilter min_filter)
{
	VkSamplerCreateInfo sampler_create_info = {};
	sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_create_info.magFilter = mag_filter;
	sampler_create_info.minFilter = min_filter;
	sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	sampler_create_info.minLod = 0.0f;
	sampler_create_info.maxLod = 0.0f;

	VkSampler sampler;
	VkResult result = vkCreateSampler(device, &sampler_create_info, nullptr, &sampler);
	VKL_CHECK_VULKAN_RESULT(result);

	return sampler;
}

void hlpDestroySampler(VkDevice device, VkSampler sampler)
{
	vkDestroySampler(device, sampler, nullptr);
}
