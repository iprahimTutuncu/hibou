#include "pch.h"
#include "vk_initializer.h"
#include <vulkan/vulkan.hpp>
#include <thsan/log.h>

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice* device, vk::SurfaceKHR* surface)
{
	QueueFamilyIndices indices;

	std::vector<vk::QueueFamilyProperties> queueFamilies = device->getQueueFamilyProperties();

	TS_CORE_INFO("device can support {} queue family", queueFamilies.size());

	for (uint32_t i = 0; i < queueFamilies.size(); i++) {
		const auto& queueFamily = queueFamilies[i];

		// Check if the current queue family supports graphics operations
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;

		// Check if the current queue family supports presentation on the surface
		if (device->getSurfaceSupportKHR(i, *surface))
			indices.presentFamily = i;

		// Exit early if we have found both required queue families
		if (indices.isComplete()) {
			TS_CORE_INFO("La queue family {} fa la job en masse", i);
			break;
		}
	}

	return indices;
}

std::vector<vk::Queue> getQueues(vk::PhysicalDevice pd, vk::Device d, vk::SurfaceKHR* surface)
{
	QueueFamilyIndices indices = findQueueFamilies(&pd, surface);
	return{ {
		d.getQueue(indices.graphicsFamily.value(), 0),
		d.getQueue(indices.presentFamily.value(), 0)
	} };
}