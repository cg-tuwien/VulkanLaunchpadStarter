/*
 * Copyright 2023 TU Wien, Institute of Visual Computing & Human-Centered Technology.
 */
#include <vulkan/vulkan.h>

void teapotCreateGeometryAndBuffers();
void teapotDestroyBuffers();
void teapotDraw();

void teapotDraw(VkPipeline pipeline);
void teapotDraw(VkPipeline pipeline, VkDescriptorSet descriptor_set);

VkBuffer teapotGetPositionsBuffer();
VkBuffer teapotGetIndicesBuffer();
uint32_t teapotGetNumIndices();
