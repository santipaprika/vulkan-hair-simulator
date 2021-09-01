/*
* Vulkan buffer class
*
* Encapsulates a Vulkan buffer
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*/

#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace vkr {

class Device;

/**
	* @brief Encapsulates access to a Vulkan buffer backed up by device memory
	* @note To be filled by an external source like the VulkanDevice
	*/
struct Buffer {
    VkDevice device;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDescriptorBufferInfo descriptorInfo;
    VkDeviceSize size = 0;
    VkDeviceSize alignment = 0;
    void* mapped = nullptr;
    /** @brief Usage flags to be filled by external source at buffer creation (to query at some later point) */
    VkBufferUsageFlags usageFlags;
    /** @brief Memory property flags to be filled by external source at buffer creation (to query at some later point) */
    VkMemoryPropertyFlags memoryPropertyFlags;

    VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void unmap();
    VkResult bind(VkDeviceSize offset = 0);
    void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void copyTo(void* data, VkDeviceSize size);
    VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void destroy();
};
}  // namespace vkr