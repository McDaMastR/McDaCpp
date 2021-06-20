#pragma once

#ifdef NDEBUG
	#define VULKAN_HPP_NO_EXCEPTIONS // Disable exceptions (only use assertions)
#endif

#define VULKAN_HPP_DISABLE_ENHANCED_MODE // Only allow object-oriented versions of existing functions and nothing else
#define RELEASE_NOEXCEPT VULKAN_HPP_NOEXCEPT_WHEN_NO_EXCEPTIONS // Noexcept if noexceptions in vulkan hpp (in release)
#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_NONE // GLFW doesn't include any OpenGL or Vulkan headers
#include <GLFW/glfw3.h>
#include <optional>
#include <vector>
#include <chrono>

#define GLM_FORCE_RADIANS // GLM functions use radians instead of degrees
// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // GLM data types use glsl allignment requirements // Better to be explicit
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _DEBUG
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
#define VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT 3

#define VERTEX_INPUT_POSITION_ATTRIBUTE_LOCATION 0
#define VERTEX_INPUT_POSITION_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_POSITION_ATTRIBUTE_FORMAT vk::Format::eR32G32Sfloat

#define VERTEX_INPUT_COLOR_ATTRIBUTE_LOCATION 1
#define VERTEX_INPUT_COLOR_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_COLOR_ATTRIBUTE_FORMAT vk::Format::eR32G32B32Sfloat

#define VERTEX_INPUT_TEX_COORD_ATTRIBUTE_LOCATION 2
#define VERTEX_INPUT_TEX_COORD_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_TEX_COORD_ATTRIBUTE_FORMAT vk::Format::eR32G32Sfloat

#define MAX_FRAMES_IN_FLIGHT 2
#define VERTEX_COUNT 4
#define INDEX_COUNT 6

struct QueueFamilyIndices
{
	// Only want graphics family queue
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

	constexpr bool isComplete() const noexcept {return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();}
};

struct SwapChainSupportDetails
{
	~SwapChainSupportDetails() noexcept
	{
		delete[] formats;
		delete[] presentModes;
	}

	vk::SurfaceCapabilitiesKHR capabilities; // Basic surface capabilities (min/max number of images in swap chain, min/max width and height of images)
	uint32_t formatCount;
	vk::SurfaceFormatKHR *formats = nullptr; // Surface formats (pixel format, color space)
	uint32_t presentModeCount;
	vk::PresentModeKHR *presentModes = nullptr; // Available presentation modes
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
	glm::vec2 texCoord;

	constexpr static vk::VertexInputBindingDescription getBindingDescription() noexcept
	{
		// Describes at which rate to load data from memory throughout the vertices
		const vk::VertexInputBindingDescription vertex_input_binding_description{
			0, // Binding: the index of the binding in the array of bindings
			sizeof(Vertex), // Stride: the number of bytes from one entry to the next
			vk::VertexInputRate::eVertex // Input rate: whether to move the the next data entry after each vertex or instance
		};

		return vertex_input_binding_description;
	}

	constexpr static std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> getAttributeDescriptions() noexcept
	{
		// Describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
		const vk::VertexInputAttributeDescription position_attribute{
			VERTEX_INPUT_POSITION_ATTRIBUTE_LOCATION, // Location: references the location directive of the input in the vertex shader
			VERTEX_INPUT_POSITION_ATTRIBUTE_BINDING, // Binding: which binding (vk::VertexInputBindingDescription) the per-vertex data comes
			VERTEX_INPUT_POSITION_ATTRIBUTE_FORMAT, // Format: the type of data for the attribute; specified using the same enumeration as color formats
			offsetof(Vertex, position) // Offset: the number of bytes since the start of the per-vertex data to read from (using the offsetof() macro)
		};

		const vk::VertexInputAttributeDescription color_attribute{
			VERTEX_INPUT_COLOR_ATTRIBUTE_LOCATION,
			VERTEX_INPUT_COLOR_ATTRIBUTE_BINDING,
			VERTEX_INPUT_COLOR_ATTRIBUTE_FORMAT,
			offsetof(Vertex, color)
		};

		const vk::VertexInputAttributeDescription tex_coord_attribute{
			VERTEX_INPUT_TEX_COORD_ATTRIBUTE_LOCATION,
			VERTEX_INPUT_TEX_COORD_ATTRIBUTE_BINDING,
			VERTEX_INPUT_TEX_COORD_ATTRIBUTE_FORMAT,
			offsetof(Vertex, texCoord)
		};

		const std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> vertex_input_attribute_descriptions{
			std::move(position_attribute),
			std::move(color_attribute),
			std::move(tex_coord_attribute)
		};

		return vertex_input_attribute_descriptions;
	}
};

using Index = uint16_t;

class Application
{
public:
	Application() RELEASE_NOEXCEPT;
	~Application() RELEASE_NOEXCEPT;

	void run() RELEASE_NOEXCEPT;
	void setFramebufferResized() RELEASE_NOEXCEPT;

private:
	void createWindow() RELEASE_NOEXCEPT;
	void initVulkan() RELEASE_NOEXCEPT;
	void drawFrame() RELEASE_NOEXCEPT;
	void printFPS() RELEASE_NOEXCEPT;
	void destroySwapChain() RELEASE_NOEXCEPT;
	void recreateSwapChain() RELEASE_NOEXCEPT;

