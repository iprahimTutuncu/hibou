#pragma once
#include <vulkan/vulkan_core.h>
#include "vk_config.h"
#include <vector>

struct SwapChainSupportInfo {
    vk::SurfaceCapabilitiesKHR* capabilities;
    std::vector<vk::SurfaceFormatKHR*> formats;
    std::vector<vk::PresentModeKHR*> presentModes;
};

struct SwapChainBundle {
    vk::SwapchainKHR* swapchain;
    std::vector<vk::Image*> images;
    vk::Format* format;
    vk::Extent2D* extent;
};

SwapChainSupportInfo querySwapChainSupport(vk::PhysicalDevice* device, vk::SurfaceKHR* surface);

SwapChainBundle createSwapChain(uint32_t width, uint32_t height, vk::PhysicalDevice* physicalDevice, vk::Device* device, vk::SurfaceKHR* surface);

vk::SurfaceFormatKHR pickSwapChainSurfaceFormat(std::vector<vk::SurfaceFormatKHR*> formats);

vk::PresentModeKHR pickSwapChainPresentMode(std::vector<vk::PresentModeKHR*> presentModes);

vk::Extent2D pickSwapChainExtent2D(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR* capabilities);

/*
class PhysicalDevice {
public:
    PhysicalDevice(vk::PhysicalDevice physicalDevice);
    ~PhysicalDevice();

    vk::SurfaceCapabilitiesKHR getSurfaceCapabilities(vk::SurfaceKHR surface);
    std::vector<vk::SurfaceFormatKHR> getSurfaceFormats(vk::SurfaceKHR surface);
    std::vector<vk::PresentModeKHR> getPresentModes(vk::SurfaceKHR surface);

    vk::PhysicalDevice getHandle() const { return handle; }

private:
    vk::PhysicalDevice* handle;
};

class Device {
public:
    Device(vk::PhysicalDevice physicalDevice, std::vector<uint32_t> queueFamilyIndices);
    ~Device();

    vk::Queue getQueue(uint32_t familyIndex, uint32_t queueIndex);

    vk::Device getHandle() const { return *handle; }

private:
    vk::Device* handle;
};

class Surface {
public:
    Surface(SDL_Window* window, vk::Instance instance);
    ~Surface();

    vk::SurfaceKHR getHandle() const { return *handle; }

private:
    vk::SurfaceKHR* handle;
};

class SwapChain {
public:
    SwapChain(Device& device, Surface& surface, uint32_t width, uint32_t height);
    ~SwapChain();

    vk::Format getImageFormat() const { return format; }
    vk::Extent2D getImageExtent() const { return extent; }
    std::vector<vk::Image> getImages() const { return images; }
    vk::SwapchainKHR getHandle() const { return *handle; }

private:
    vk::SwapchainKHR* handle;
    std::vector<vk::Image> images;
    vk::Format format;
    vk::Extent2D extent;
};

SwapChain createSwapChain(Surface& surface, PhysicalDevice& physicalDevice, Device& device, uint32_t width, uint32_t height);

*/