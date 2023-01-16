/*
 * Copyright 2023 TU Wien, Institute of Visual Computing & Human-Centered Technology.
 */
#include <vulkan/vulkan.h>
#include <vector>

/* --------------------------------------------- */
// Vulkan-Specific Helper Function Definitions
// As a convention, their names start with `hlp`.
/* --------------------------------------------- */

/*!
 *	Queries this system's supported instance extensions and determines whether or not the given 
 *	extension name is among them.
 *	@param		extension_name		The extension name to be checked.
 *	@return		True if the extension name is supported on this system, false otherwise.
 */
bool hlpIsInstanceExtensionSupported(const char* extension_name);

/*!
 *	Queries this system's supported instance layers and determines whether or not the given 
 *	layer name is among them.
 *	@param		layer_name			The layer name to be checked.
 *	@return		True if the layer name is supported on this system, false otherwise.
 */
bool hlpIsInstanceLayerSupported(const char* layer_name);

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
uint32_t hlpSelectPhysicalDeviceIndex(const VkPhysicalDevice* physical_devices, uint32_t physical_device_count, VkSurfaceKHR surface);

/*!
 *	From the given list of physical devices, select the first one that satisfies all requirements.
 *	@param		physical_devices	A vector containing all available VkPhysicalDevice handles, like those
 *									that are returned from vkEnumeratePhysicalDevices. 
 *	@param		surface				A valid VkSurfaceKHR handle, which is used to determine if a certain 
 *									physical device supports presenting images to the given surface.
 *	@return		The index of the physical device that satisfies all requirements is returned.
 */ 
uint32_t hlpSelectPhysicalDeviceIndex(const std::vector<VkPhysicalDevice>& physical_devices, VkSurfaceKHR surface);

/*!
 *	Based on the given physical device and the surface, a the physical device's surface capabilites are read and returned.
 *	@return		VkSurfaceCapabilitiesKHR data
 */
VkSurfaceCapabilitiesKHR hlpGetPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

/*!
 *	Based on the given physical device and the surface, a supported surface image format
 *	which can be used for the framebuffer's attachment formats is searched and returned.
 *	@return		A supported format is returned.
 */
VkSurfaceFormatKHR hlpGetSurfaceImageFormat(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

/*!
 *	Based on the given physical device and the surface, return its surface transform flag.
 *	This can be used to set the swap chain to the same configuration as the surface's current transform.
 *	@return		The surface capabilities' currentTransform value is returned, which is suitable for swap chain config.
 */
VkSurfaceTransformFlagBitsKHR hlpGetSurfaceTransform(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
