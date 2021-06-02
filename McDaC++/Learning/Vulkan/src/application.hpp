#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <optional>

struct QueueFamilyIndices
{
    // Only want graphics family queue
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    inline bool isComplete() const {return graphicsFamily.has_value() && presentFamily.has_value();}
    constexpr inline uint32_t queueCount() const {return 1;} // Keep up to date! // graphics and present are 1 queue
};

struct SwapChainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities; // Basic surface capabilities (min/max number of images in swap chain, min/max width and height of images)
	std::vector<vk::SurfaceFormatKHR> formats; // Surface formats (pixel format, color space)
	std::vector<vk::PresentModeKHR> presentModes; // Available presentation modes
};

class Application
{
public:
    Application(const Application &) = delete;
    Application(const Application &&) = delete;
    void operator=(const Application &) = delete;
    void operator=(const Application &&) = delete;

    inline static void run() {return app._run();}

    inline static void printRequiredExtensions() {return app._printRequiredExtensions();}
    inline static void printAllExtensions() {return app._printAllExtensions();}
    inline static void printLayerSupport() {return app._printLayerSupport();}

private:
    Application();
    ~Application();

    void createWindow();
    void initVulkan();
    void createInstance();
    void createDebugCallback();
    void createSurface();
    void getPhysicalDevice();
    void createLogicalDevice();
	void createSwapChain();

    template <size_t S> 
    bool areLayersSupported(const std::array<const char * const, S> &layers);
    std::vector<const char *> getRequiredInstanceExtensions();
    bool deviceSupportsRequirements(const vk::PhysicalDevice &device);
    QueueFamilyIndices getQueueFamilies(const vk::PhysicalDevice &device);
	bool deviceSupportsExtensions(const vk::PhysicalDevice &device);

	SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &device);
	bool swapChainAdequate(const SwapChainSupportDetails &details);
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &formats);
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &present_modes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);

    void _run();

    void _printRequiredExtensions();
    void _printAllExtensions();
    void _printLayerSupport();

private:
    // Vulkan
    vk::ApplicationInfo m_appInfo;
    vk::Instance m_instance;
    vk::PhysicalDevice m_physicalDevice;
    vk::Device m_logicalDevice;
    vk::DebugUtilsMessengerEXT m_debugMessenger;
    vk::SurfaceKHR m_surface;
    vk::DispatchLoaderDynamic m_dynamicLoader;

    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;

    // GLFW
    GLFWwindow *m_window;

    // Application
    static Application app;
};
