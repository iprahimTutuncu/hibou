#include "pch.h"
#include "vk_swapchain.h"
#include <vulkan/vulkan.hpp>
#include <thsan/log.h>
#include "vk_initializer.h"

SwapChainSupportInfo querySwapChainSupport(vk::PhysicalDevice* device, vk::SurfaceKHR* surface) {

	SwapChainSupportInfo support;
	support.capabilities = new vk::SurfaceCapabilitiesKHR();
	*(support.capabilities) = device->getSurfaceCapabilitiesKHR(*surface);

	/*
	*
	typedef struct VkSurfaceCapabilitiesKHR {
		uint32_t                         minImageCount;
		uint32_t                         maxImageCount;
		VkExtent2D                       currentExtent;
		VkExtent2D                       minImageExtent;
		VkExtent2D                       maxImageExtent;
		uint32_t                         maxImageArrayLayers;
		VkSurfaceTransformFlagsKHR       supportedTransforms;
		VkSurfaceTransformFlagBitsKHR    currentTransform;
		VkCompositeAlphaFlagsKHR         supportedCompositeAlpha;
		VkImageUsageFlags                supportedUsageFlags;
	} VkSurfaceCapabilitiesKHR;

	*/

	TS_CORE_INFO("swapchain can support: ");
	TS_CORE_INFO("minimum image count: {}, maximum image count: {}.",
		support.capabilities->minImageCount,
		support.capabilities->maxImageCount
	);
	TS_CORE_INFO("minImageExtent	  : {}, {}", support.capabilities->minImageExtent.width, support.capabilities->minImageExtent.height);
	TS_CORE_INFO("maxImageExtent	  : {}, {}", support.capabilities->maxImageExtent.width, support.capabilities->maxImageExtent.height);
	TS_CORE_INFO("currentExtent: {}, {}", support.capabilities->currentExtent.width, support.capabilities->currentExtent.height);
	TS_CORE_INFO("maxImageArrayLayers : {}", support.capabilities->maxImageArrayLayers);

	std::vector<vk::SurfaceFormatKHR> formats = device->getSurfaceFormatsKHR(*surface);
	for (vk::SurfaceFormatKHR f : formats) {
		support.formats.push_back(new vk::SurfaceFormatKHR());
		*support.formats[support.formats.size() - 1] = f;
	}

	for (vk::SurfaceFormatKHR* format : support.formats) {
		TS_CORE_INFO("supported pixel format: {}", vk::to_string(format->format));
		TS_CORE_INFO("supported color space: {}", vk::to_string(format->colorSpace));
	}

	std::vector<vk::PresentModeKHR> presentModes = device->getSurfacePresentModesKHR(*surface);
	std::string presentModeStr;
	for (vk::PresentModeKHR p : presentModes) {
		support.presentModes.push_back(new vk::PresentModeKHR());
		*support.presentModes[support.presentModes.size() - 1] = p;

		if (p == vk::PresentModeKHR::eMailbox) {
			presentModeStr += " eMailBox |";
		}
		else if (p == vk::PresentModeKHR::eFifo) {
			presentModeStr += " eFifo |";
		}
		else if (p == vk::PresentModeKHR::eFifoRelaxed) {
			presentModeStr += " eFifoRelaxed |";
		}
		else if (p == vk::PresentModeKHR::eImmediate) {
			presentModeStr += " eImmediate |";
		}
		else if (p == vk::PresentModeKHR::eSharedContinuousRefresh) {
			presentModeStr += " eSharedContinuousRefresh |";
		}
		else if (p == vk::PresentModeKHR::eSharedDemandRefresh) {
			presentModeStr += " eSharedDemandRefresh |";
		}
	}

	TS_CORE_INFO("supported present mode: |{}", presentModeStr);
	return support;
}

