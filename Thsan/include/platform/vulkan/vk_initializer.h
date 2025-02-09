#pragma once
#include <vector>
#include "vk_config.h"

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice* device, vk::SurfaceKHR* surface);
std::vector<vk::Queue> getQueues(vk::PhysicalDevice pd, vk::Device d, vk::SurfaceKHR* surface);