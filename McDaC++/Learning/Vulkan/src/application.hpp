#pragma once

#define VK_ENABLE_BETA_EXTENSIONS // Enable beta extensions (VK_KHR_portability_subset)
#define VULKAN_HPP_DISABLE_ENHANCED_MODE // Only allow object-oriented versions of existing functions and nothing else
#define VULKAN_HPP_NO_EXCEPTIONS // Disable exceptions (only use assertions)
#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_NONE // GLFW doesn't include any OpenGL or Vulkan headers
#include <GLFW/glfw3.h>
#include <optional>
#include <vector>

#define GLM_FORCE_RADIANS // GLM functions use radians instead of degrees
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // GLM uses depths of (0.0 - 1.0) instead of (-1.0 - 1.0)
// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // GLM data types use glsl alignment requirements // Better to be explicit
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL // Enable experimental features (hashing functions)
#include <glm/gtx/hash.hpp>

#define ENABLE_PIPELINE_DYNAMIC_STATE 1
#define ENABLE_MULTISAMPLING 0
#define ENABLE_MIPMAPS 1
#define ENABLE_APIDUMP 0

#ifdef _DEBUG
	#if ENABLE_APIDUMP
		#define NUM_VALIDATION_LAYERS 2
	#else
		#define NUM_VALIDATION_LAYERS 1
	#endif
#endif

#define NUM_DEVICE_EXTENSIONS 5

#define VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"
#define VK_LAYER_LUNARG_API_DUMP_LAYER_NAME "VK_LAYER_LUNARG_api_dump"

#define VERTEX_INPUT_BINDING_DESCRIPTION_COUNT 1
#define VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT 3

#define VERTEX_INPUT_POSITION_ATTRIBUTE_LOCATION 0
#define VERTEX_INPUT_POSITION_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_POSITION_ATTRIBUTE_FORMAT vk::Format::eR32G32B32Sfloat

#define VERTEX_INPUT_COLOR_ATTRIBUTE_LOCATION 1
#define VERTEX_INPUT_COLOR_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_COLOR_ATTRIBUTE_FORMAT vk::Format::eR32G32B32Sfloat

#define VERTEX_INPUT_TEX_COORD_ATTRIBUTE_LOCATION 2
#define VERTEX_INPUT_TEX_COORD_ATTRIBUTE_BINDING 0
#define VERTEX_INPUT_TEX_COORD_ATTRIBUTE_FORMAT vk::Format::eR32G32Sfloat

#define PIPELINE_SHADER_STAGE_COUNT 2
#define PIPELINE_VIEWPORT_COUNT 1
#define PIPELINE_SCISSOR_COUNT 1
#define PIPELINE_COLOR_BLEND_ATTACHMENT_COUNT 1
#define PIPELINE_DYNAMIC_STATE_COUNT 2
#define MAX_FRAMES_IN_FLIGHT 2

struct QueueFamilyIndices
{
	// Only want graphics family queue
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;
	std::optional<uint32_t> computeFamily;

	[[nodiscard]] constexpr bool isComplete() const noexcept 
	{
		return 	graphicsFamily.has_value() && 
				presentFamily.has_value() && 
				transferFamily.has_value() && 
				computeFamily.has_value();
	}
};

struct QueueHandles
{
	vk::Queue m_graphicsQueue;
	vk::Queue m_presentQueue;
	vk::Queue m_transferQueue; // Could just use graphics queue, but I'm being explicit with the queues
	vk::Queue m_computeQueue;
};

struct SwapChainSupportDetails
{
	constexpr ~SwapChainSupportDetails() noexcept
	{
		delete[] formats;
		delete[] presentModes;
	}

	vk::SurfaceCapabilities2KHR capabilities; // Basic surface capabilities (min/max number of images in swap chain, min/max width and height of images)
	uint32_t formatCount;
	vk::SurfaceFormat2KHR *formats = nullptr; // Surface formats (pixel format, color space)
	uint32_t presentModeCount;
	vk::PresentModeKHR *presentModes = nullptr; // Available presentation modes
};

