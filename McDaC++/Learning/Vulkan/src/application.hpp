#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
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
	Application();
	~Application();

	void run();

	void printRequiredExtensions();
	void printAllExtensions();
	void printLayerSupport();

private:
	void createWindow();
	void initVulkan();
	
	void createInstance();
	void createDebugCallback();
	void createSurface();
	void getPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createGraphicsPipeline();

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

	std::vector<char> readFile(const std::string_view &&filename);
	vk::ShaderModule createShaderModule(const std::vector<char> &code);

private:
	// Vulkan
	vk::ApplicationInfo m_appInfo;
	vk::Instance m_instance;
	vk::SurfaceKHR m_surface;
	vk::PhysicalDevice m_physicalDevice;
	vk::Device m_logicalDevice;

	vk::DebugUtilsMessengerEXT m_debugMessenger;
	vk::DispatchLoaderDynamic m_dynamicLoader;

	vk::Queue m_graphicsQueue;
	vk::Queue m_presentQueue;

	vk::SwapchainKHR m_swapChain;
	vk::Extent2D m_swapChainExtent;
	vk::Format m_swapChainImageFormat;
	std::vector<vk::Image> m_swapChainImages;
	std::vector<vk::ImageView> m_swapChainImageViews; // Possibly use pointers later

	// GLFW
	GLFWwindow *m_window;
};