	template <class T>
	void createAndFillBuffer(const T * const buffer_data, const size_t count, const vk::BufferUsageFlags usage, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) RELEASE_NOEXCEPT;
	void createBuffer(const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) RELEASE_NOEXCEPT;
	void copyBuffer(const vk::Buffer &src_buffer, const vk::Buffer &dst_buffer, const vk::DeviceSize size) RELEASE_NOEXCEPT;
	void updateUniformBuffer(const uint32_t current_image) RELEASE_NOEXCEPT;

	void createImage(const uint32_t width, const uint32_t height, const vk::Format format, const vk::ImageTiling tiling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &image_memory) RELEASE_NOEXCEPT;
	void transitionImageLayout(const vk::Image image, const vk::Format format, const vk::ImageLayout old_layout, const vk::ImageLayout new_layout) RELEASE_NOEXCEPT;
	void copyBufferToImage(const vk::Buffer src_buffer, const vk::Image dst_image, const uint32_t width, const uint32_t height) RELEASE_NOEXCEPT;

	vk::CommandBuffer beginSingleTimeCommand() RELEASE_NOEXCEPT;
	void endSingleTimeCommand(const vk::CommandBuffer command_buffer) RELEASE_NOEXCEPT;
	
	void createInstance() RELEASE_NOEXCEPT;
#ifdef _DEBUG
	void createDebugCallback();
#endif
	void createSurface() RELEASE_NOEXCEPT;
	void choosePhysicalDevice() RELEASE_NOEXCEPT;
	void createLogicalDevice() RELEASE_NOEXCEPT;
	void createSwapChain() RELEASE_NOEXCEPT;
	void createSwapChainImageViews() RELEASE_NOEXCEPT;
	void createRenderPass() RELEASE_NOEXCEPT;
	void createDescriptorSetLayout() RELEASE_NOEXCEPT;
	void createGraphicsPipeline() RELEASE_NOEXCEPT;
	void createFrameBuffers() RELEASE_NOEXCEPT;
	void createCommandPools() RELEASE_NOEXCEPT;
	void createTextureImage() RELEASE_NOEXCEPT;
	void createTextureImageView() RELEASE_NOEXCEPT;
	void createTextureSampler() RELEASE_NOEXCEPT;
	void createVertexBuffer() RELEASE_NOEXCEPT;
	void createIndexBuffer() RELEASE_NOEXCEPT;
	void createUniformBuffers() RELEASE_NOEXCEPT;
	void createDescriptorPool() RELEASE_NOEXCEPT;
	void createDescriptorSets() RELEASE_NOEXCEPT;
	void createCommandBuffers() RELEASE_NOEXCEPT;
	void createSyncObjects() RELEASE_NOEXCEPT;

	template <size_t S> 
	bool areLayersSupported(const char * const * const layers) RELEASE_NOEXCEPT;
	std::vector<const char *> getRequiredInstanceExtensions() RELEASE_NOEXCEPT;
	bool physicalDeviceSupportsExtensions(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;

	bool physicalDeviceSupportsRequirements(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;
	SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;
	bool swapChainAdequate(const SwapChainSupportDetails &details) RELEASE_NOEXCEPT;

	QueueFamilyIndices getQueueFamilies(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;

	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const vk::SurfaceFormatKHR * const formats, const uint32_t count) RELEASE_NOEXCEPT;
	vk::PresentModeKHR chooseSwapPresentMode(const vk::PresentModeKHR * const present_modes, const uint32_t count) RELEASE_NOEXCEPT;
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) RELEASE_NOEXCEPT;

	std::vector<char> readFile(const std::string_view &&filename) RELEASE_NOEXCEPT;
	vk::ShaderModule createShaderModule(const std::vector<char> &code) RELEASE_NOEXCEPT;

	uint32_t findMemoryType(const uint32_t type_filter, const vk::MemoryPropertyFlags required_properties) RELEASE_NOEXCEPT;
	vk::ImageView createImageView(const vk::Image image, const vk::Format format) RELEASE_NOEXCEPT;

private:
	// GLFW
	GLFWwindow *m_window;

	// Vulkan
	vk::Instance m_instance;
	vk::SurfaceKHR m_surface;

	vk::PhysicalDevice m_physicalDevice;
	vk::PhysicalDeviceProperties m_physicalDeviceProperties;
	vk::PhysicalDeviceFeatures m_physicalDeviceFeatures;
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
	vk::DeviceMemory *m_uniformBuffersMemory = nullptr;

	vk::Image m_textureImage;
	vk::DeviceMemory m_textureImageMemory;
	vk::ImageView m_textureImageView;
	vk::Sampler m_textureSampler;

	vk::DescriptorPool m_descriptorPool;
	vk::DescriptorSet *m_descriptorSets = nullptr;

	const Vertex m_vertices[VERTEX_COUNT] = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	const Index m_indices[INDEX_COUNT] = {
		0, 1, 2, 2, 3, 0
	};

#ifdef _DEBUG
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