struct GraphicsPipelineInfo
{
	vk::PipelineShaderStageCreateInfo stages[PIPELINE_SHADER_STAGE_COUNT];
	vk::PipelineVertexInputStateCreateInfo vertexInputState;
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
	vk::PipelineTessellationStateCreateInfo tessellationState;
	vk::PipelineViewportStateCreateInfo viewportState;
	vk::PipelineRasterizationStateCreateInfo rasterizationState;
	vk::PipelineMultisampleStateCreateInfo multisampleState;
	vk::PipelineDepthStencilStateCreateInfo depthStencilState;
	vk::PipelineColorBlendStateCreateInfo colorBlendState;
	vk::PipelineDynamicStateCreateInfo dynamicState;
	vk::PipelineLayout layout;
	vk::RenderPass renderPass;
	uint32_t subpass = 0;

	vk::ShaderModule vertexShaderModule;
	vk::ShaderModule fragmentShaderModule;
	vk::VertexInputBindingDescription vertexBindingDescription;
	std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> vertexAttributeDescriptions;
	vk::Viewport viewports[PIPELINE_VIEWPORT_COUNT];
	vk::Rect2D scissors[PIPELINE_SCISSOR_COUNT];
	vk::PipelineColorBlendAttachmentState colorBlendAttachments[PIPELINE_COLOR_BLEND_ATTACHMENT_COUNT];
	std::array<float, 4> colorBlendConstants;
#if ENABLE_PIPELINE_DYNAMIC_STATE
	vk::DynamicState dynamicStates[PIPELINE_DYNAMIC_STATE_COUNT];
#endif
	vk::PipelineLayoutCreateInfo layoutInfo;

	bool created = false;
};

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;

	[[nodiscard]] constexpr static vk::VertexInputBindingDescription getBindingDescription() noexcept
	{
		// Describes at which rate to load data from memory throughout the vertices
		return {
			0, // Binding: the index of the binding in the array of bindings
			sizeof(Vertex), // Stride: the number of bytes from one entry to the next
			vk::VertexInputRate::eVertex // Input rate: whether to move the the next data entry after each vertex or instance
		};
	}

	[[nodiscard]] constexpr static std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> getAttributeDescriptions() noexcept
	{
		// Describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
		return { 
			vk::VertexInputAttributeDescription{
				VERTEX_INPUT_POSITION_ATTRIBUTE_LOCATION, // Location: references the location directive of the input in the vertex shader
				VERTEX_INPUT_POSITION_ATTRIBUTE_BINDING, // Binding: which binding (vk::VertexInputBindingDescription) the per-vertex data comes
				VERTEX_INPUT_POSITION_ATTRIBUTE_FORMAT, // Format: the type of data for the attribute; specified using the same enumeration as color formats
				offsetof(Vertex, position) // Offset: the number of bytes since the start of the per-vertex data to read from (using the offsetof() macro)
			},
			{
				VERTEX_INPUT_COLOR_ATTRIBUTE_LOCATION,
				VERTEX_INPUT_COLOR_ATTRIBUTE_BINDING,
				VERTEX_INPUT_COLOR_ATTRIBUTE_FORMAT,
				offsetof(Vertex, color)
			},
			{
				VERTEX_INPUT_TEX_COORD_ATTRIBUTE_LOCATION,
				VERTEX_INPUT_TEX_COORD_ATTRIBUTE_BINDING,
				VERTEX_INPUT_TEX_COORD_ATTRIBUTE_FORMAT,
				offsetof(Vertex, texCoord)
			}
		};
	}

	constexpr bool operator==(const Vertex &other) const noexcept
	{
		return position == other.position && color == other.color && texCoord == other.texCoord;
	}
};

