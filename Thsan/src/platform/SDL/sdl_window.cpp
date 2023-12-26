#pragma once


#include "pch.h"

#include "sdl_window.h"
#include "SDL.h"
#include "thsan/log.h"

#include <GL/glew.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "thsan/graphics/graphic_api.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/game.h"

#include "thsan/Input/event.h"
#include <set>

#ifdef _WIN32
#pragma comment(linker, "/subsystem:windows")
#define VK_USE_PLATFORM_WIN32_KHR
#define PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif

Thsan::SDLWindow::SDLWindow() {

}

Thsan::SDLWindow::~SDLWindow() {
	if (window) {
		close();
	}
}


bool Thsan::SDLWindow::create(const int width, const int height, const char* title)
{
	bool success = true;
	TS_CORE_ASSERT(width > 0 || height > 0,
		"Error: cannot create window context. width and height must both be bigger than 0.");

	this->width = width;
	this->height = height;
	this->title = title;

	setSize(width, height);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		TS_CORE_ERROR("error initiliazing SDL2: {}", SDL_GetError());
		return false;
	}

	//mettre SDL dans window
	SDL_version version;
	SDL_VERSION(&version);
	TS_CORE_TRACE("SDL {}.{}.{}", version.major, version.minor, version.patch);

	success = setGraphicAPI(get_graphic_API());

	return success;
}

void Thsan::SDLWindow::setTitle(const char* title)
{
	this->title = title;
	SDL_SetWindowTitle(window, title);
}

void Thsan::SDLWindow::setSize(const int width, const int height)
{
	SDL_SetWindowSize(window, width, height);
}

void Thsan::SDLWindow::close()
{
	if (get_graphic_API() == GraphicAPI::vulkan)
		destroyContextVulkan();
	else if (get_graphic_API() == GraphicAPI::openGL)
		destroyContextOpenGL();

	SDL_DestroyWindow(window);

	window = nullptr;
	SDL_Quit();
}

bool Thsan::SDLWindow::isRunning()
{
	return running;
}

void Thsan::SDLWindow::swapBuffers()
{
	SDL_GL_SwapWindow(window);
}

std::vector<Thsan::Event> Thsan::SDLWindow::pollEvent() {
	std::vector<Thsan::Event> events;
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		Event tmp_event;
		if (e.type == SDL_QUIT) {
			running = false;
		}

		if (e.type == SDL_KEYUP) {
			tmp_event.key.code = static_cast<Key>(e.key.keysym.scancode);
			tmp_event.type = Event::KeyReleased;

		}
		else if (e.type == SDL_KEYDOWN) {
			tmp_event.key.code = static_cast<Key>(e.key.keysym.scancode);
			tmp_event.type = Event::KeyPressed;
		}

		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
			int width = e.window.data1;
			int height = e.window.data2;
			if (resizeCallback) {
				resizeCallback(width, height);
			}
			
		}

		events.push_back(tmp_event);
	}

	return events;
}

void Thsan::SDLWindow::setResizeCallback(std::function<void(int, int)> callback)
{
	resizeCallback = callback;
};

bool Thsan::SDLWindow::createContextOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	window = SDL_CreateWindow("my application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		TS_CORE_ERROR("Error creating window: {}", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	glContext = SDL_GL_CreateContext(window);

	int major_version, minor_version = -1;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version);

	TS_CORE_INFO("OpenGL - version {}.{}\n", major_version, minor_version);

	glewExperimental = GL_TRUE;
	glewInit();
	return true;
}


bool isDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return deviceFeatures.tessellationShader;
}

