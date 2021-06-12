#pragma once

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <optional>


#ifdef DEBUG
	#define NUM_VALIDATION_LAYERS 1
#endif

#ifdef __APPLE__
	#define NUM_DEVICE_EXTENSIONS 2
#else
	#define NUM_DEVICE_EXTENSIONS 1
#endif

#define MAX_FRAMES_IN_FLIGHT 2

struct QueueFamilyIndices
{
	// Only want graphics family queue
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	inline bool isComplete() const {return graphicsFamily.has_value() && presentFamily.has_value();}
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
	void printPhysicalDevices();

	void setFramebufferResized();

private:
	void createWindow();
	void initVulkan();
	void drawFrame();
	void printFPS();
	void destroySwapChain();
	void recreateSwapChain();
	
	void createInstance();
	void createDebugCallback();
	void createSurface();
	void choosePhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFrameBuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSyncObjects();

	template <size_t S> 
	bool areLayersSupported(const std::array<const char * const, S> &layers);
	std::vector<const char *> getRequiredInstanceExtensions();
	bool physicalDeviceSupportsRequirements(const vk::PhysicalDevice &device);
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
	// GLFW
	GLFWwindow *m_window;

	// Vulkan
	vk::Instance m_instance;
	vk::SurfaceKHR m_surface;

	vk::PhysicalDevice m_physicalDevice;
	vk::Device m_logicalDevice;

	vk::DebugUtilsMessengerEXT m_debugMessenger;
	vk::DispatchLoaderDynamic m_dynamicLoader;

	vk::Queue m_graphicsQueue;
	vk::Queue m_presentQueue;
	QueueFamilyIndices m_queueFamilyIndices;

	vk::SwapchainKHR m_swapChain;
	vk::Extent2D m_swapChainExtent;
	vk::Format m_swapChainImageFormat;
	std::vector<vk::Image> m_swapChainImages;
	std::vector<vk::ImageView> m_swapChainImageViews;

	vk::RenderPass m_renderPass;
	vk::PipelineLayout m_pipelineLayout;
	vk::Pipeline m_graphicsPipeline;
	std::vector<vk::Framebuffer> m_swapChainFramebuffers;

	vk::CommandPool m_commandPool;
	std::vector<vk::CommandBuffer> m_commandBuffers;

	vk::Semaphore m_imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
	vk::Semaphore m_renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
	vk::Fence m_inFlightFences[MAX_FRAMES_IN_FLIGHT];
	std::vector<vk::Fence> m_imagesInFlight;

	size_t m_currentFrame;
	bool m_framebufferResized;

#ifdef DEBUG
	const char * const m_validationLayers[NUM_VALIDATION_LAYERS];
#endif
	const char * const m_deviceExtensions[NUM_DEVICE_EXTENSIONS];

	// Preformance
	double m_previousTimeFPS;
	uint32_t m_frameCountFPS;
};