// Needed for hashing the vertex struct
namespace std
{
	template<>
	struct hash<Vertex> 
	{
		size_t operator()(Vertex const& vertex) const noexcept
		{
			return ((hash<glm::vec3>()(vertex.position) ^
				   (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				   (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

using Index = uint32_t;

class Application
{
public:
	Application() noexcept;
	~Application() noexcept;

	void run() noexcept;
	void setFramebufferResized() noexcept;

#ifdef _DEBUG
	void addDebugCallback() noexcept;
	void addAllocationCallback() noexcept;
	void addReallocationCallback() noexcept;
	void addDeallocationCallback() noexcept;
#endif

private:
	void createWindow() noexcept;
	void initVulkan() noexcept;
	void drawFrame() noexcept;
	void printFPS() noexcept;
	void destroySwapChain() noexcept;
	void recreateSwapChain() noexcept;
	void writePipelineCache() noexcept;

	template <class T>
	void createAndFillBuffer(const T * const buffer_data, const size_t count, const vk::BufferUsageFlags usage, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) noexcept;
	void createBuffer(const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) noexcept;
	void copyBuffer(const vk::Buffer &src_buffer, const vk::Buffer &dst_buffer, const vk::DeviceSize size) noexcept;
	void updateUniformBuffer(const uint32_t current_image) noexcept;

	void createImage(const uint32_t width, const uint32_t height, const uint32_t mip_levels, const vk::SampleCountFlagBits sample_count, const vk::Format format, const vk::ImageTiling tiling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &image_memory) noexcept;
	void transitionImageLayout(const vk::Image image, const vk::Format format, const vk::ImageAspectFlags image_aspect_flags, const vk::ImageLayout old_layout, const vk::ImageLayout new_layout, const uint32_t mip_levels) noexcept;
	void copyBufferToImage(const vk::Buffer src_buffer, const vk::Image dst_image, const uint32_t width, const uint32_t height) noexcept;
#if ENABLE_MIPMAPS
	void generateMipmaps(const vk::Image image, const vk::Format image_format, int tex_width, int tex_height, const uint32_t mip_levels) noexcept;
#endif

	[[nodiscard]] vk::CommandBuffer beginSingleTimeCommand(const bool need_graphics) noexcept;
	void endSingleTimeCommand(const vk::CommandBuffer command_buffer, const bool need_graphics) noexcept;

	void createInstance() noexcept;
#ifdef _DEBUG
	void createDebugCallback();
#endif
	void createSurface() noexcept;
	void choosePhysicalDevice() noexcept;
	void createLogicalDevice() noexcept;
	void createSwapChain() noexcept;
	void createSwapChainImageViews() noexcept;
	void createRenderPass() noexcept;
	void createDescriptorSetLayout() noexcept;
	void createPipelineCache() noexcept;
	void createGraphicsPipeline() noexcept;
	void createFrameBuffers() noexcept;
	void createCommandPools() noexcept;
#if ENABLE_MULTISAMPLING
	void createColorResources() noexcept;
#endif
	void createDepthResources() noexcept;
	void createTextureImage() noexcept;
	void createTextureImageView() noexcept;
	void createTextureSampler() noexcept;
	void loadModel() noexcept;
	void createVertexBuffer() noexcept;
	void createIndexBuffer() noexcept;
	void createUniformBuffers() noexcept;
	void createDescriptorPool() noexcept;
	void createDescriptorSets() noexcept;
	void createCommandBuffers() noexcept;
	void createSyncObjects() noexcept;

	template <size_t S> 
	[[nodiscard]] bool areLayersSupported(const char * const * const layers) noexcept;
	[[nodiscard]] std::vector<const char *> getRequiredInstanceExtensions() noexcept;
	[[nodiscard]] bool physicalDeviceSupportsExtensions(const vk::PhysicalDevice &physical_device) noexcept;

	[[nodiscard]] bool physicalDeviceSupportsRequirements(const vk::PhysicalDevice &physical_device) noexcept;
	[[nodiscard]] SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &physical_device) noexcept;
	[[nodiscard]] bool swapChainAdequate(const SwapChainSupportDetails &details) noexcept;

	[[nodiscard]] QueueFamilyIndices getQueueFamilies(const vk::PhysicalDevice &physical_device) noexcept;

	[[nodiscard]] constexpr vk::SurfaceFormat2KHR chooseSwapSurfaceFormat(const vk::SurfaceFormat2KHR * const formats, const uint32_t count) noexcept;
	[[nodiscard]] constexpr vk::PresentModeKHR chooseSwapPresentMode(const vk::PresentModeKHR * const present_modes, const uint32_t count) noexcept;
	[[nodiscard]] vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilities2KHR &capabilities) noexcept;

	[[nodiscard]] std::vector<char> readFile(const std::string_view &&filename) noexcept;
	[[nodiscard]] vk::ShaderModule createShaderModule(const std::vector<char> &code) noexcept;

	[[nodiscard]] constexpr uint32_t findMemoryType(const uint32_t type_filter, const vk::MemoryPropertyFlags required_properties) noexcept;
	[[nodiscard]] vk::ImageView createImageView(const vk::Image image, const vk::Format format, const vk::ImageAspectFlags aspect_flags, const uint32_t mip_levels) noexcept;
	[[nodiscard]] vk::Format findSupportedFormat(const vk::Format * const candidate_formats, const uint32_t count, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) noexcept;
	[[nodiscard]] vk::Format findDepthFormat() noexcept;
	[[nodiscard]] constexpr bool hasStencilComponent(const vk::Format format) noexcept;
	[[nodiscard]] constexpr vk::SampleCountFlagBits getMaxUsableSampleCount() noexcept;

private:
	// GLFW
	GLFWwindow *m_window;

	// Vulkan
	vk::Instance m_instance;
	vk::SurfaceKHR m_surface;
	vk::DispatchLoaderDynamic m_dynamicLoader;
	vk::DebugUtilsMessengerEXT m_debugMessenger;
	vk::AllocationCallbacks m_allocationCallbacks;

	vk::PhysicalDevice m_physicalDevice;
	vk::PhysicalDeviceFeatures2 m_physicalDeviceFeatures;
	vk::PhysicalDeviceProperties2 m_physicalDeviceProperties;
	vk::PhysicalDeviceMemoryProperties2 m_physicalDeviceMemoryProperties;
	vk::PhysicalDeviceDriverPropertiesKHR m_physicalDeviceDriverProperties;
	vk::PhysicalDevicePortabilitySubsetFeaturesKHR m_physicalDevicePortabilitySubsetFeatures;
	vk::PhysicalDevicePortabilitySubsetPropertiesKHR m_physicalDevicePortabilitySubsetProperties;
	vk::Device m_logicalDevice;

	QueueHandles m_queueHandles;
	QueueFamilyIndices m_queueFamilyIndices;

	vk::SwapchainKHR m_swapChain;
	vk::Extent2D m_swapChainExtent;
	vk::Format m_swapChainImageFormat;
	uint32_t m_swapChainImageCount;
	vk::Image *m_swapChainImages = nullptr; // Size = m_swapChainImageCount
	vk::ImageView *m_swapChainImageViews = nullptr;

	GraphicsPipelineInfo m_parentGraphicsPipelineInfo;
	vk::Pipeline m_parentGraphicsPipeline;
	vk::Pipeline m_childGraphicsPipeline;
	vk::PipelineCache m_graphicsPipelineCache;
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

	std::vector<Vertex> m_vertices;
	std::vector<Index> m_indices;

	vk::Buffer *m_uniformBuffers = nullptr;
	vk::DeviceMemory *m_uniformBuffersMemory = nullptr;

	vk::Image m_textureImage;
	vk::DeviceMemory m_textureImageMemory;
	vk::ImageView m_textureImageView;
	vk::Sampler m_textureSampler;
#if ENABLE_MIPMAPS
	uint32_t m_textureMipLevels;
#endif

	vk::Image m_depthImage;
	vk::DeviceMemory m_depthImageMemory;
	vk::ImageView m_depthImageView;

#if ENABLE_MULTISAMPLING
	vk::Image m_colorImage;
	vk::DeviceMemory m_colorImageMemory;
	vk::ImageView m_colorImageView;
#endif

	vk::DescriptorSetLayout m_descriptorSetLayout;
	vk::DescriptorPool m_descriptorPool;
	vk::DescriptorSet *m_descriptorSets = nullptr;

#if ENABLE_MULTISAMPLING
	vk::SampleCountFlagBits m_msaaSamples;
#endif

#ifdef _DEBUG
	uint64_t m_debugCallbackCount = 0;
	uint64_t m_debugAllocationCount = 0;
	uint64_t m_debugReallocationCount = 0;
	uint64_t m_debugDeallocationCount = 0;

	const char * const m_validationLayers[NUM_VALIDATION_LAYERS] = {
		VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME
#if ENABLE_APIDUMP
		, VK_LAYER_LUNARG_API_DUMP_LAYER_NAME
#endif
	};
#endif

	const char * const m_deviceExtensions[NUM_DEVICE_EXTENSIONS] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
		VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME,
		VK_GOOGLE_DISPLAY_TIMING_EXTENSION_NAME,
		VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
	};

	// Performance
	std::chrono::steady_clock::time_point m_previousTimeFPS;
	uint32_t m_frameCountFPS;
};