bool Thsan::SDLWindow::createInstance()
{

	// Get WSI extensions from SDL (we can add more if we like - we just can't remove these)
	unsigned extension_count;
	if (!SDL_Vulkan_GetInstanceExtensions(window, &extension_count, NULL)) {
		TS_CORE_ERROR("Could not get the number of required instance extensions from SDL.\n");
		return false;
	}

	std::vector<const char*> extensions(extension_count);
	if (!SDL_Vulkan_GetInstanceExtensions(window, &extension_count, extensions.data())) {
		TS_CORE_ERROR("Could not get the names of required instance extensions from SDL.\n");
		return false;
	}

	TS_CORE_INFO("Device support {} extension", extension_count);
	for (auto ext : extensions)
		TS_CORE_INFO("- {}", ext);


	std::vector<vk::LayerProperties> supported_layers = vk::enumerateInstanceLayerProperties();

	TS_CORE_INFO("Device support {} layers", supported_layers.size());
	for (auto layer : supported_layers)
		TS_CORE_INFO("- {}", layer.layerName);

	// Use validation layers if this is a debug build
	std::vector<const char*> layers;
#if defined(TS_CONFIG_DEBUG)
	layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
	// vk::ApplicationInfo allows the programmer to specifiy some basic information about the
	// program, which can be useful for layers and tools to provide more debug information.
	vk::ApplicationInfo appInfo = vk::ApplicationInfo()
		.setPApplicationName("THSAN_CLIENT")
		.setApplicationVersion(1)
		.setPEngineName("THSAN_CORE")
		.setEngineVersion(1)
		.setApiVersion(VK_API_VERSION_1_0);

	// vk::InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
	 // are needed.
	vk::InstanceCreateInfo instInfo = vk::InstanceCreateInfo()
		.setFlags(vk::InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
		.setPpEnabledExtensionNames(extensions.data())
		.setEnabledLayerCount(static_cast<uint32_t>(layers.size()))
		.setPpEnabledLayerNames(layers.data());

	// Create the Vulkan instance.
	instance = new vk::Instance();
	try {
		*instance = vk::createInstance(instInfo);
	}
	catch (const std::exception& e) {
		TS_CORE_ERROR("Could not create a Vulkan instance: {}\n", e.what());
		return false;
	}

	return true;
}


bool Thsan::SDLWindow::createSurface()
{
	// Create a Vulkan surface for rendering
	surface = new vk::SurfaceKHR();
	VkSurfaceKHR c_surface;
	if (!SDL_Vulkan_CreateSurface(window, static_cast<VkInstance>(*instance), &c_surface)) {
		TS_CORE_ERROR("Could not create a Vulkan surface.");
		return false;
	}

	*surface = c_surface;

	return true;
}
bool Thsan::SDLWindow::selectPhysicalDevice()
{

	//pick a physical device
	physicalDevice = new vk::PhysicalDevice();

	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(*instance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0) {
		TS_CORE_ERROR("Error: deviceCount is equal to 0, failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> physical_devices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(*instance, &physicalDeviceCount, physical_devices.data());

	for (const auto& device : physical_devices) {
		if (isDeviceSuitable(device)) {
			*physicalDevice = device;
			break;
		}
	}

	//show the physical device that are available
	TS_CORE_INFO("There is {} Physical Device that are available, them being: ", physical_devices.size());

	//get the properties of selected physical device
	VkPhysicalDeviceProperties physicalDeviceProperties;
	if (physicalDevice)
		vkGetPhysicalDeviceProperties(*physicalDevice, &physicalDeviceProperties);

	for (const VkPhysicalDevice& temp_physicalDevice : physical_devices) {

		VkPhysicalDeviceProperties temp_deviceProperties;
		vkGetPhysicalDeviceProperties(temp_physicalDevice, &temp_deviceProperties);

		if (physicalDevice) {
			if (physicalDeviceProperties.deviceID == temp_deviceProperties.deviceID) {
				TS_CORE_INFO("- {} *", temp_deviceProperties.deviceName);
			}
		}
		else {
			TS_CORE_INFO("- {}", temp_deviceProperties.deviceName);
		}
	}

	if (!physicalDevice) {
		TS_CORE_ERROR("Error: physicalDevice is VK_NULL_HANDLE, failed to find a suitable GPU!");
	}

	return true;
}

bool Thsan::SDLWindow::createLogicalDevice()
{
	device = new vk::Device();
	
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
	std::vector<uint32_t> uniqueIndices;
	uniqueIndices.push_back(indices.graphicsFamily.value());
	if (indices.graphicsFamily.value() != indices.presentFamily.value()) {
		uniqueIndices.push_back(indices.presentFamily.value());
	}

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo{};
	float queuePriority = 1.0f;

	for (uint32_t queueFamIndex: uniqueIndices) {
		vk::DeviceQueueCreateInfo queueCreateInfoElem;
		queueCreateInfoElem.queueFamilyIndex = queueFamIndex;
		queueCreateInfoElem.queueCount = 1;
		queueCreateInfoElem.pQueuePriorities = &queuePriority;

		queueCreateInfo.push_back(queueCreateInfoElem);
	}

	std::vector<const char*> deviceExt = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};

	vk::PhysicalDeviceFeatures deviceFeatures{};

	vk::DeviceCreateInfo createInfo{};
	createInfo.queueCreateInfoCount = queueCreateInfo.size();
	createInfo.pQueueCreateInfos = queueCreateInfo.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	std::vector<const char*> layers;
#if defined(TS_CONFIG_DEBUG)
	layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

	createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
	createInfo.ppEnabledLayerNames = layers.data();
	createInfo.enabledExtensionCount = deviceExt.size();
	createInfo.ppEnabledExtensionNames = deviceExt.data();

	try {
		*device = physicalDevice->createDevice(createInfo);
		TS_CORE_INFO("Device succesfully created!");

		return true;
	}
	catch (vk::SystemError err) {
		TS_CORE_ERROR("Error in device creation: {}", err.what());
		return false;
	}
}

bool Thsan::SDLWindow::createContextVulkan()
{
	// Load the Vulkan library
	if (SDL_Vulkan_LoadLibrary(nullptr) != 0) {
		TS_CORE_ERROR("Failed to load Vulkan library: {}", SDL_GetError());
		return false;
	}

	// Initialize SDL video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		TS_CORE_ERROR("Failed to initialize SDL video subsystem: {}", SDL_GetError());
		return false;
	}

	// Create a Vulkan window
	window = SDL_CreateWindow(
		"Vulkan Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
	);

	if (!window) {
		TS_CORE_ERROR("Failed to create window: {}", SDL_GetError());
		return false;
	}

	// Query Vulkan API version
	uint32_t apiVersion = 0;
	vkEnumerateInstanceVersion(&apiVersion);
	TS_CORE_TRACE("Vulkan API version: {}.{}.{}", VK_VERSION_MAJOR(apiVersion), VK_VERSION_MINOR(apiVersion), VK_VERSION_PATCH(apiVersion));

	// Create a Vulkan instance
	if (!createInstance())
		return false;

	// Create a Vulkan surface
	if (!createSurface())
		return false;
	
	// Select a physical device
	if (!selectPhysicalDevice())
		return false;

	if (!createLogicalDevice())
		return false;

	graphicQueue = new vk::Queue();
	presentQueue = new vk::Queue();
	std::vector<vk::Queue> queues = getQueues(*physicalDevice, *device, surface);
	*graphicQueue = queues[0];
	*presentQueue = queues[1];
	
	swapchainBundle = createSwapChain(900, 600, physicalDevice, device, surface);

	return true;
}

void Thsan::SDLWindow::destroyContextOpenGL()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
}

void Thsan::SDLWindow::destroyContextVulkan()
{
	device->destroySwapchainKHR(*swapchainBundle.swapchain);
	device->destroy();
	instance->destroySurfaceKHR(*surface);
	SDL_DestroyWindow(window);
	instance->destroy();
}
