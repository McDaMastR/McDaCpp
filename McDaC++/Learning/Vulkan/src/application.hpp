#pragma once

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_NONE // GLFW doesn't include any OpenGL or Vulkan headers
#include <GLFW/glfw3.h>
#include <optional>
#include <chrono>

#define GLM_FORCE_RADIANS // GLM functions use radians instead of degrees
// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // GLM data types use glsl allignment requirements // Better to be explicit
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef DEBUG
	#define NUM_VALIDATION_LAYERS 1
#endif

#ifdef __APPLE__
	#define NUM_DEVICE_EXTENSIONS 2
#else
	#define NUM_DEVICE_EXTENSIONS 1
#endif

#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#define VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

#define VERTEX_INPUT_BINDING_DESCRIPTION_COUNT 1
#define VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT 2

#define VERTEX_INPUT_POSITION_ATTRIBUTE_LOCATION 0
#define VERTEX_INPUT_POSITION_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_POSITION_ATTRIBUTE_FORMAT vk::Format::eR32G32Sfloat
#define VERTEX_INPUT_POSITION_ATTRIBUTE_OFFSET offsetof(Vertex, position)

#define VERTEX_INPUT_COLOR_ATTRIBUTE_LOCATION 1
#define VERTEX_INPUT_COLOR_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_COLOR_ATTRIBUTE_FORMAT vk::Format::eR32G32B32Sfloat
#define VERTEX_INPUT_COLOR_ATTRIBUTE_OFFSET offsetof(Vertex, color)

#define MAX_FRAMES_IN_FLIGHT 2
#define VERTEX_COUNT 4
#define INDEX_COUNT 6

struct QueueFamilyIndices
{
	// Only want graphics family queue
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

	constexpr bool isComplete() const {return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();}
};

struct SwapChainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities; // Basic surface capabilities (min/max number of images in swap chain, min/max width and height of images)
	std::vector<vk::SurfaceFormatKHR> formats; // Surface formats (pixel format, color space)
	std::vector<vk::PresentModeKHR> presentModes; // Available presentation modes
};

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;

	constexpr static vk::VertexInputBindingDescription getBindingDescription()
	{
		// Describes at which rate to load data from memory throughout the vertices
		const vk::VertexInputBindingDescription vertex_input_binding_description{
			0, // Binding: the index of the binding in the array of bindings
			sizeof(Vertex), // Stride: the number of bytes from one entry to the next
			vk::VertexInputRate::eVertex // Input rate: whether to move the the next data entry after each vertex or instance
		};

		return vertex_input_binding_description;
	}

	constexpr static std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> getAttributeDescriptions()
	{
		// Describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
		const vk::VertexInputAttributeDescription position_attribute{
			VERTEX_INPUT_POSITION_ATTRIBUTE_LOCATION, // Location: references the location directive of the input in the vertex shader
			VERTEX_INPUT_POSITION_ATTRIBUTE_BINDING, // Binding: which binding (vk::VertexInputBindingDescription) the per-vertex data comes
			VERTEX_INPUT_POSITION_ATTRIBUTE_FORMAT, // Format: the type of data for the attribute; specified using the same enumeration as color formats
			VERTEX_INPUT_POSITION_ATTRIBUTE_OFFSET // Offset: the number of bytes since the start of the per-vertex data to read from (using the offsetof() macro)
		};

		const vk::VertexInputAttributeDescription color_attribute{
			VERTEX_INPUT_COLOR_ATTRIBUTE_LOCATION,
			VERTEX_INPUT_COLOR_ATTRIBUTE_BINDING,
			VERTEX_INPUT_COLOR_ATTRIBUTE_FORMAT,
			VERTEX_INPUT_COLOR_ATTRIBUTE_OFFSET
		};

		const std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> vertex_input_attribute_descriptions{
			std::move(position_attribute),
			std::move(color_attribute)
		};

		return vertex_input_attribute_descriptions;
	}
};

using Index = uint16_t;

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

	template <class T>
	void createAndFillBuffer(const T * const buffer_data, const size_t count, const vk::BufferUsageFlags usage, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory);
	void createBuffer(const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &bufferMemory);
	void copyBuffer(const vk::Buffer &src_buffer, const vk::Buffer &dst_buffer, const vk::DeviceSize size);
	void updateUniformBuffer(const uint32_t current_image);
	
	void createInstance();
	void createDebugCallback();
	void createSurface();
	void choosePhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFrameBuffers();
	void createCommandPools();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void createSyncObjects();

	template <size_t S> 
	bool areLayersSupported(const std::array<const char * const, S> &layers);
	std::vector<const char *> getRequiredInstanceExtensions();
	bool deviceSupportsExtensions(const vk::PhysicalDevice &device);

	bool physicalDeviceSupportsRequirements(const vk::PhysicalDevice &device);
	SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &device);
	bool swapChainAdequate(const SwapChainSupportDetails &details);

	QueueFamilyIndices getQueueFamilies(const vk::PhysicalDevice &device);

	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &formats);
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &present_modes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);

	std::vector<char> readFile(const std::string_view &&filename);
	vk::ShaderModule createShaderModule(const std::vector<char> &code);

	uint32_t findMemoryType(const uint32_t type_filter, const vk::MemoryPropertyFlags required_properties);

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
	vk::Queue m_transferQueue; // Could just use graphics queue, but I'm being explicit with the queues
	QueueFamilyIndices m_queueFamilyIndices;

	vk::SwapchainKHR m_swapChain;
	vk::Extent2D m_swapChainExtent;
	vk::Format m_swapChainImageFormat;
	uint32_t m_swapChainImageCount;
	vk::Image *m_swapChainImages; // Size = m_swapChainImageCount
	vk::ImageView *m_swapChainImageViews = nullptr;

	vk::RenderPass m_renderPass;
	vk::DescriptorSetLayout m_descriptorSetLayout;

	vk::PipelineLayout m_pipelineLayout;
	vk::Pipeline m_graphicsPipeline;
	vk::Framebuffer *m_swapChainFramebuffers = nullptr;

	vk::CommandPool m_graphicsCommandPool;
	vk::CommandPool m_transferCommandPool;
	vk::CommandBuffer *m_commandBuffers = nullptr;

	vk::Semaphore m_imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
	vk::Semaphore m_renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
	vk::Fence m_inFlightFences[MAX_FRAMES_IN_FLIGHT];
	vk::Fence *m_imagesInFlight = nullptr;

	size_t m_currentFrame = 0;
	bool m_framebufferResized = false;

	vk::Buffer m_vertexBuffer;
	vk::DeviceMemory m_vertexBufferMemory;
	vk::Buffer m_indexBuffer;
	vk::DeviceMemory m_indexBufferMemory;
	vk::Buffer *m_uniformBuffers = nullptr;
	vk::DeviceMemory *m_uniformBuffersMemory;

	vk::DescriptorPool m_descriptorPool;
	vk::DescriptorSet *m_descriptorSets = nullptr;

	const Vertex m_vertices[VERTEX_COUNT] = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const Index m_indices[INDEX_COUNT] = {
		0, 1, 2, 2, 3, 0
	};

#ifdef DEBUG
	const char * const m_validationLayers[NUM_VALIDATION_LAYERS] = {
		VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME
	};
#endif

	const char * const m_deviceExtensions[NUM_DEVICE_EXTENSIONS] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
#ifdef __APPLE__
		, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#endif
	};

	// Preformance
	std::chrono::steady_clock::time_point m_previousTimeFPS;
	uint32_t m_frameCountFPS;
};
