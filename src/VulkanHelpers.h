/*
 * Copyright 2023 TU Wien, Institute of Visual Computing & Human-Centered Technology.
 */
#include <vulkan/vulkan.h>
#include <vector>

/* --------------------------------------------- */
// Vulkan-Specific Helper Struct Definitions
// As a convention, their names start with `Hlp`.
/* --------------------------------------------- */

/*!
 * A struct containing all data for a geometry object on the GPU-side.
 * Concretely, includes handles for the positions, normals, and texture 
 * coordinate buffers as well as the number of indices and their format. 
 */
struct HlpGeometryHandles {
	//! The size of the positions buffer in bytes
	size_t positionsBufferSize;

	//! A handle to a Vulkan Buffer intended to contain the vertex position data.
	VkBuffer positionsBuffer;

	//! The size of the indices buffer in bytes
	size_t indicesBufferSize;

	//! A handle to a Vulkan Buffer intended to contain the face index data.
	VkBuffer indicesBuffer;

	//! The total number of indices in the `indicesBuffer`.
	uint32_t numberOfIndices;

	//! Specifies the size of the indices. In the context of Vulkan Launchpad, VK_INDEX_TYPE_UINT32
	//! will be the right value in most cases---for example, vklLoadModelGeometry stores indices as 
	//! uint32_t => use VK_INDEX_TYPE_UINT32 to match its type!
	VkIndexType indexType;

	//! The size of the normals buffer in bytes
	size_t normalsBufferSize;

	//! A handle to a Vulkan Buffer intended to contain the vertex normal data.
	VkBuffer normalsBuffer;

	//! The size of the texture coordinates buffer in bytes
	size_t textureCoordinatesBufferSize;

	//! A handle to a Vulkan Buffer on the GPU intended to contain vertex texture coordinates.
	VkBuffer textureCoordinatesBuffer;
 };

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

/*!
 *  Records an image memory barrier with layout transition into the given command buffer.
 *  @param	command_buffer	Command buffer to record the image memory barrier into
 *  @param	src_stage_mask	The stage(s) of previous commands to sync with.
 *	@param	dst_stage_mask	The stage(s) of subsequent commands to sync with. 
 *	@param	src_access_mask	The memory access(es) of previous commands to be made available.
 *	@param	dst_access_mask	The memory access(es) of subsequent commands to make the data visible to.
 *	@param	image			The image that must be synchronized
 *	@param	old_layout		The previous image layout, i.e. the layout transitioned from.
 *	@param	new_layout		The new layout the image shall be transitioned into.
 */
void hlpRecordPipelineBarrierWithImageLayoutTransition(
	VkCommandBuffer            command_buffer,
	VkPipelineStageFlags       src_stage_mask,
	VkPipelineStageFlags       dst_stage_mask,
	VkAccessFlags              src_access_mask,
	VkAccessFlags              dst_access_mask,
	VkImage                    image,
	VkImageLayout              old_layout,
	VkImageLayout              new_layout);

/*!
 *  Records a copy buffer to image command into the given command buffer
 *  @param	command_buffer	Command buffer to record the copy command into
 *  @param	buffer			The buffer to be copied from
 *	@param	image			The image to be copied to
 *	@param	image_width		The image's width
 *	@param	image_height	The image's height
 *	@param	image_layout	The image's layout at the time the copy happens.
 */
void hlpRecordCopyBufferToImage(
	VkCommandBuffer            command_buffer,
	VkBuffer                   buffer,
	VkImage                    image,
	uint32_t                   image_width,
	uint32_t                   image_height,
	VkImageLayout              image_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

/*!
 *  Creates an image view for the given image.
 *  Note: This convenience function only creates an image view for the image's 
 *        first layer and for its first mipmap layer. 
 *  @param	device			Device handle
 *  @param	image			The image which an image view shall be created for
 *	@param	image_format	The image's format
 *  @return	A handle to a new image view.
 */
VkImageView hlpCreateImageView(VkDevice device, VkImage image, VkFormat image_format);

/*!
 *  Destroys an image view which was previously created with hlpCreateImageView
 *  @param	device			Device handle
 *  @param	image_view		The image view which shall be destroyed.
 */
void hlpDestroyImageView(VkDevice device, VkImageView image_view);

/*!
 *  Creates a sampler with most its configuration properties set to sensible default values.
 *  Only mag filter and min filter can be configured through the respective parameters.
 *  @param	device			Device handle
 *  @param	mag_filter		Specifies how to lookup textures in the magnification case.
 *  @param	min_filter		Specifies how to lookup textures in the minification case.
 *  @return	A handle to a new sampler.
 */
VkSampler hlpCreateSampler(VkDevice device, VkFilter mag_filter, VkFilter min_filter);

/*!
 *  Destroys a sampler which was previously created with hlpCreateSampler
 *  @param	device			Device handle
 *  @param	sampler			The sampler which shall be destroyed.
 */
void hlpDestroySampler(VkDevice device, VkSampler sampler);
