#pragma once
#include <optional>

namespace vk {
    class Instance;
    class SurfaceKHR;
    class PhysicalDevice;
    class Device;
    class Queue;
    class SwapchainKHR;
    class Image;
    struct Extent2D;
    struct SurfaceCapabilitiesKHR;
    struct SurfaceFormatKHR;
    enum class PresentModeKHR;
    enum class Format;
};


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};