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
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // GLM uses depths of (0.0 - 1.0) instead of (-1.0 - 1.0)
// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // GLM data types use glsl allignment requirements // Better to be explicit
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL // Enable experimental features (hashing functions)
#include <glm/gtx/hash.hpp>

#ifdef _DEBUG
	#define NUM_VALIDATION_LAYERS 1
#endif

#ifdef __APPLE__
	#define NUM_DEVICE_EXTENSIONS 4
#else
	#define NUM_DEVICE_EXTENSIONS 3
#endif

#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#define VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

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

#define MAX_FRAMES_IN_FLIGHT 2

struct QueueFamilyIndices
{
	// Only want graphics family queue
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

	[[nodiscard]] constexpr bool isComplete() const noexcept {return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();}
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
		const vk::VertexInputBindingDescription vertex_input_binding_description{
			0, // Binding: the index of the binding in the array of bindings
			sizeof(Vertex), // Stride: the number of bytes from one entry to the next
			vk::VertexInputRate::eVertex // Input rate: whether to move the the next data entry after each vertex or instance
		};

		return vertex_input_binding_description;
	}

	[[nodiscard]] constexpr static std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> getAttributeDescriptions() noexcept
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

	bool operator==(const Vertex &other) const noexcept
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
		size_t operator()(Vertex const& vertex) const 
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

	void createImage(const uint32_t width, const uint32_t height, const uint32_t mip_levels, const vk::SampleCountFlagBits sample_count, const vk::Format format, const vk::ImageTiling tiling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &image_memory) RELEASE_NOEXCEPT;
	void transitionImageLayout(const vk::Image image, const vk::Format format, const vk::ImageAspectFlags image_aspect_flags, const vk::ImageLayout old_layout, const vk::ImageLayout new_layout, const uint32_t mip_levels) RELEASE_NOEXCEPT;
	void copyBufferToImage(const vk::Buffer src_buffer, const vk::Image dst_image, const uint32_t width, const uint32_t height) RELEASE_NOEXCEPT;
	void generateMipmaps(const vk::Image image, const vk::Format image_format, int tex_width, int tex_height, const uint32_t mip_levels) RELEASE_NOEXCEPT;

	[[nodiscard]] vk::CommandBuffer beginSingleTimeCommand(const bool need_graphics) RELEASE_NOEXCEPT;
	void endSingleTimeCommand(const vk::CommandBuffer command_buffer, const bool need_graphics) RELEASE_NOEXCEPT;
	
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
	void createColorResources() RELEASE_NOEXCEPT;
	void createDepthResources() RELEASE_NOEXCEPT;
	void createTextureImage() RELEASE_NOEXCEPT;
	void createTextureImageView() RELEASE_NOEXCEPT;
	void createTextureSampler() RELEASE_NOEXCEPT;
	void loadModel() RELEASE_NOEXCEPT;
	void createVertexBuffer() RELEASE_NOEXCEPT;
	void createIndexBuffer() RELEASE_NOEXCEPT;
	void createUniformBuffers() RELEASE_NOEXCEPT;
	void createDescriptorPool() RELEASE_NOEXCEPT;
	void createDescriptorSets() RELEASE_NOEXCEPT;
	void createCommandBuffers() RELEASE_NOEXCEPT;
	void createSyncObjects() RELEASE_NOEXCEPT;

	template <size_t S> 
	[[nodiscard]] bool areLayersSupported(const char * const * const layers) RELEASE_NOEXCEPT;
	[[nodiscard]] std::vector<const char *> getRequiredInstanceExtensions() RELEASE_NOEXCEPT;
	[[nodiscard]] bool physicalDeviceSupportsExtensions(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;

	[[nodiscard]] bool physicalDeviceSupportsRequirements(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;
	[[nodiscard]] SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;
	[[nodiscard]] bool swapChainAdequate(const SwapChainSupportDetails &details) RELEASE_NOEXCEPT;

	[[nodiscard]] QueueFamilyIndices getQueueFamilies(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT;

	[[nodiscard]] vk::SurfaceFormat2KHR chooseSwapSurfaceFormat(const vk::SurfaceFormat2KHR * const formats, const uint32_t count) RELEASE_NOEXCEPT;
	[[nodiscard]] vk::PresentModeKHR chooseSwapPresentMode(const vk::PresentModeKHR * const present_modes, const uint32_t count) RELEASE_NOEXCEPT;
	[[nodiscard]] vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilities2KHR &capabilities) RELEASE_NOEXCEPT;

	[[nodiscard]] std::vector<char> readFile(const std::string_view &&filename) RELEASE_NOEXCEPT;
	[[nodiscard]] vk::ShaderModule createShaderModule(const std::vector<char> &code) RELEASE_NOEXCEPT;

	[[nodiscard]] uint32_t findMemoryType(const uint32_t type_filter, const vk::MemoryPropertyFlags required_properties) RELEASE_NOEXCEPT;
	[[nodiscard]] vk::ImageView createImageView(const vk::Image image, const vk::Format format, const vk::ImageAspectFlags aspect_flags, const uint32_t mip_levels) RELEASE_NOEXCEPT;
	[[nodiscard]] vk::Format findSupportedFormat(const vk::Format * const candidate_formats, const uint32_t count, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) RELEASE_NOEXCEPT;
	[[nodiscard]] vk::Format findDepthFormat() RELEASE_NOEXCEPT;
	[[nodiscard]] bool hasStencilComponent(const vk::Format format) RELEASE_NOEXCEPT;
	[[nodiscard]] vk::SampleCountFlagBits getMaxUsableSampleCount() RELEASE_NOEXCEPT;

private:
	// GLFW
	GLFWwindow *m_window;

	// Vulkan
	vk::Instance m_instance;
	vk::SurfaceKHR m_surface;

	vk::PhysicalDevice m_physicalDevice;
	vk::PhysicalDeviceFeatures2 m_physicalDeviceFeatures;
	vk::PhysicalDeviceProperties2 m_physicalDeviceProperties;
	vk::PhysicalDeviceMemoryProperties2 m_physicalDeviceMemoryProperties;
	vk::PhysicalDeviceDriverPropertiesKHR m_physicalDeviceDriverProperties;
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

	std::vector<Vertex> m_vertices;
	std::vector<Index> m_indices;

	vk::Buffer *m_uniformBuffers = nullptr;
	vk::DeviceMemory *m_uniformBuffersMemory = nullptr;

	vk::Image m_textureImage;
	vk::DeviceMemory m_textureImageMemory;
	vk::ImageView m_textureImageView;
	vk::Sampler m_textureSampler;
	uint32_t m_textureMipLevels;

	vk::Image m_depthImage;
	vk::DeviceMemory m_depthImageMemory;
	vk::ImageView m_depthImageView;

	vk::Image m_colorImage;
	vk::DeviceMemory m_colorImageMemory;
	vk::ImageView m_colorImageView;

	vk::DescriptorPool m_descriptorPool;
	vk::DescriptorSet *m_descriptorSets = nullptr;

	vk::SampleCountFlagBits m_msaaSamples;

#ifdef _DEBUG
	const char * const m_validationLayers[NUM_VALIDATION_LAYERS] = {
		VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME
	};
#endif

	const char * const m_deviceExtensions[NUM_DEVICE_EXTENSIONS] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
		VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME
#ifdef __APPLE__
		, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#endif
	};

	// Preformance
	std::chrono::steady_clock::time_point m_previousTimeFPS;
	uint32_t m_frameCountFPS;
};