SwapChainBundle createSwapChain(uint32_t width, uint32_t height, vk::PhysicalDevice* physicalDevice, vk::Device* device, vk::SurfaceKHR* surface)
{
	// Query the necessary swapchain support details
	SwapChainSupportInfo swapChainSupport = querySwapChainSupport(physicalDevice, surface);

	// Pick the surface format, presentation mode, and extent
	vk::SurfaceFormatKHR surfaceFormat = pickSwapChainSurfaceFormat(swapChainSupport.formats);
	vk::PresentModeKHR presentMode = pickSwapChainPresentMode(swapChainSupport.presentModes);
	vk::Extent2D extent = pickSwapChainExtent2D(width, height, swapChainSupport.capabilities);

	// Determine the number of images in the swapchain
	uint32_t imageCount = swapChainSupport.capabilities->minImageCount + 1;
	if (swapChainSupport.capabilities->maxImageCount > 0 && imageCount > swapChainSupport.capabilities->maxImageCount)
		imageCount = swapChainSupport.capabilities->maxImageCount;


	// Create the swapchain
	vk::SwapchainCreateInfoKHR createInfo{};
	createInfo.surface = *surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily.value() != indices.presentFamily.value()) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;


	createInfo.preTransform = swapChainSupport.capabilities->currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

	SwapChainBundle swapchainBundle;
	swapchainBundle.swapchain = new vk::SwapchainKHR();
	swapchainBundle.format = new vk::Format();
	swapchainBundle.extent = new vk::Extent2D();

	try {
		*swapchainBundle.swapchain = device->createSwapchainKHR(createInfo);

		// Get the swapchain images
		std::vector<vk::Image> images = device->getSwapchainImagesKHR(*swapchainBundle.swapchain);
		swapchainBundle.images.reserve(images.size());
		for (auto& image : images) {
			swapchainBundle.images.push_back(&image);
		}

		// Store the format and extent
		*swapchainBundle.format = surfaceFormat.format;
		*swapchainBundle.extent = extent;

		return swapchainBundle;
	}
	catch (vk::SystemError err) {
		TS_CORE_ERROR("Error in swapchain creation: {}", err.what());
		return swapchainBundle;
	}
}

vk::SurfaceFormatKHR pickSwapChainSurfaceFormat(std::vector<vk::SurfaceFormatKHR*> formats)
{
	TS_CORE_ASSERT(formats.size() != 0, "assert : in SDLWindow::pickSwapChainSurfaceFormat(...), formats.size() is 0. No formats available.");
	for (vk::SurfaceFormatKHR* format : formats) {
		if (format->colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear && format->format == vk::Format::eB8G8R8A8Unorm)
			return *format;
	}

	return *formats[0];
}

vk::PresentModeKHR pickSwapChainPresentMode(std::vector<vk::PresentModeKHR*> presentModes)
{
	TS_CORE_ASSERT(presentModes.size() != 0, "assert : in SDLWindow::pickSwapChainPresentMode(...), presentModes.size() is 0. No presentMode available.");
	for (vk::PresentModeKHR* pm : presentModes) {
		if (*pm == vk::PresentModeKHR::eMailbox)
			return *pm;
	}

	for (vk::PresentModeKHR* pm : presentModes) {
		if (*pm == vk::PresentModeKHR::eFifo)
			return *pm;
	}

	return *presentModes[0];
}

vk::Extent2D pickSwapChainExtent2D(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR* capabilities)
{
	vk::Extent2D minImageExt = capabilities->minImageExtent;
	vk::Extent2D maxImageExt = capabilities->maxImageExtent;
	vk::Extent2D currImageExt = capabilities->currentExtent;

	// If the current extent is already set, use that
	if (currImageExt.width != UINT32_MAX)
		return currImageExt;
	else {
		// Otherwise, pick the closest extent within the allowed range
		vk::Extent2D extent = { width, height };
		extent.width = std::clamp(extent.width, minImageExt.width, maxImageExt.width);
		extent.height = std::clamp(extent.height, minImageExt.height, maxImageExt.height);
		return extent;
	}
}