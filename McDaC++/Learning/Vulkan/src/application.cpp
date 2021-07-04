#include "application.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"

#include <iostream>
#include <fstream>
#include <set>
#include <unordered_map>

#define SWAP_CHAIN_QUEUE_FAMILY_INDEX_COUNT 2
#define DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT 1
#define DRAW_FRAME_SUBMIT_INFO_SIGNAL_SEMAPHORE_COUNT 1
#define PRESENT_INFO_SWAP_CHAIN_COUNT 1
#define DESCRIPTOR_SET_LAYOUT_BINDING_COUNT 2
#define DESCRIPTOR_POOL_SIZE_COUNT 2
#define VERTEX_BUFFER_COUNT 1
#define DESCRIPTOR_SET_WRITE_COUNT 2
#define DEPTH_FORMAT_COUNT 3
#define CLEAR_VALUE_COUNT 2
#define GRAPHICS_PIPELINE_COUNT 2
#define RENDER_PASS_ATTACHMENT_COUNT FRAMEBUFFER_ATTACHMENT_COUNT

#if ENABLE_MULTISAMPLING
	#define FRAMEBUFFER_ATTACHMENT_COUNT 3
#else
	#define FRAMEBUFFER_ATTACHMENT_COUNT 2
#endif

#define CLEAR_COLOR_R 0.0f
#define CLEAR_COLOR_G 0.0f
#define CLEAR_COLOR_B 0.0f
#define CLEAR_COLOR_A 1.0f

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define VIKING_MODEL_PATH "assets/models/viking_room.obj"
#define VIKING_TEXTURE_PATH "assets/images/viking_room.png"

#ifdef NDEBUG
	#define APP_NAME "LearnVulkan"
#else
	#define APP_NAME "LearnVulkan-DEBUG"
#endif
#define ENGINE_NAME "No Engine"

#define APP_VERSION VK_MAKE_API_VERSION(0, 1, 0, 0)
#define ENGINE_VERSION VK_MAKE_API_VERSION(0, 1, 0, 0)

#ifdef _DEBUG
	#define DEBUG_LOG(x) std::cout << "\nDebug Log:\n\t" << x << "\n";
#else
	#define DEBUG_LOG(x)
#endif

#ifdef _DEBUG

#define DEBUG_CALLBACK(o, s, t, d) std::c##o << "\nDebug Callback:\n"\
											 << "\tServerity: " << vk::to_string(s)\
											 << "\n\tType: "    << vk::to_string(t)\
											 << "\n\n\t" << d << '\n'


static VKAPI_ATTR vk::Bool32 VKAPI_CALL vulkanDebugCallback(
	vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	vk::DebugUtilsMessageTypeFlagsEXT messageType,
	const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
#ifdef _DEBUG
	void *pUserData) noexcept
#else
	void * /* pUserData */) noexcept
#endif
{
	if (messageSeverity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError) [[unlikely]]
		DEBUG_CALLBACK(err, messageSeverity, messageType, pCallbackData->pMessage);
	else [[likely]]
		DEBUG_CALLBACK(out, messageSeverity, messageType, pCallbackData->pMessage);

#ifdef _DEBUG
	Application * const app = reinterpret_cast<Application *>(pUserData);
	app->addDebugCallback();
#endif

	return VK_FALSE; // Do not abort program
}

static VKAPI_ATTR void * VKAPI_CALL vulkanAllocateMemory(
#ifdef _DEBUG
	void *pUserData, 
#else
	void * /* pUserData */, 
#endif
	size_t size, 
	size_t alignment, 
#ifdef _DEBUG
	vk::SystemAllocationScope allocationScope) noexcept
#else
	vk::SystemAllocationScope /* allocationScope */) noexcept
#endif
{
#ifdef _DEBUG
	Application * const app = reinterpret_cast<Application *>(pUserData);
	app->addAllocationCallback();
#endif

	void *allocated_memory = nullptr;
	posix_memalign(&allocated_memory, alignment, size);
	DEBUG_LOG("Allocating Vulkan host memory; Allocation scope: " << vk::to_string(allocationScope) << ", Pointer: " << allocated_memory);
	return allocated_memory;
}

static VKAPI_ATTR void * VKAPI_CALL vulkanReallocateMemory(
#ifdef _DEBUG
	void *pUserData, 
#else
	void * /* pUserData */, 
#endif
	void *pOriginal,
	size_t size, 
	size_t alignment,
#ifdef _DEBUG
	vk::SystemAllocationScope allocationScope) noexcept
#else
	vk::SystemAllocationScope /* allocationScope */) noexcept
#endif
{
#ifdef _DEBUG
	Application * const app = reinterpret_cast<Application *>(pUserData);
	app->addReallocationCallback();
#endif

	void *reallocated_memory = realloc(pOriginal, size);
	std::align(alignment, size, reallocated_memory, size);
	DEBUG_LOG("Reallocating Vulkan host memory; Allocation scope: " << vk::to_string(allocationScope) << ", Pointer: " << reallocated_memory);
	return reallocated_memory;
}

static VKAPI_ATTR void VKAPI_CALL vulkanFreeMemory(
#ifdef _DEBUG
	void *pUserData, 
#else
	void * /* pUserData */, 
#endif
	void *pMemory) noexcept
{
#ifdef _DEBUG
	Application * const app = reinterpret_cast<Application *>(pUserData);
	app->addDeallocationCallback();
#endif

	free(pMemory);
	DEBUG_LOG("Freeing Vulkan host memory; address: " << pMemory);
}

static void glfwErrorCallback(int error_code, const char *description) noexcept
{
	std::cerr << "GLFW Error has occurred!\n\tError code: 0x" << std::hex << error_code
			  << "\n\tError description: " << description << '\n';
}

#endif

static void glfwFramebufferResizeCallback(GLFWwindow* window, int /* width */, int /* height */) noexcept
{
	Application * const app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
	app->setFramebufferResized();
}

Application::Application() noexcept
	: m_dynamicLoader(vkGetInstanceProcAddr), 
	m_allocationCallbacks(
		this, // User data
		reinterpret_cast<PFN_vkAllocationFunction>(vulkanAllocateMemory), // Allocation function
		reinterpret_cast<PFN_vkReallocationFunction>(vulkanReallocateMemory), // Reallocation function
		vulkanFreeMemory, // Free function
		nullptr, // Internal allocation function
		nullptr // Internal free function
	)
{
	// GLFW
	createWindow();

	// Vulkan
	initVulkan();
}

Application::~Application() noexcept
{
	// GLFW
	glfwTerminate();

	// Vulkan
	for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		m_logicalDevice.destroySemaphore(m_imageAvailableSemaphores[i], &m_allocationCallbacks);
		m_logicalDevice.destroySemaphore(m_renderFinishedSemaphores[i], &m_allocationCallbacks);
		m_logicalDevice.destroyFence(m_inFlightFences[i], &m_allocationCallbacks);
	}

	destroySwapChain();

#if ENABLE_PIPELINE_DYNAMIC_STATE
	m_logicalDevice.destroyPipeline(m_childGraphicsPipeline, &m_allocationCallbacks);
	m_logicalDevice.destroyPipeline(m_parentGraphicsPipeline, &m_allocationCallbacks);
	m_logicalDevice.destroyPipelineLayout(m_parentGraphicsPipelineInfo.layout, &m_allocationCallbacks);
	m_logicalDevice.destroyRenderPass(m_parentGraphicsPipelineInfo.renderPass, &m_allocationCallbacks);

	m_logicalDevice.destroyShaderModule(m_parentGraphicsPipelineInfo.vertexShaderModule, &m_allocationCallbacks);
	m_logicalDevice.destroyShaderModule(m_parentGraphicsPipelineInfo.fragmentShaderModule, &m_allocationCallbacks);
	// m_parentGraphicsPipelineInfo.created = false; // Not necessary
#endif

	writePipelineCache();
	m_logicalDevice.destroyPipelineCache(m_graphicsPipelineCache, &m_allocationCallbacks);
	m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout, &m_allocationCallbacks);

	m_logicalDevice.destroySampler(m_textureSampler, &m_allocationCallbacks);
	m_logicalDevice.destroyImageView(m_textureImageView, &m_allocationCallbacks);
	m_logicalDevice.destroyImage(m_textureImage, &m_allocationCallbacks);
	m_logicalDevice.freeMemory(m_textureImageMemory, &m_allocationCallbacks);

	m_logicalDevice.destroyBuffer(m_vertexBuffer, &m_allocationCallbacks);
	m_logicalDevice.freeMemory(m_vertexBufferMemory, &m_allocationCallbacks); // Free buffer memory after the buffer is destroyed
	m_logicalDevice.destroyBuffer(m_indexBuffer, &m_allocationCallbacks);
	m_logicalDevice.freeMemory(m_indexBufferMemory, &m_allocationCallbacks);

	m_logicalDevice.destroyCommandPool(m_graphicsCommandPool, &m_allocationCallbacks);
	m_logicalDevice.destroyCommandPool(m_transferCommandPool, &m_allocationCallbacks);
	m_logicalDevice.destroy(&m_allocationCallbacks);
	
	m_instance.destroySurfaceKHR(m_surface, &m_allocationCallbacks);
#ifdef _DEBUG
	m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, &m_allocationCallbacks, m_dynamicLoader);
#endif
	m_instance.destroy(&m_allocationCallbacks);

	// Memory cleanup
	delete[] m_swapChainImages;
	delete[] m_swapChainImageViews;
	delete[] m_swapChainFramebuffers;
	delete[] m_commandBuffers;
	delete[] m_imagesInFlight;
	delete[] m_uniformBuffers;
	delete[] m_uniformBuffersMemory;
	delete[] m_descriptorSets;

	// Logging
	DEBUG_LOG("Total vulkan debug callbacks: " << m_debugCallbackCount);
	DEBUG_LOG("Total vulkan allocations: " << m_debugAllocationCount);
	DEBUG_LOG("Total vulkan reallocations: " << m_debugReallocationCount);
	DEBUG_LOG("Total vulkan deallocations: " << m_debugDeallocationCount);
}

void Application::createWindow() noexcept
{
#ifdef _DEBUG
	glfwSetErrorCallback(glfwErrorCallback);
#endif

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Not using OpenGL
	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);
	DEBUG_LOG("Created the window; Width: " << WINDOW_WIDTH << ", Height: " << WINDOW_HEIGHT << ", Pointer:: " << m_window);

	glfwSetWindowUserPointer(m_window, this); // Bind the application with the glfw window pointer
	glfwSetFramebufferSizeCallback(m_window, glfwFramebufferResizeCallback);

	assert(glfwVulkanSupported() && "VULKAN ASSERT: Vulkan not supported on device!");
}

void Application::initVulkan() noexcept
{
	// Functions HAVE to be called in this order
	createInstance();
#ifdef _DEBUG
	createDebugCallback();
#endif
	createSurface(); 
	choosePhysicalDevice();
	createLogicalDevice(); 
	createSwapChain();
	createSwapChainImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createPipelineCache();
	createGraphicsPipeline();
	createCommandPools();
#if ENABLE_MULTISAMPLING
	createColorResources();
#endif
	createDepthResources();
	createFrameBuffers();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
	loadModel();
	createVertexBuffer();
	createIndexBuffer(); 
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();
}

void Application::createInstance() noexcept
{
	// Create result variable for result values at the start of every vulkan function (may be commented out if not used)
	vk::Result result;

	// Check validation layers
	assert(areLayersSupported<NUM_VALIDATION_LAYERS>(m_validationLayers) && "VULKAN ASSERT: Requested validation layers are not supported!");

	// Application info
	constexpr vk::ApplicationInfo app_info{
		APP_NAME, 
		APP_VERSION, 
		ENGINE_NAME, 
		ENGINE_VERSION, 
		VK_API_VERSION_1_1 // Highest version supported by MoltenVK
	};

	// Get required extensions
	const std::vector<const char *> extensions = getRequiredInstanceExtensions();

	// Create instance info
#ifdef NDEBUG
	const
#endif
	vk::InstanceCreateInfo instance_create_info{
		{}, // Flags
		&app_info, // Application info
#ifdef _DEBUG
		NUM_VALIDATION_LAYERS, // Enabled layer count: number of wanted validation layers
		m_validationLayers, // Enabled layer names: wanted validation layers
#else
		0, nullptr,
#endif
		static_cast<uint32_t>(extensions.size()), // Enabled extension count: number of wanted extensions
		extensions.data() // Enabled extension names: wanted extensions
	};

#ifdef _DEBUG
	// Add debugging info
	const vk::DebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{
		{}, 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | 
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | 
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(vulkanDebugCallback),
		this
	};
	instance_create_info.pNext = static_cast<const void *>(&debug_messenger_create_info);
#endif

	// Create instance
	result = vk::createInstance(&instance_create_info, &m_allocationCallbacks, &m_instance);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Instance; Pointer:: " << m_instance);

	// Add more functions to dynamic loader
	m_dynamicLoader.init(m_instance);
}

#ifdef _DEBUG

void Application::createDebugCallback() 
{
	vk::Result result;

	const vk::DebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{
		{}, // Flags
		// vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | // Diagnostic messages
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | // Informational messages like the creation of a resource
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | // Messages about behavior that is not necessarily an error, but very likely a bug in your application
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError, // Messages about behavior that is invalid and may cause crashes
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | // Some event has happened that is unrelated to the specification or performance
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | // Something has happened that violates the specification or indicates a possible mistake
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance, // Potential non-optimal use of Vulkan
		reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(vulkanDebugCallback), // Pointer: to debug callback function
		this // Pointer: to data for callback function to fill (not being used)
	};

	// Create debug messenger
	result = m_instance.createDebugUtilsMessengerEXT(&debug_messenger_create_info, &m_allocationCallbacks, &m_debugMessenger, m_dynamicLoader); // Extension functions need to be dynamically loaded
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Debug Utils Messenger; Pointer:: " << m_debugMessenger);
}

#endif

void Application::createSurface() noexcept
{
	vk::Result result;
	VkSurfaceKHR temp_surface;

	result = static_cast<vk::Result>(glfwCreateWindowSurface(m_instance, m_window, reinterpret_cast<const VkAllocationCallbacks *>(&m_allocationCallbacks), &temp_surface));
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Window Surface; Pointer:: " << temp_surface);

	m_surface = temp_surface;
}

void Application::choosePhysicalDevice() noexcept
{
	vk::Result result;
	vk::PhysicalDevice *physical_devices;
	uint32_t physical_device_count;

	result = m_instance.enumeratePhysicalDevices(&physical_device_count, nullptr);
	assert(result == vk::Result::eSuccess);

	physical_devices = new vk::PhysicalDevice[physical_device_count];
	result = m_instance.enumeratePhysicalDevices(&physical_device_count, physical_devices);
	assert(result == vk::Result::eSuccess);

	assert(physical_device_count && "VULKAN ASSERT: Failed to find GPU/s with Vulkan support!");
	for (uint32_t i = 0; i < physical_device_count; ++i) {
		if (physicalDeviceSupportsRequirements(physical_devices[i])) {
			m_physicalDevice = physical_devices[i];
			DEBUG_LOG("Created Vulkan Physical Device; Pointer:: " << m_physicalDevice);

			m_physicalDeviceProperties.pNext = &m_physicalDeviceDriverProperties;
			m_physicalDeviceDriverProperties.pNext = &m_physicalDevicePortabilitySubsetProperties;

			m_physicalDeviceFeatures.pNext = &m_physicalDevicePortabilitySubsetFeatures;

			m_physicalDevice.getFeatures2(&m_physicalDeviceFeatures);
			m_physicalDevice.getProperties2(&m_physicalDeviceProperties);
			m_physicalDevice.getMemoryProperties2(&m_physicalDeviceMemoryProperties);

#if ENABLE_MULTISAMPLING
			m_msaaSamples = getMaxUsableSampleCount();
#endif

#ifdef _DEBUG
			const auto printMemoryTypes = [&]{
				for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryProperties.memoryTypeCount; ++i) {
					std::cout << "\tMemory Type " << i << " Property Flags:\t\t\t\t" << vk::to_string(m_physicalDeviceMemoryProperties.memoryProperties.memoryTypes[i].propertyFlags) << '\n'
							  << "\tMemory Type " << i << " Heap Index:\t\t\t\t" << m_physicalDeviceMemoryProperties.memoryProperties.memoryTypes[i].heapIndex << '\n';
				}

				return ' ';
			};

			const auto printMemoryHeaps = [&]{
				for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryProperties.memoryHeapCount; ++i) {
					std::cout << "\tMemory Heap " << i << " Size:\t\t\t\t\t" << m_physicalDeviceMemoryProperties.memoryProperties.memoryHeaps[i].size << '\n'
							  << "\tMemory Heap " << i << " Flags:\t\t\t\t\t" << vk::to_string(m_physicalDeviceMemoryProperties.memoryProperties.memoryHeaps[i].flags) << '\n';
				}

				return ' ';
			};
#endif

			DEBUG_LOG(
				std::boolalpha <<
				"API version:\t\t\t\t\t\t" << VK_API_VERSION_VARIANT(m_physicalDeviceProperties.properties.apiVersion) << '.'
											   << VK_API_VERSION_MAJOR(m_physicalDeviceProperties.properties.apiVersion) << '.'
									 	 	   << VK_API_VERSION_MINOR(m_physicalDeviceProperties.properties.apiVersion) << '.'
									 	 	   << VK_API_VERSION_PATCH(m_physicalDeviceProperties.properties.apiVersion) << '\n' << 

				"\tDriver Version:\t\t\t\t\t\t" << VK_API_VERSION_VARIANT(m_physicalDeviceProperties.properties.driverVersion) << '.'
												  << VK_API_VERSION_MAJOR(m_physicalDeviceProperties.properties.driverVersion) << '.'
								   				  << VK_API_VERSION_MINOR(m_physicalDeviceProperties.properties.driverVersion) << '.' // Not always the right way to get the driver version (it's vendor-specific)
								   				  << VK_API_VERSION_PATCH(m_physicalDeviceProperties.properties.driverVersion) << '\n' <<

				"\tDriver Conformance Version:\t\t\t\t" << +m_physicalDeviceDriverProperties.conformanceVersion.major << '.' 
														  << +m_physicalDeviceDriverProperties.conformanceVersion.minor << '.' 
														  << +m_physicalDeviceDriverProperties.conformanceVersion.subminor << '.' 
														  << +m_physicalDeviceDriverProperties.conformanceVersion.patch << '\n' <<

				"\tDriver Info:\t\t\t\t\t\t" << m_physicalDeviceDriverProperties.driverInfo.data() << '\n' << 
				"\tDriver ID:\t\t\t\t\t\t" << vk::to_string(m_physicalDeviceDriverProperties.driverID) << '\n' << 
				"\tDriver Name:\t\t\t\t\t\t" << m_physicalDeviceDriverProperties.driverName.data() << '\n' << 

				"\tVendor ID:\t\t\t\t\t\t0x" << std::hex << m_physicalDeviceProperties.properties.vendorID << '\n' << 
				"\tDevice ID:\t\t\t\t\t\t0x" << m_physicalDeviceProperties.properties.deviceID << std::dec << '\n' << 
				"\tDevice Type:\t\t\t\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.deviceType) << '\n' <<
				"\tDevice Name:\t\t\t\t\t\t" << m_physicalDeviceProperties.properties.deviceName.data() << '\n' <<
				"\tPipeline Cache UUID:\t\t\t\t\t0x" << std::hex << +m_physicalDeviceProperties.properties.pipelineCacheUUID[0]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[1]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[2]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[3]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[4]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[5]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[6]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[7]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[8]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[9]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[10]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[11]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[12]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[13]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[14]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[15]
														   	 	   << +m_physicalDeviceProperties.properties.pipelineCacheUUID[16] << std::dec << '\n' <<
				
				"\tMax Image Dimension 1D:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimension1D << '\n' <<
				"\tMax Image Dimension 2D:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimension2D << '\n' <<
				"\tMax Image Dimension 3D:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimension3D << '\n' <<
				"\tMax Image Dimension Cube:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimensionCube << '\n' <<
				"\tMax Image Array layers:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageArrayLayers << '\n' <<
				"\tMax Texel Buffer Elements:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTexelBufferElements << '\n' <<
				"\tMax Uniform Buffer Range:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxUniformBufferRange << '\n' <<
				"\tMax Storage Buffer Range:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxStorageBufferRange << '\n' <<
				"\tMax Push Constants Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxPushConstantsSize << '\n' <<
				"\tMax Memory Allocation Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxMemoryAllocationCount << '\n' <<
				"\tMax Sampler Allocation Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSamplerAllocationCount << '\n' <<
				"\tBuffer Image Granularity:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.bufferImageGranularity << '\n' <<
				"\tSparse Address Space Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.sparseAddressSpaceSize << '\n' <<
				"\tMax Bound Descriptor Sets:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxBoundDescriptorSets << '\n' <<
				"\tMax Per Stage Descriptor Samplers:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorSamplers << '\n' <<
				"\tMax Per Stage Descriptor Uniform Buffers:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorUniformBuffers << '\n' <<
				"\tMax Per Stage Descriptor Storage Buffers:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorStorageBuffers << '\n' << 
				"\tMax Per Stage Descriptor Sampled Images:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorSampledImages << '\n' <<
				"\tMax Per Stage Descriptor Storage Images:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorStorageImages << '\n' <<
				"\tMax Per Stage Descriptor Input Attachments:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorInputAttachments << '\n' <<
				"\tMax Per Stage Resources:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageResources << '\n' <<
				"\tMax Descriptor Set Samplers:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetSamplers << '\n' <<
				"\tMax Descriptor Set Uniform Buffers:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetUniformBuffers << '\n' <<
				"\tMax Descriptor Set Uniform Buffers Dynamic:\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetUniformBuffersDynamic << '\n' <<
				"\tMax Descriptor Set Storage Buffers:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetStorageBuffers << '\n' <<
				"\tMax Descriptor Set Storage Buffers Dynamic:\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetStorageBuffersDynamic << '\n' <<
				"\tMax Descriptor Set Sampled Images:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetSampledImages << '\n' <<
				"\tMax Descriptor Set Storage Images:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetStorageImages << '\n' <<
				"\tMax Descriptor Set Input Attachments:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetInputAttachments << '\n' <<
				"\tMax Vertex Input Attributes:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputAttributes << '\n' <<
				"\tMax Vertex Input Bindings:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputBindings << '\n' <<
				"\tMax Vertex Input Attribute Offset:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputAttributeOffset << '\n' <<
				"\tMax Vertex Input Binding Stride:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputBindingStride << '\n' <<
				"\tMax Vertex Output Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexOutputComponents << '\n' <<
				"\tMax Tessellation Generation Level:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationGenerationLevel << '\n' <<
				"\tMax Tessellation Patch Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationPatchSize << '\n' <<
				"\tMax Tessellation Control Per Vertex Input Components:\t" << m_physicalDeviceProperties.properties.limits.maxTessellationControlPerVertexInputComponents << '\n' <<
				"\tMax Tessellation Control Per Vertex Output Components:\t" << m_physicalDeviceProperties.properties.limits.maxTessellationControlPerVertexOutputComponents << '\n' <<
				"\tMax Tessellation Control Per Patch Output Components:\t" << m_physicalDeviceProperties.properties.limits.maxTessellationControlPerPatchOutputComponents << '\n' <<
				"\tMax Tessellation Evaluation Input Components:\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationEvaluationInputComponents << '\n' <<
				"\tMax Tessellation Evaluation Output Components:\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationEvaluationOutputComponents << '\n' <<
				"\tMax Geometry Shader Invocations:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryShaderInvocations << '\n' <<
				"\tMax Geometry Input Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryInputComponents << '\n' <<
				"\tMax Geometry Output Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryOutputComponents << '\n' <<
				"\tMax Geometry Output Vertices:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryOutputVertices << '\n' <<
				"\tMax Geometry Total Output Components:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryTotalOutputComponents << '\n' <<
				"\tMax Fragment Input Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentInputComponents << '\n' <<
				"\tMax Fragment Output Attachments:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentOutputAttachments << '\n' <<
				"\tMax Fragment Dual Src Attachments:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentDualSrcAttachments << '\n' <<
				"\tMax Fragment Combined Output Resources:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentCombinedOutputResources << '\n' <<
				"\tMax Compute Shared Memory Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeSharedMemorySize << '\n' <<
				"\tMax Compute Work Group Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupCount[0] << ", " 
															<< m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupCount[1] << ", " 
															<< m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupCount[2] << '\n' <<

				"\tMax Compute Work Group Invocations:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupInvocations << '\n' <<
				"\tMax Compute Work Group Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupSize[0] << ", " 
														   << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupSize[1] << ", " 
														   << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupSize[2] << '\n' <<

				"\tSub Pixel Precision Bits:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.subPixelPrecisionBits << '\n' <<
				"\tSub Texel Precision Bits:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.subTexelPrecisionBits << '\n' <<
				"\tMipmap Precision Bits:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.mipmapPrecisionBits << '\n' <<
				"\tMax Draw Indexed Index Value:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDrawIndexedIndexValue << '\n' <<
				"\tMax Draw Indirect Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDrawIndirectCount << '\n' <<
				"\tMax Sampler LOD Bias:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSamplerLodBias << '\n' <<
				"\tMax Sampler Anisotropy:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSamplerAnisotropy << '\n' <<
				"\tMax Viewports:\t\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxViewports << '\n' <<
				"\tMax Viewport Dimensions:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxViewportDimensions[0] << ", " 
													   << m_physicalDeviceProperties.properties.limits.maxViewportDimensions[1] << '\n' <<
				
				"\tViewport Bounds Range:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.viewportBoundsRange[0] << ", " 
													   << m_physicalDeviceProperties.properties.limits.viewportBoundsRange[1] << '\n' <<
				
				"\tViewport Sub Pixel Bits:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.viewportSubPixelBits << '\n' <<
				"\tMin Memory Map Alignment:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minMemoryMapAlignment << '\n' <<
				"\tMin Texel Buffer Offset Alignment:\t\t\t" << m_physicalDeviceProperties.properties.limits.minTexelBufferOffsetAlignment << '\n' <<
				"\tMin Uniform Buffer Offset Alignment:\t\t\t" << m_physicalDeviceProperties.properties.limits.minUniformBufferOffsetAlignment << '\n' <<
				"\tMin Storage Buffer Offset Alignment:\t\t\t" << m_physicalDeviceProperties.properties.limits.minStorageBufferOffsetAlignment << '\n' <<
				"\tMin Texel Offset:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minTexelOffset << '\n' <<
				"\tMax Texel Offset:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTexelOffset << '\n' <<
				"\tMin Texel Gather Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minTexelGatherOffset << '\n' <<
				"\tMax Texel Gather Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTexelGatherOffset << '\n' <<
				"\tMin Interpolation Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minInterpolationOffset << '\n' <<
				"\tMax Interpolation Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxInterpolationOffset << '\n' <<
				"\tSub Pixel Interpolation Offset Bits:\t\t\t" << m_physicalDeviceProperties.properties.limits.subPixelInterpolationOffsetBits << '\n' <<
				"\tMax Framebuffer Width:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFramebufferWidth << '\n' <<
				"\tMax Framebuffer Height:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFramebufferHeight << '\n' <<
				"\tMax Framebuffer Layers:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFramebufferLayers << '\n' <<
				"\tFramebuffer Color Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferColorSampleCounts) << '\n' <<
				"\tFramebuffer Depth Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferDepthSampleCounts) << '\n' <<
				"\tFramebuffer Stencil Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferStencilSampleCounts) << '\n' <<
				"\tFramebuffer No Attachments Sample Counts:\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferNoAttachmentsSampleCounts) << '\n' <<
				"\tMax Color Attachments:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxColorAttachments << '\n' <<
				"\tSampled Image Color Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageColorSampleCounts) << '\n' <<
				"\tSampled Image Integer Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageIntegerSampleCounts) << '\n' <<
				"\tSampled Image Depth Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageDepthSampleCounts) << '\n' <<
				"\tSampled Image Stencil Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageStencilSampleCounts) << '\n' <<
				"\tStorage Image Sample Counts:\t\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.storageImageSampleCounts) << '\n' <<
				"\tMax Sample Mask Words:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSampleMaskWords << '\n' <<
				"\tTimestamp Compute And Graphics:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.limits.timestampComputeAndGraphics) << '\n' <<
				"\tTimestamp Period:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.timestampPeriod << '\n' <<
				"\tMax Clip Distances:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxClipDistances << '\n' <<
				"\tMax Cull Distances:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxCullDistances << '\n' <<
				"\tMax Combined Clip And Cull Distances:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxCombinedClipAndCullDistances << '\n' <<
				"\tDiscrete Queue Priorities:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.discreteQueuePriorities << '\n' <<
				"\tPoint Size Range:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.pointSizeRange[0] << ", "
												  << m_physicalDeviceProperties.properties.limits.pointSizeRange[1] << '\n' <<
				
				"\tLine Width Range:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.lineWidthRange[0] << ", "
												  << m_physicalDeviceProperties.properties.limits.lineWidthRange[1] << '\n' <<
				
				"\tPoint Size Granularity:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.pointSizeGranularity << '\n' <<
				"\tLine Width Granularity:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.lineWidthGranularity << '\n' <<
				"\tMin Vertex Input Binding Stride Alignment:\t\t" << m_physicalDevicePortabilitySubsetProperties.minVertexInputBindingStrideAlignment << '\n' <<
				"\tStrict Lines:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.limits.strictLines) << '\n' <<
				"\tStandard Sample Locations:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.limits.standardSampleLocations) << '\n' <<
				"\tOptimal Buffer Copy Offset Alignment:\t\t\t" << m_physicalDeviceProperties.properties.limits.optimalBufferCopyOffsetAlignment << '\n' <<
				"\tOptimal Buffer Copy Row Pitch Alignment:\t\t" << m_physicalDeviceProperties.properties.limits.optimalBufferCopyRowPitchAlignment << '\n' <<
				"\tNon Coherent Atom Size:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.nonCoherentAtomSize << '\n' <<
				"\tResidency Standard 2D Block Shape:\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyStandard2DBlockShape) << '\n' <<
				"\tResidency Standard 2D Multisample Block Shape:\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyStandard2DMultisampleBlockShape) << '\n' <<
				"\tResidency Standard 3D Block Shape:\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyStandard3DBlockShape) << '\n' <<
				"\tResidency Aligned Mip Size:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyAlignedMipSize) << '\n' <<
				"\tResidency Non Resident Strict:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyNonResidentStrict) << '\n' <<
				printMemoryTypes() << 
				printMemoryHeaps() <<
				"\tRobust Buffer Access:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.robustBufferAccess) << '\n' <<
				"\tFull Draw Index Uint32:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.fullDrawIndexUint32) << '\n' <<
				"\tImage Cube Array:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.imageCubeArray) << '\n' <<
				"\tIndependent Blend:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.independentBlend) << '\n' <<
				"\tGeometry Shader:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.geometryShader) << '\n' <<
				"\tTessellation Shader:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.tessellationShader) << '\n' <<
				"\tSample Rate Shading:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sampleRateShading) << '\n' <<
				"\tDual Src Blend:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.dualSrcBlend) << '\n' <<
				"\tLogic Op:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.logicOp) << '\n' <<
				"\tMulti Draw Indirect:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.multiDrawIndirect) << '\n' <<
				"\tDraw Indirect First Instance:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.drawIndirectFirstInstance) << '\n' <<
				"\tDepth Clamp:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.depthClamp) << '\n' <<
				"\tDepth Bias Clamp:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.depthBiasClamp) << '\n' <<
				"\tFill Mode Non Solid:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.fillModeNonSolid) << '\n' <<
				"\tDepth Bounds:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.depthBounds) << '\n' <<
				"\tWide Lines:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.wideLines) << '\n' <<
				"\tLarge Points:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.largePoints) << '\n' <<
				"\tAlpha To One:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.alphaToOne) << '\n' <<
				"\tMulti Viewport:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.multiViewport) << '\n' <<
				"\tSampler Anisotropy:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.samplerAnisotropy) << '\n' <<
				"\tTexture Compression ETC 2:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.textureCompressionETC2) << '\n' <<
				"\tTexture Compression ASTC LDR:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.textureCompressionASTC_LDR) << '\n' <<
				"\tTexture Compression BC:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.textureCompressionBC) << '\n' <<
				"\tOcclusion Query Precise:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.occlusionQueryPrecise) << '\n' <<
				"\tPipeline Statistics Query:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.pipelineStatisticsQuery) << '\n' <<
				"\tVertex Pipeline Stores And Atomics:\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.vertexPipelineStoresAndAtomics) << '\n' <<
				"\tFragment Stores And Atomics:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.fragmentStoresAndAtomics) << '\n' <<
				"\tShader Tessellation And Geometry Point Size:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderTessellationAndGeometryPointSize) << '\n' <<
				"\tShader Image Gather Extended:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderImageGatherExtended) << '\n' <<
				"\tShader Storage Image Extended Formats:\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageExtendedFormats) << '\n' <<
				"\tShader Storage Image Multisample:\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageMultisample) << '\n' <<
				"\tShader Storage Image Read Without Format:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageReadWithoutFormat) << '\n' <<
				"\tShader Storage Image Write Without Format:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageWriteWithoutFormat) << '\n' <<
				"\tShader Uniform Buffer Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderUniformBufferArrayDynamicIndexing) << '\n' <<
				"\tShader Sampled Image Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderSampledImageArrayDynamicIndexing) << '\n' <<
				"\tShader Storage Buffer Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageBufferArrayDynamicIndexing) << '\n' <<
				"\tShader Storage Image Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageArrayDynamicIndexing) << '\n' <<
				"\tShader Clip Distance:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderClipDistance) << '\n' <<
				"\tShader Cull Distance:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderCullDistance) << '\n' <<
				"\tShader Float64:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderFloat64) << '\n' <<
				"\tShader Int64:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderInt64) << '\n' <<
				"\tShader Int16:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderInt16) << '\n' <<
				"\tShader Resource Residency:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderResourceResidency) << '\n' <<
				"\tShader Resource Min LOD:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderResourceMinLod) << '\n' <<
				"\tSparse Binding:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseBinding) << '\n' <<
				"\tSparse Residency Buffer:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyBuffer) << '\n' <<
				"\tSparse Residency Image 2D:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyImage2D) << '\n' <<
				"\tSparse Residency Image 3D:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyImage3D) << '\n' <<
				"\tSparse Residency 2 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency2Samples) << '\n' <<
				"\tSparse Residency 4 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency4Samples) << '\n' <<
				"\tSparse Residency 8 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency8Samples) << '\n' <<
				"\tSparse Residency 16 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency16Samples) << '\n' <<
				"\tSparse Residency Aliased:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyAliased) << '\n' <<
				"\tVariable Multisample Rate:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.variableMultisampleRate) << '\n' <<
				"\tInherited Queries:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.inheritedQueries) << '\n' <<
				"\tConstant Alpha Color Blend Factors:\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.constantAlphaColorBlendFactors) << '\n' <<
				"\tEvents:\t\t\t\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.events) << '\n' <<
				"\tImage View Format Reinterpretation:\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.imageViewFormatReinterpretation) << '\n' <<
				"\tImage View Format Swizzle:\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.imageViewFormatSwizzle) << '\n' <<
				"\tImage View 2D On 3D Image:\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.imageView2DOn3DImage) << '\n' <<
				"\tMultisample Array Image:\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.multisampleArrayImage) << '\n' <<
				"\tMutable Comparison Samplers:\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.mutableComparisonSamplers) << '\n' <<
				"\tPoint Polygons:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.pointPolygons) << '\n' <<
				"\tSampler Mip LOD Bias:\t\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.samplerMipLodBias) << '\n' <<
				"\tSeparate Stencil Mask Ref:\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.separateStencilMaskRef) << '\n' <<
				"\tShader Sample Rate Interpolation Functions:\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.shaderSampleRateInterpolationFunctions) << '\n' <<
				"\tTessellation Isolines:\t\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.tessellationIsolines) << '\n' <<
				"\tTessellation Point Mode:\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.tessellationPointMode) << '\n' <<
				"\tTriangle Fans:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.triangleFans) << '\n' <<
				"\tVertex Attribute Access Beyond Stride:\t\t\t" << static_cast<bool>(m_physicalDevicePortabilitySubsetFeatures.vertexAttributeAccessBeyondStride)
			);

			delete[] physical_devices;
			return;
		}
	}

	delete[] physical_devices;
	assert(false && "VULKAN ASSERT: Failed to find GPU with requested requirements!");
}

void Application::createLogicalDevice() noexcept
{
	vk::Result result;

	m_queueFamilyIndices = getQueueFamilies(m_physicalDevice);
	const std::set<uint32_t> queue_family_indices = {
		m_queueFamilyIndices.graphicsFamily.value(),
		m_queueFamilyIndices.presentFamily.value(),
		m_queueFamilyIndices.transferFamily.value(),
		m_queueFamilyIndices.computeFamily.value()
	};

#ifdef _DEBUG
	uint32_t queue_family_count;
	vk::QueueFamilyProperties2 *queue_family_properties; 

	m_physicalDevice.getQueueFamilyProperties2(&queue_family_count, nullptr);
	queue_family_properties = new vk::QueueFamilyProperties2[queue_family_count];
	m_physicalDevice.getQueueFamilyProperties2(&queue_family_count, queue_family_properties);

	const auto printQueueFamilyProperties = [&]{
		for (uint32_t i = 0; i < queue_family_count; ++i) {
			std::cout << "\t\tQueue Family " << i << " queue count:\t" << queue_family_properties[i].queueFamilyProperties.queueCount << '\n'
					  << "\t\tQueue Family " << i << " Flags:\t\t" << vk::to_string(queue_family_properties[i].queueFamilyProperties.queueFlags) << '\n';
		}
		
		return ' ';
	};

	DEBUG_LOG(
		"Total number of Queue Families: " << queue_family_count << '\n' <<
		printQueueFamilyProperties() << '\n' <<
		"\tNumber of Queue Families being used: " << queue_family_indices.size()
	);

	if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.transferFamily.value())
		DEBUG_LOG("Graphics family found, transfer family missing: using graphics family");
	
	if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.computeFamily.value())
		DEBUG_LOG("Graphics family found, compute family missing: using graphics family");

	delete[] queue_family_properties;
#endif

	vk::DeviceQueueCreateInfo * const device_queue_create_infos = new vk::DeviceQueueCreateInfo[queue_family_indices.size()];

	// Required device features
	constexpr vk::PhysicalDeviceFeatures2 features{
		{
			VK_FALSE, // Robust buffer access
			VK_FALSE, // Full draw index uint32
			VK_FALSE, // Image cube array
			VK_FALSE, // Independent blend
			VK_FALSE, // Geometry shader
			VK_FALSE, // Tessellation shader
#if ENABLE_MULTISAMPLING
			VK_TRUE, // Sample rate shading
#else
			VK_FALSE,
#endif
			VK_FALSE, // Dual src blend
			VK_FALSE, // Logic op
			VK_FALSE, // Multi draw indirect
			VK_FALSE, // Draw indirect first instance
			VK_FALSE, // Depth clamp
			VK_FALSE, // Depth bias clamp
			VK_FALSE, // Fill mode non solid
			VK_FALSE, // Depth bounds
			VK_FALSE, // Wide lines
			VK_FALSE, // Large points
			VK_FALSE, // Alpha to one
			VK_FALSE, // Multi viewport
			VK_TRUE, // Sampler anisotropy
			VK_FALSE, // Texture compression ETC2
			VK_FALSE, // Texture compression ASTC_LDR
			VK_FALSE, // Texture compression BC
			VK_FALSE, // Occlusion query precise
			VK_FALSE, // Pipeline statistics query
			VK_FALSE, // Vertex pipeline stores and atomics
			VK_FALSE, // Fragment stores and atomics
			VK_FALSE, // Shader tessellation and geometry point size
			VK_FALSE, // Shader image gather extended
			VK_FALSE, // Shader storage image extended formats
			VK_FALSE, // Shader storage image multisample
			VK_FALSE, // Shader storage image read without format
			VK_FALSE, // Shader storage image write without format
			VK_FALSE, // Shader uniform buffer array dynamic indexing
			VK_FALSE, // Shader sampled image array dynamic indexing
			VK_FALSE, // Shader storage buffer array dynamic indexing
			VK_FALSE, // Shader storage image array dynamic indexing
			VK_FALSE, // Shader clip distance
			VK_FALSE, // Shader cull distance
			VK_FALSE, // Shader float 64
			VK_FALSE, // Shader int 64
			VK_FALSE, // Shader int 16
			VK_FALSE, // Shader resource residency
			VK_FALSE, // Shader resource min lod
			VK_FALSE, // Sparse binding
			VK_FALSE, // Sparse residency buffer
			VK_FALSE, // Sparse residency image 2D
			VK_FALSE, // Sparse residency image 3D
			VK_FALSE, // Sparse residency 2 samples
			VK_FALSE, // Sparse residency 4 samples
			VK_FALSE, // Sparse residency 8 samples
			VK_FALSE, // Sparse residency 16 samples
			VK_FALSE, // Sparse residency aliased
			VK_FALSE, // Variable multisample rate
			VK_FALSE // Inherited queries
		} // Temporary original vk::PhysicalDeviceFeatures
	};

	uint8_t i = 0;
	constexpr float queue_priority = 1.0f;
	for (const auto queue_family_index : queue_family_indices) {
		device_queue_create_infos[i] = {
			{}, // Flags
			queue_family_index, // Queue family index
			static_cast<uint32_t>(queue_family_indices.size()), // Queue count
			&queue_priority // Queue priorities
		};

		++i;
	}

	const vk::DeviceCreateInfo device_create_info{
		{}, // Flags
		static_cast<uint32_t>(queue_family_indices.size()), // Queue create info count: number of queue create infos
		device_queue_create_infos, // Queue create infos: array of queue create infos
#ifdef _DEBUG
		NUM_VALIDATION_LAYERS, // Enabled layer count: number of validation layers
		m_validationLayers, // Enabled layer names: required validation layers (when in debug)
#else
		0, nullptr, // No validation layers required (when in release)
#endif
		NUM_DEVICE_EXTENSIONS, // Enabled extension count: number of device extensions
		m_deviceExtensions, // Enabled extension names: required device extensions
		&features.features // Enabled features: features of physical device
	};

	// Create the logical device
	result = m_physicalDevice.createDevice(&device_create_info, &m_allocationCallbacks, &m_logicalDevice);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan [logical] Device; Pointer: " << m_logicalDevice);

	// Add more functions to dynamic loader
	m_dynamicLoader.init(m_logicalDevice);

	// Free memory
	delete[] device_queue_create_infos;

	// Get handles to the queue families
	vk::DeviceQueueInfo2 queue_info{
		{}, // Flags
		m_queueFamilyIndices.graphicsFamily.value(), // Queue family index
		0 // Queue index
	};

	m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_graphicsQueue);

	// Create the right amount of queue handles
	switch (queue_family_indices.size())
	{
	case 1:
		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);
		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);
		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);
		return;

	case 2:
		[[likely]] if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.presentFamily.value()) {
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);

			queue_info.queueFamilyIndex = m_queueFamilyIndices.transferFamily.value();
			queue_info.queueIndex = 1;
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);
			return;
		}

		if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.transferFamily.value()) {
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);

			queue_info.queueFamilyIndex = m_queueFamilyIndices.presentFamily.value();
			queue_info.queueIndex = 1;
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);
			return;
		}

		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);

		queue_info.queueFamilyIndex = m_queueFamilyIndices.presentFamily.value();
		queue_info.queueIndex = 1;
		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);
		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);
		return;

	case 3:
		[[likely]] if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.presentFamily.value()) {
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);

			queue_info.queueFamilyIndex = m_queueFamilyIndices.transferFamily.value();
			queue_info.queueIndex = 1;
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);

			queue_info.queueFamilyIndex = m_queueFamilyIndices.computeFamily.value();
			queue_info.queueIndex = 2;
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);
			return;
		}

		if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.transferFamily.value()) {
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);

			queue_info.queueFamilyIndex = m_queueFamilyIndices.presentFamily.value();
			queue_info.queueIndex = 1;
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);

			queue_info.queueFamilyIndex = m_queueFamilyIndices.computeFamily.value();
			queue_info.queueIndex = 2;
			m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);
			return;
		}

		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);

		queue_info.queueFamilyIndex = m_queueFamilyIndices.presentFamily.value();
		queue_info.queueIndex = 1;
		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);

		queue_info.queueFamilyIndex = m_queueFamilyIndices.transferFamily.value();
		queue_info.queueIndex = 2;
		m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);
		return;
	}

	queue_info.queueFamilyIndex = m_queueFamilyIndices.presentFamily.value();
	queue_info.queueIndex = 1;
	m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_presentQueue);

	queue_info.queueFamilyIndex = m_queueFamilyIndices.transferFamily.value();
	queue_info.queueIndex = 2;
	m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_transferQueue);

	queue_info.queueFamilyIndex = m_queueFamilyIndices.computeFamily.value();
	queue_info.queueIndex = 3;
	m_logicalDevice.getQueue2(&queue_info, &m_queueHandles.m_computeQueue);
}

void Application::createSwapChain() noexcept
{
	vk::Result result;

	const SwapChainSupportDetails swap_chain_support = querySwapChainSupport(m_physicalDevice); //
	const vk::SurfaceFormat2KHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats, swap_chain_support.formatCount);
	const vk::PresentModeKHR present_mode = chooseSwapPresentMode(swap_chain_support.presentModes, swap_chain_support.presentModeCount);
	m_swapChainExtent = chooseSwapExtent(swap_chain_support.capabilities);

	uint32_t image_count = swap_chain_support.capabilities.surfaceCapabilities.minImageCount + 1; // Amount of images to be stored in the queue (recommended to request at least 1 more than the minimum)
	m_swapChainImageFormat = surface_format.surfaceFormat.format;

	if (swap_chain_support.capabilities.surfaceCapabilities.maxImageCount > 0 && // If maxImageCount == 0, then there is no maximum
		image_count > swap_chain_support.capabilities.surfaceCapabilities.maxImageCount) // Wanted queue size is larger than the maximum
		image_count = swap_chain_support.capabilities.surfaceCapabilities.maxImageCount;

	const uint32_t queue_family_indices[SWAP_CHAIN_QUEUE_FAMILY_INDEX_COUNT] = {
		m_queueFamilyIndices.graphicsFamily.value(),
		m_queueFamilyIndices.presentFamily.value()
	};

	vk::SwapchainCreateInfoKHR swapchain_create_info{
		{}, // Flags
		m_surface, // Surface
		image_count, // Min image count
		surface_format.surfaceFormat.format, // Image format
		surface_format.surfaceFormat.colorSpace, // Image color space
		m_swapChainExtent, // Image extent
		1, // Image array layers: The amount of layers each image consists of
		vk::ImageUsageFlagBits::eColorAttachment, // The kind of operations we'll use the images in the swap chain for (color attachment == render directly to them)
		vk::SharingMode::eExclusive, // Image sharing mode: whether images are owned by one queue family at a time, or multiple (one; offers the best performance)
		{}, // Queue family index count
		queue_family_indices, // Queue family indices
		swap_chain_support.capabilities.surfaceCapabilities.currentTransform, // Pre transform (current transform means add no transform)
		vk::CompositeAlphaFlagBitsKHR::eOpaque, // For blending with other windows in the window system (Opaque means to ignore the alpha channel)
		present_mode, // Present mode
		VK_TRUE, // Clipped: true if you don't care about the color of pixels that are obscured, for example because another window is in front of them
		{} // Old swap chain: reference to old swap chain if current is invalid or unoptimized (e.g. because window is resized). No given reference means we'll only create one swap chain
	};
	
	// Queue families
	if (m_queueFamilyIndices.graphicsFamily != m_queueFamilyIndices.presentFamily) { // Use concurrent mode if queue families differ to avoid manual ownership transfer
		swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent; // Images can be used across multiple queue families without explicit ownership transfers
		swapchain_create_info.queueFamilyIndexCount = SWAP_CHAIN_QUEUE_FAMILY_INDEX_COUNT;
	}

	// Create the swapchain
	result = m_logicalDevice.createSwapchainKHR(&swapchain_create_info, &m_allocationCallbacks, &m_swapChain);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Swapchain; Pointer: " << m_swapChain);

	result = m_logicalDevice.getSwapchainImagesKHR(m_swapChain, &m_swapChainImageCount, nullptr);
	assert(result == vk::Result::eSuccess);

	[[unlikely]] if (m_swapChainImages == nullptr)
		m_swapChainImages = new vk::Image[m_swapChainImageCount];

	result = m_logicalDevice.getSwapchainImagesKHR(m_swapChain, &m_swapChainImageCount, m_swapChainImages);
	assert(result == vk::Result::eSuccess);
}

void Application::createSwapChainImageViews() noexcept
{
	[[unlikely]] if (m_swapChainImageViews == nullptr)
		m_swapChainImageViews = new vk::ImageView[m_swapChainImageCount];

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		m_swapChainImageViews[i] = createImageView(
			m_swapChainImages[i], // Image
			m_swapChainImageFormat, // Format
			vk::ImageAspectFlagBits::eColor, // Aspect flags
			1 // Mip levels
		);
	}
}

void Application::createRenderPass() noexcept
{
	vk::Result result;

	// Color buffer attachment represented by one of the images from the swap chain
	const vk::AttachmentDescription2KHR color_attachment{ // KHR extension being used because it wasn't core until 1.2 (using 1.1)
		{}, // Flags
		m_swapChainImageFormat, // Format (matches the swap chain image format)
#if ENABLE_MULTISAMPLING
		m_msaaSamples, // Samples
#else
		vk::SampleCountFlagBits::e1,
#endif
		vk::AttachmentLoadOp::eClear, // Load op: what to do with the color and depth data in the attachment before rendering (clear the values to a constant at the start)
		vk::AttachmentStoreOp::eDontCare, // Store op: what to do with the color and depth data in the attachment after rendering
		vk::AttachmentLoadOp::eDontCare, // Stencil load op: what to do with the stencil data in the attachment before rendering 
		vk::AttachmentStoreOp::eDontCare, // Stencil store op: what to do with the stencil data in the attachment after rendering
		vk::ImageLayout::eUndefined, // Initial layout: the layout the image will have before the render pass begins (don't care)
#if ENABLE_MULTISAMPLING
		vk::ImageLayout::eColorAttachmentOptimal // Final layout: the layout to automatically transition to when the render pass finishes
#else
		vk::ImageLayout::ePresentSrcKHR
#endif
	};

	// Depth buffer attachment
	const vk::AttachmentDescription2KHR depth_attachment{
		{}, 
		findDepthFormat(), 
#if ENABLE_MULTISAMPLING
		m_msaaSamples,
#else
		vk::SampleCountFlagBits::e1,
#endif
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eDontCare, 
		vk::AttachmentLoadOp::eDontCare, 
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined, 
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};

#if ENABLE_MULTISAMPLING
	// Color attachment resolve (for multisampling)
	const vk::AttachmentDescription2KHR color_attachment_resolve{
		{}, 
		m_swapChainImageFormat, 
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eStore, 
		vk::AttachmentLoadOp::eDontCare, 
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined, 
		vk::ImageLayout::ePresentSrcKHR
	};
#endif

	const vk::AttachmentDescription2KHR attachment_descriptions[RENDER_PASS_ATTACHMENT_COUNT] = {
		std::move(color_attachment),
		std::move(depth_attachment)
#if ENABLE_MULTISAMPLING
		, std::move(color_attachment_resolve)
#endif
	};

	// Reference attachments using the indices of an array of attachments
	static constexpr vk::AttachmentReference2KHR color_attachment_reference{
		0, // Attachment: which attachment to reference by its index in the attachment descriptions array
		vk::ImageLayout::eColorAttachmentOptimal, // Layout: which layout we would like the attachment to have during a subpass that uses this reference (using color buffer and wanting optimal performance)
		{} // Aspect mask: Only used for input attachents
	};

	static constexpr vk::AttachmentReference2KHR depth_attachment_reference{
		1, 
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};

#if ENABLE_MULTISAMPLING
	static constexpr vk::AttachmentReference2KHR color_attachment_resolve_reference{
		2, 
		vk::ImageLayout::eColorAttachmentOptimal
	};
#endif

	// Subsequent rendering operations that depend on the contents of framebuffers in previous passes
	constexpr vk::SubpassDescription2KHR subpass{
		{}, // Flags
		vk::PipelineBindPoint::eGraphics, // Pipeline bind point: type of subpass (graphics subpass)
		0, // View mask
		0, // Input attachment count: number of input attachments (not using any)
		nullptr, // Input attachments: attachments that are read from a shader (not using any)
		1, // Color attachment count: number of color attachments (using 1)
		&color_attachment_reference, // Color attachments: attachments used for outputing pixel color (this is index 0 of the color attachment array, referenced from the fragment shader: layout(location = 0) out vec4 color)
#if ENABLE_MULTISAMPLING
		&color_attachment_resolve_reference, // Resolve attachments: Attachments used for multisampling color attachments
#else
		nullptr,
#endif
		&depth_attachment_reference, // Depth stencil attachment: Attachment for depth and stencil data (not using)
		0, // Preserve attachment count: number of preserve attachments (not using any)
		nullptr // Preserve attachments: attachments that are not used by this subpass, but for which the data must be preserved (not using any)
	};

	// Dependencies automatically takes care of image layout transitions
	constexpr vk::SubpassDependency2KHR subpass_dependency{
		VK_SUBPASS_EXTERNAL, // Src subpass (VK_SUBPASS_EXTERNAL refers to the implicit subpass before or after the render pass)
		0, // Dst subpass; must always be higher than srcSubpass to prevent cycles in the dependency graph; except for VK_SUBPASS_EXTERNAL (index 0 refers to the only created subpass)
		vk::PipelineStageFlagBits::eColorAttachmentOutput |
		vk::PipelineStageFlagBits::eEarlyFragmentTests, // Src stage mask: the stages to wait on
		vk::PipelineStageFlagBits::eColorAttachmentOutput |
		vk::PipelineStageFlagBits::eEarlyFragmentTests, // Dst stage mask
		{}, // Src access mask: the operations to wait on
		vk::AccessFlagBits::eColorAttachmentWrite|
		vk::AccessFlagBits::eDepthStencilAttachmentWrite, // Dst access mask
		{}, // Dependency flags
		0 // View offset
	};

	const vk::RenderPassCreateInfo2KHR render_pass_create_info{
		{}, // Flags
		RENDER_PASS_ATTACHMENT_COUNT, // Attachment count: number of attachments (using 1)
		attachment_descriptions, // Attachments: the array of attachments (using 1)
		1, // Subpass count: number of subpasses (using 1)
		&subpass, // Subpasses: array of subpasses (using 1)
		1, // Dependency count: number of dependencies (using 1)
		&subpass_dependency, // dependencies: array of subpass dependencies (not using any)
		0, // Correlated view mask count
		nullptr // Correlated view masks
	};

	// Create the render pass
	result = m_logicalDevice.createRenderPass2KHR(&render_pass_create_info, &m_allocationCallbacks, &m_parentGraphicsPipelineInfo.renderPass, m_dynamicLoader); // Extension function needs to be dynamically loaded
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Render Pass; Pointer: " << m_parentGraphicsPipelineInfo.renderPass);
}

void Application::createDescriptorSetLayout() noexcept
{
	vk::Result result;

	// Information for the shader bindings
	static constexpr vk::DescriptorSetLayoutBinding layout_bindings[DESCRIPTOR_SET_LAYOUT_BINDING_COUNT] = {
		{
			0, // Binding
			vk::DescriptorType::eUniformBuffer, // Descriptor type
			1, // Descriptor count: the number of values in the array of UBOs (only using 1)
			vk::ShaderStageFlagBits::eVertex, // Stage flags: the shader stages the UBO is referenced in
			nullptr // Immutable samplers: for image sampling
		},
		{
			1,
			vk::DescriptorType::eCombinedImageSampler,
			1,
			vk::ShaderStageFlagBits::eFragment
		}
	};

	// Information for the creation of the descriptor set
	constexpr vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{
		{}, // Flags
		DESCRIPTOR_SET_LAYOUT_BINDING_COUNT, // Binding count
		layout_bindings // Bindings
	};

	// Create the descriptor set
	result = m_logicalDevice.createDescriptorSetLayout(&descriptor_set_layout_create_info, &m_allocationCallbacks, &m_descriptorSetLayout);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Descriptor Set Layout; Pointer: " << m_descriptorSetLayout);
}

void Application::createPipelineCache() noexcept
{
	vk::Result result;

	std::streampos cache_size = 0;
	char *cache_data = nullptr;

	std::ifstream cache_file{
		"pipeline_cache_data.bin",
		std::ios::ate |
		std::ios::binary
	};

	[[likely]] if (cache_file.is_open()) {
		cache_size = cache_file.tellg();
		cache_file.seekg(0);

		cache_data = new char[static_cast<size_t>(cache_size)];
		cache_file.read(cache_data, cache_size);

		uint32_t header_length;
		vk::PipelineCacheHeaderVersion header_version;
		uint32_t vendor_ID;
		uint32_t device_ID;
		uint8_t cache_UUID[VK_UUID_SIZE];

		memcpy(&header_length, reinterpret_cast<uint8_t *>(cache_data) + 0, 4);
		memcpy(&header_version, reinterpret_cast<uint8_t *>(cache_data) + 4, 4);
		memcpy(&vendor_ID, reinterpret_cast<uint8_t *>(cache_data) + 8, 4);
		memcpy(&device_ID, reinterpret_cast<uint8_t *>(cache_data) + 12, 4);
		memcpy(cache_UUID, reinterpret_cast<uint8_t *>(cache_data) + 16, VK_UUID_SIZE);

		assert(header_length > 0);
		assert(header_version == vk::PipelineCacheHeaderVersion::eOne);
		assert(vendor_ID == m_physicalDeviceProperties.properties.vendorID);
		assert(device_ID == m_physicalDeviceProperties.properties.deviceID);
		assert(memcmp(cache_UUID, m_physicalDeviceProperties.properties.pipelineCacheUUID.data(), VK_UUID_SIZE) == 0);
	}

	const vk::PipelineCacheCreateInfo pipeline_cache_create_info{
		{}, // Flags
		static_cast<size_t>(cache_size), // Inital data size
		cache_data // Inital data: previously retrieved pipeline cache data
	};

	result = m_logicalDevice.createPipelineCache(&pipeline_cache_create_info, &m_allocationCallbacks, &m_graphicsPipelineCache);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Pipeline Cache; Pointer: " << m_graphicsPipelineCache);

	delete[] cache_data;
}

void Application::createGraphicsPipeline() noexcept
{
	vk::Result result;
	vk::GraphicsPipelineCreateInfo pipeline_create_info;

	if (!m_parentGraphicsPipelineInfo.created) {
		const std::vector<char> vertex_shader_code = readFile(std::move("bin/vert.spv"));
		const std::vector<char> fragment_shader_code = readFile(std::move("bin/frag.spv"));

		m_parentGraphicsPipelineInfo.vertexShaderModule = createShaderModule(vertex_shader_code);
		m_parentGraphicsPipelineInfo.fragmentShaderModule = createShaderModule(fragment_shader_code);

		// Shader stages
		m_parentGraphicsPipelineInfo.stages[0] = {
			{}, // Flags
			vk::ShaderStageFlagBits::eVertex, // Stage: shader Stage/Type (Vertex)
			m_parentGraphicsPipelineInfo.vertexShaderModule, // Module: shader module containing the code
			"main", // Name: entrypoint of the shader (main function)
			{} // Specialization Info: specify values for shader constants (not using)
		};

		m_parentGraphicsPipelineInfo.stages[1] = {
			{},
			vk::ShaderStageFlagBits::eFragment,
			m_parentGraphicsPipelineInfo.fragmentShaderModule,
			"main"
		};

		// Vertex information
		m_parentGraphicsPipelineInfo.vertexBindingDescription = Vertex::getBindingDescription();
		m_parentGraphicsPipelineInfo.vertexAttributeDescriptions = Vertex::getAttributeDescriptions();

		// Describes the format of the vertex data that will be passed to the vertex shader
		m_parentGraphicsPipelineInfo.vertexInputState = {
			{}, // Flags
			VERTEX_INPUT_BINDING_DESCRIPTION_COUNT, // Vertex binding description count (no data being passed into vertex shader)
			&m_parentGraphicsPipelineInfo.vertexBindingDescription, // Vertex binding descriptions: points to an array of structs that describe the details for loading vertex bindings (no data is being loaded)
			VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT, // Vertex attribute description count (no data being passed into vertex shader)
			m_parentGraphicsPipelineInfo.vertexAttributeDescriptions.data() // Vertex attribute descriptions: points to an array of structs that describe the details for loading vertex attributes (no data is being loaded)
		};

		// Describes what kind of geometry will be drawn from the vertices and if primitive restart should be enabled
		m_parentGraphicsPipelineInfo.inputAssemblyState = {
			{}, // Flags
			vk::PrimitiveTopology::eTriangleList, // Topology: the kind of geometry that will be drawn from the vertices (1 triangle from every 3 vertices without reuse)
			VK_FALSE // Primitive restart: can break up lines and triangles in the strip topology modes (not using strip topology mode)
		};

		// Describes the region of the framebuffer that the output will be rendered to
		// This will almost always be (0, 0) to (width, height)
		m_parentGraphicsPipelineInfo.viewports[0] = vk::Viewport{
			0.0f, // X
			0.0f, // Y
			static_cast<float>(m_swapChainExtent.width), // Width
			static_cast<float>(m_swapChainExtent.height), // Height
			0.0f, // Min depth: the minimum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
			1.0f // Max depth: the maximum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
		};

		// Defines which regions pixels will actually be stored
		// Any pixels outside the scissor rectangles will be discarded by the rasterizer
		m_parentGraphicsPipelineInfo.scissors[0] = vk::Rect2D{
			{
				0, // X
				0 // Y
			}, // Offset
			m_swapChainExtent // Extent (the chosen extent of the screen)
		};

		// Viewport state containing viewport and scissor rectangle
		m_parentGraphicsPipelineInfo.viewportState = {
			{}, // Flags
			PIPELINE_VIEWPORT_COUNT, // Viewport count: number of viewports
			m_parentGraphicsPipelineInfo.viewports, // Viewports: pointer to array of viewports being used
			PIPELINE_SCISSOR_COUNT, // Scissor count: number of scissor rectangles
			m_parentGraphicsPipelineInfo.scissors // Scissors: pointer to array of scissor rectangles being used
		};
		
		// Takes the geometry that is shaped by the vertices from the vertex shader and turns it into fragments to be colored by the fragment shader
		// Performs depth testing, face culling and the scissor test, and it can be configured to output fragments that fill entire polygons or just the edges
		m_parentGraphicsPipelineInfo.rasterizationState = {
			{}, // Flags
			VK_FALSE, // Depth clamp enable: if true, then fragments that are beyond the near and far planes are clamped to them as opposed to discarding them (not using)
			VK_FALSE, // Rasterizer discard enable: if true, then geometry never passes through the rasterizer stage; disables any output to the framebuffer
			vk::PolygonMode::eFill, // Polygon mode: how fragments are generated for geometry (fill the area of the polygon with fragments)
			vk::CullModeFlagBits::eBack, // Cull mode: the type of face culling to use (cull the back faces)
			vk::FrontFace::eCounterClockwise, // Front face: specifies the vertex order for faces to be considered front-facing (counter-clockwise)
			VK_FALSE, // Depth bias enable: alters the depth values by adding a constant value or biasing them based on a fragment's slope (not using)
			0.0f, // Depth bias constant factor (not using)
			0.0f, // Depth bias clamp (not using)
			0.0f, // Depth bias slope factor (not using)
			1.0f // Line width: the thickness of lines in terms of number of fragments (1 fragment thick)
		};

		// Configures multisampling by combining the fragment shader results of multiple polygons that rasterize to the same pixel
		// Not using multisampling, so struct is disabled
		m_parentGraphicsPipelineInfo.multisampleState = {
			{}, // Flags
#if ENABLE_MULTISAMPLING
			m_msaaSamples, // Rasterization samples
			VK_TRUE, // Sample shading enable
#else
			vk::SampleCountFlagBits::e1,
			VK_FALSE,
#endif
			0.2f, // Min sample shading: min fraction for sample shading; closer to one is smoother
			nullptr, // Sample mask
			VK_FALSE, // Alpha to coverage enable
			VK_FALSE // Alpha to one enable
		};

		// Configures the depth and stencil tests
		m_parentGraphicsPipelineInfo.depthStencilState = {
			{}, // Flags
			VK_TRUE, // Depth test enable: whether the depth of new fragments should be compared to the depth buffer to see if they should be discarded
			VK_TRUE, // Depth write enable: whether the new depth of fragments that pass the depth test should actually be written to the depth buffer
			vk::CompareOp::eLess, // Depth compare op: the comparison that is performed to keep or discard fragments (less; lower depth = closer)
			VK_FALSE, // Depth bounds test enable
			VK_FALSE, // Stencil test enable
			{}, // Front
			{}, // Back
			0.0f, // Min depth bounds
			1.0f // Max depth bounds
		};

		// Contains the color blending configuration per attached framebuffer
		m_parentGraphicsPipelineInfo.colorBlendAttachments[0] = {
			VK_TRUE, // Blend enable
			vk::BlendFactor::eSrcAlpha, // Src color blend factor
			vk::BlendFactor::eOneMinusSrcAlpha, // Dst color blend factor
			vk::BlendOp::eAdd, // Color blend op
			vk::BlendFactor::eOne, // Src alpha blend factor
			vk::BlendFactor::eZero, // Dst alpha blend factor
			vk::BlendOp::eAdd, // Alpha blend op
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA // Color write mask
		};

		// Blend constants for color blend state
		m_parentGraphicsPipelineInfo.colorBlendConstants = {0.0f};

		// Contains the global color blending settings
		// References the array of structures for all of the framebuffers and allows you to set blend constants
		m_parentGraphicsPipelineInfo.colorBlendState = {
			{}, // Flags
			VK_FALSE, // Logic op enable: use bitwise combination for blending (disabled)
			vk::LogicOp::eCopy, // Logic op: the bitwise operation for blending
			PIPELINE_COLOR_BLEND_ATTACHMENT_COUNT, // Attachment count: number of color blend attachments
			m_parentGraphicsPipelineInfo.colorBlendAttachments, // Attachments: pointer to array of color blend attachments
			m_parentGraphicsPipelineInfo.colorBlendConstants // Blend constants
		};

#if ENABLE_PIPELINE_DYNAMIC_STATE
		m_parentGraphicsPipelineInfo.dynamicStates[0] = vk::DynamicState::eViewport;
		m_parentGraphicsPipelineInfo.dynamicStates[1] = vk::DynamicState::eScissor;

		// Pipeline settings that can and have to be dynamically changed at drawing time
		m_parentGraphicsPipelineInfo.dynamicState = {
			{}, // Flags
			PIPELINE_DYNAMIC_STATE_COUNT, // Dynamic state count
			m_parentGraphicsPipelineInfo.dynamicStates // Dynamic states
		};
#endif

		// Infomation for the pipeline layout
		m_parentGraphicsPipelineInfo.layoutInfo = {
			{}, // Flags
			1, // Set layout count
			&m_descriptorSetLayout, // Set layouts
			0, // Push constant range count
			nullptr // Push constant ranges
		};

		// Create pipeline layout
		result = m_logicalDevice.createPipelineLayout(&m_parentGraphicsPipelineInfo.layoutInfo, &m_allocationCallbacks, &m_parentGraphicsPipelineInfo.layout);
		assert(result == vk::Result::eSuccess);
		DEBUG_LOG("Created Vulkan Pipeline Layout; Pointer: " << m_parentGraphicsPipelineInfo.layout);

		m_parentGraphicsPipelineInfo.created = true;

		// Graphics pipeline info
		pipeline_create_info = {
			vk::PipelineCreateFlagBits::eAllowDerivatives, // Flags
			PIPELINE_SHADER_STAGE_COUNT, // Stage count: number of shader stages (2: vertex and fragment)
			m_parentGraphicsPipelineInfo.stages, // Stages: the array of shader stages
			&m_parentGraphicsPipelineInfo.vertexInputState, // Vertex input state
			&m_parentGraphicsPipelineInfo.inputAssemblyState, // Input assembly state
			&m_parentGraphicsPipelineInfo.tessellationState, // Tessellation state (ignored if the pipeline does not include a tessellation control shader stage and tessellation evaluation shader stage)
			&m_parentGraphicsPipelineInfo.viewportState, // Viewport state
			&m_parentGraphicsPipelineInfo.rasterizationState, // Rasterization state
			&m_parentGraphicsPipelineInfo.multisampleState, // Multisample state
			&m_parentGraphicsPipelineInfo.depthStencilState, // Depth stencil state
			&m_parentGraphicsPipelineInfo.colorBlendState, // Color blend state
			&m_parentGraphicsPipelineInfo.dynamicState, // Dynamic state
			m_parentGraphicsPipelineInfo.layout, // Layout
			m_parentGraphicsPipelineInfo.renderPass, // Render pass
			m_parentGraphicsPipelineInfo.subpass, // Subpass: index of the subpass where this graphics pipeline will be used
			VK_NULL_HANDLE, // Base pipeline handle
			-1 // Base pipeline index
		};

		// Create the parent graphics pipeline
		result = m_logicalDevice.createGraphicsPipelines(m_graphicsPipelineCache, 1, &pipeline_create_info, &m_allocationCallbacks, &m_parentGraphicsPipeline);
		assert(result == vk::Result::eSuccess);
		DEBUG_LOG("Created Vulkan Graphics Pipeline; Pointer: " << m_parentGraphicsPipeline);
	}
	else {
		pipeline_create_info = {
			vk::PipelineCreateFlagBits::eAllowDerivatives, 
			PIPELINE_SHADER_STAGE_COUNT, 
			m_parentGraphicsPipelineInfo.stages, 
			&m_parentGraphicsPipelineInfo.vertexInputState,
			&m_parentGraphicsPipelineInfo.inputAssemblyState, 
			&m_parentGraphicsPipelineInfo.tessellationState, 
			&m_parentGraphicsPipelineInfo.viewportState, 
			&m_parentGraphicsPipelineInfo.rasterizationState,
			&m_parentGraphicsPipelineInfo.multisampleState, 
			&m_parentGraphicsPipelineInfo.depthStencilState, 
			&m_parentGraphicsPipelineInfo.colorBlendState, 
			&m_parentGraphicsPipelineInfo.dynamicState,
			m_parentGraphicsPipelineInfo.layout, 
			m_parentGraphicsPipelineInfo.renderPass,
			m_parentGraphicsPipelineInfo.subpass,
			VK_NULL_HANDLE,
			-1 
		};
	}

	// Modify vk::GraphicsPipelineCreateInfo for child pipeline
	pipeline_create_info.flags = vk::PipelineCreateFlagBits::eDerivative;
	pipeline_create_info.basePipelineHandle = m_parentGraphicsPipeline;

	m_parentGraphicsPipelineInfo.rasterizationState.cullMode = vk::CullModeFlagBits::eNone;

	// Create the child pipeline
	result = m_logicalDevice.createGraphicsPipelines(m_graphicsPipelineCache, 1, &pipeline_create_info, &m_allocationCallbacks, &m_childGraphicsPipeline);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Graphics Pipeline; Pointer: " << m_childGraphicsPipeline);
}

void Application::createFrameBuffers() noexcept
{
	vk::Result result;

	[[unlikely]] if (m_swapChainFramebuffers == nullptr) // Unlikely because this function may be called many times, but this branch will only be gone into the first call
		m_swapChainFramebuffers = new vk::Framebuffer[m_swapChainImageCount];

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		const vk::ImageView attachments[FRAMEBUFFER_ATTACHMENT_COUNT] = {
#if ENABLE_MULTISAMPLING
			m_colorImageView,
			m_depthImageView,
			m_swapChainImageViews[i]
#else
			m_swapChainImageViews[i],
			m_depthImageView
#endif
		};

		const vk::FramebufferCreateInfo framebuffer_create_info{
			{}, // Flags
			m_parentGraphicsPipelineInfo.renderPass, // Render pass
			FRAMEBUFFER_ATTACHMENT_COUNT, // Attachment count
			attachments, // Attachments: the vk::ImageView objects that should be bound to the respective attachment descriptions in the render pass Attachment array
			m_swapChainExtent.width, // Width
			m_swapChainExtent.height, // Height
			1 // Layers: number of layers in image arrays (using single images)
		};

		// Create the framebuffer
		result = m_logicalDevice.createFramebuffer(&framebuffer_create_info, &m_allocationCallbacks, &m_swapChainFramebuffers[i]);
		assert(result == vk::Result::eSuccess);
		DEBUG_LOG("Created Vulkan Framebuffer; Pointer: " << m_swapChainFramebuffers[i]);
	}
}

void Application::createCommandPools() noexcept
{
	vk::Result result;

	// Information for the graphics command pool
	const vk::CommandPoolCreateInfo graphics_command_pool_create_info{
		{}, // Flags
		m_queueFamilyIndices.graphicsFamily.value() // Queue family index
	};

	// Information for the transfer command pool
	const vk::CommandPoolCreateInfo transfer_command_pool_create_info{
		vk::CommandPoolCreateFlagBits::eTransient, // Uses temporary/short-lived command buffers
		m_queueFamilyIndices.transferFamily.value()
	};

	// Creating the command pools
	result = m_logicalDevice.createCommandPool(&graphics_command_pool_create_info, &m_allocationCallbacks, &m_graphicsCommandPool);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Command Pool; Pointer: " << m_graphicsCommandPool);

	result = m_logicalDevice.createCommandPool(&transfer_command_pool_create_info, &m_allocationCallbacks, &m_transferCommandPool);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Command Pool; Pointer: " << m_transferCommandPool);
}

#if ENABLE_MULTISAMPLING

void Application::createColorResources() noexcept
{
	// Create the color image
	createImage(
		m_swapChainExtent.width, 
		m_swapChainExtent.height, 
		1, 
		m_msaaSamples,
		m_swapChainImageFormat, 
		vk::ImageTiling::eOptimal, 
		vk::ImageUsageFlagBits::eTransientAttachment | 
		vk::ImageUsageFlagBits::eColorAttachment, 
		vk::MemoryPropertyFlagBits::eDeviceLocal, 
		m_colorImage, 
		m_colorImageMemory
	);

	// Create the color image view
	m_colorImageView = createImageView(
		m_colorImage,
		m_swapChainImageFormat,
		vk::ImageAspectFlagBits::eColor,
		1
	);
}

#endif // ENABLE_MULTISAMPLING

void Application::createDepthResources() noexcept
{
	// Get the supported depth format
	const vk::Format depth_format = findDepthFormat();

	// Create the depth image
	createImage(
		m_swapChainExtent.width,
		m_swapChainExtent.height,
		1,
#if ENABLE_MULTISAMPLING
		m_msaaSamples,
#else
		vk::SampleCountFlagBits::e1,
#endif
		depth_format,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		m_depthImage,
		m_depthImageMemory
	);

	// Create the depth image view
	m_depthImageView = createImageView(
		m_depthImage,
		depth_format,
		vk::ImageAspectFlagBits::eDepth,
		1
	);

	// Explicitly transitioning the image is not needed, as it will happen in the render pass, but is good to keep here for learning
#if 0
	if (hasStencilComponent(depth_format)) [[likely]] {
		transitionImageLayout(
			m_depthImage,
			depth_format,
			vk::ImageAspectFlagBits::eDepth |
			vk::ImageAspectFlagBits::eStencil,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal,
			1
		);
	} 
	else [[unlikely]] {
		transitionImageLayout(
			m_depthImage,
			depth_format,
			vk::ImageAspectFlagBits::eDepth,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal,
			1
		);
	}
#endif
}

void Application::createTextureImage() noexcept
{
	vk::Result result;

	// Dimensions of the texture
	int tex_width, tex_height, tex_channels;

	// Load the texture
	stbi_uc * const pixels = stbi_load(
		VIKING_TEXTURE_PATH, // Filename
		&tex_width, // X: width
		&tex_height, // Y: height
		&tex_channels, // Channels in file
		STBI_rgb_alpha // Desired channels
	);

	// Check if the texture loaded
	assert(pixels && "ASSERT: Failed to load texture");

	// Size of the pixels array
	const vk::DeviceSize image_size = static_cast<vk::DeviceSize>(tex_width * tex_height * 4);

#if ENABLE_MIPMAPS
	// Number of mip levels
	m_textureMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(tex_width, tex_height)))) + 1;
#endif

	// Map and fill a staging buffer
	vk::Buffer staging_buffer;
	vk::DeviceMemory staging_buffer_memory;

	createBuffer(
		image_size,
		vk::BufferUsageFlagBits::eTransferSrc, 
		vk::MemoryPropertyFlagBits::eHostVisible | 
		vk::MemoryPropertyFlagBits::eHostCoherent,
		staging_buffer, 
		staging_buffer_memory 
	);

	void *mapped_data;
	result = m_logicalDevice.mapMemory(staging_buffer_memory, 0, VK_WHOLE_SIZE, {}, &mapped_data);
	assert(result == vk::Result::eSuccess);

	memcpy(mapped_data, pixels, static_cast<size_t>(image_size));

	m_logicalDevice.unmapMemory(staging_buffer_memory);
	
	// Free the memory used for the image
	stbi_image_free(static_cast<void *>(pixels));

	// Create the image
	createImage(
		static_cast<uint32_t>(tex_width), // Width
		static_cast<uint32_t>(tex_height), // Height
#if ENABLE_MIPMAPS
		m_textureMipLevels, // Mip levels
#else
		1,
#endif
		vk::SampleCountFlagBits::e1, // Sample count
		vk::Format::eR8G8B8A8Srgb, // Format
		vk::ImageTiling::eOptimal, // Tiling
		vk::ImageUsageFlagBits::eTransferDst |
		vk::ImageUsageFlagBits::eTransferSrc |
		vk::ImageUsageFlagBits::eSampled, // Usage
		vk::MemoryPropertyFlagBits::eDeviceLocal, // Properties
		m_textureImage, // Image
		m_textureImageMemory // Image memory
	);

	// Transition the image to TransferDstOptimal
	transitionImageLayout(
		m_textureImage, // Image
		vk::Format::eR8G8B8A8Srgb, // Format
		vk::ImageAspectFlagBits::eColor, // Image aspect flags
		vk::ImageLayout::eUndefined, // Old layout
		vk::ImageLayout::eTransferDstOptimal, // New layout
#if ENABLE_MIPMAPS
		m_textureMipLevels // Mip levels
#else
		1
#endif
	);

	// Copy the staging buffer data to the image
	copyBufferToImage(
		staging_buffer, // Src buffer
		m_textureImage, // Dst image
		static_cast<uint32_t>(tex_width), // Width
		static_cast<uint32_t>(tex_height) // Height
	);

#if ENABLE_MIPMAPS
	// Generate the mipmaps
	generateMipmaps(
		m_textureImage,
		vk::Format::eR8G8B8A8Srgb,
		tex_width,
		tex_height,
		m_textureMipLevels
	);
#else
	// Transition the image for shader read-only access
	transitionImageLayout(
		m_textureImage,
		vk::Format::eR8G8B8A8Srgb,
		vk::ImageAspectFlagBits::eColor,
		vk::ImageLayout::eTransferDstOptimal,
		vk::ImageLayout::eShaderReadOnlyOptimal,
		1
	);
#endif

	// Memory cleanup
	m_logicalDevice.destroyBuffer(staging_buffer, &m_allocationCallbacks);
	m_logicalDevice.freeMemory(staging_buffer_memory, &m_allocationCallbacks);
}

void Application::createTextureImageView() noexcept
{
	m_textureImageView = createImageView(
		m_textureImage, 
		vk::Format::eR8G8B8A8Srgb, 
		vk::ImageAspectFlagBits::eColor, 
#if ENABLE_MIPMAPS
		m_textureMipLevels
#else	
		1
#endif
	);
}

void Application::createTextureSampler() noexcept
{
	vk::Result result;

	// Information for creating a sampler
	const vk::SamplerCreateInfo sampler_create_info{
		{}, // Flags
		vk::Filter::eLinear, // Mag filter
		vk::Filter::eLinear, // Min filter
		vk::SamplerMipmapMode::eLinear, // Mipmap mode
		vk::SamplerAddressMode::eRepeat, // Address mode U (X)
		vk::SamplerAddressMode::eRepeat, // Address mode V (Y)
		vk::SamplerAddressMode::eRepeat, // Address mode W (Z)
		0.0f, // Mip lod bias
		VK_TRUE, // Anisotropy enable
		m_physicalDeviceProperties.properties.limits.maxSamplerAnisotropy, // Max anisotropy (max quality)
		VK_FALSE, // Compare enable
		vk::CompareOp::eAlways, // Compare op
		0.0f, // Min lod
#if ENABLE_MIPMAPS
		static_cast<float>(m_textureMipLevels), // Max lod
#else
		1.0f,
#endif
		vk::BorderColor::eIntOpaqueBlack, // Boarder color: for ClampToBoarder address mode
		VK_FALSE // Unnormalized coordinates
	};

	// Create the texture sampler
	result = m_logicalDevice.createSampler(&sampler_create_info, &m_allocationCallbacks, &m_textureSampler);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Sampler; Pointer: " << m_textureSampler);
}

void Application::loadModel() noexcept
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	// Load the object file
#ifdef NDEBUG
	tinyobj::LoadObj(
		&attrib, // Attrib
		&shapes, // Shapes
		&materials, // Materials
		&warn, // Warn
		&err, // Err
		VIKING_MODEL_PATH, // Filename
		nullptr, // Mtl basedir (optional)
		true, // Triangulate (optional)
		true // Default vcols fallback (optional)
	);
#else
	assert(tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, VIKING_MODEL_PATH) && "ASSERT: Failed to load object file");
#endif

	std::unordered_map<Vertex, uint32_t> unique_vertices;

	for (const auto &shape : shapes) {
		for (const auto &index : shape.mesh.indices) {
			const Vertex vertex{
				.position = 
				{
					attrib.vertices[static_cast<size_t>(3 * index.vertex_index + 0)],
					attrib.vertices[static_cast<size_t>(3 * index.vertex_index + 1)],
					attrib.vertices[static_cast<size_t>(3 * index.vertex_index + 2)]
				},

				.color = 
				{
					1.0f, 
					1.0f, 
					1.0f
				},

				.texCoord = 
				{
					attrib.texcoords[static_cast<size_t>(2 * index.texcoord_index + 0)],
					1.0f - attrib.texcoords[static_cast<size_t>(2 * index.texcoord_index + 1)]
				}
			};

			if (unique_vertices.count(vertex) == 0) { // If vertex already exists
				unique_vertices[vertex] = static_cast<uint32_t>(m_vertices.size());
				m_vertices.emplace_back(std::move(vertex));
			}

			m_indices.emplace_back(unique_vertices[vertex]);
		}
	}
}

void Application::createVertexBuffer() noexcept
{
	// Create and fill the vertex buffer
	createAndFillBuffer<Vertex>( // Type of data (template arg)
		m_vertices.data(), // Data
		m_vertices.size(), // Count
		vk::BufferUsageFlagBits::eVertexBuffer, // Usage
		m_vertexBuffer, // Buffer
		m_vertexBufferMemory // Buffer memory
	);
}

void Application::createIndexBuffer() noexcept
{
	createAndFillBuffer<Index>(
		m_indices.data(),
		m_indices.size(),
		vk::BufferUsageFlagBits::eIndexBuffer,
		m_indexBuffer,
		m_indexBufferMemory
	);
}

void Application::createUniformBuffers() noexcept
{
	[[unlikely]] if (m_uniformBuffers == nullptr) {
		m_uniformBuffers = new vk::Buffer[m_swapChainImageCount];
		m_uniformBuffersMemory = new vk::DeviceMemory[m_swapChainImageCount];
	}

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		createBuffer(
			sizeof(UniformBufferObject),
			vk::BufferUsageFlagBits::eUniformBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible |
			vk::MemoryPropertyFlagBits::eHostCoherent,
			m_uniformBuffers[i],
			m_uniformBuffersMemory[i]
		);
	}
}

void Application::createDescriptorPool() noexcept
{
	vk::Result result;

	const vk::DescriptorPoolSize ubo_descriptor_pool_size{
		vk::DescriptorType::eUniformBuffer, // Type
		m_swapChainImageCount // Descriptor count
	};

	const vk::DescriptorPoolSize sampler_descriptor_pool_size{
		vk::DescriptorType::eCombinedImageSampler, 
		m_swapChainImageCount
	};

	const vk::DescriptorPoolSize descriptor_pool_sizes[DESCRIPTOR_POOL_SIZE_COUNT] = {
		std::move(ubo_descriptor_pool_size),
		std::move(sampler_descriptor_pool_size)
	};

	const vk::DescriptorPoolCreateInfo descriptor_pool_create_info{
		{}, // Flags
		m_swapChainImageCount, // Max sets
		DESCRIPTOR_POOL_SIZE_COUNT, // Pool size count
		descriptor_pool_sizes // Pool sizes
	};

	// Create the descriptor pool
	result = m_logicalDevice.createDescriptorPool(&descriptor_pool_create_info, &m_allocationCallbacks, &m_descriptorPool);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Descriptor Pool; Pointer: " << m_descriptorPool);
}

void Application::createDescriptorSets() noexcept
{
	vk::Result result;

	[[unlikely]] if (m_descriptorSets == nullptr)
		m_descriptorSets = new vk::DescriptorSet[m_swapChainImageCount];

	vk::DescriptorSetLayout * const descriptor_set_layouts = new vk::DescriptorSetLayout[m_swapChainImageCount];
	for (uint32_t i = 0; i < m_swapChainImageCount; ++i)
		descriptor_set_layouts[i] = m_descriptorSetLayout;

	const vk::DescriptorSetAllocateInfo descriptor_set_allocate_info{
		m_descriptorPool, // Descriptor pool
		m_swapChainImageCount, // Descriptor set count
		descriptor_set_layouts // Set layouts
	};

	result = m_logicalDevice.allocateDescriptorSets(&descriptor_set_allocate_info, m_descriptorSets);
	assert(result == vk::Result::eSuccess);
#ifdef _DEBUG
	const auto printDescriptorSetLayouts = [&]{
		for (uint32_t i = 0; i < descriptor_set_allocate_info.descriptorSetCount; ++i)
			std::cout << "\n\t\tLayout " << (i + 1) << ": " << descriptor_set_allocate_info.pSetLayouts[i];
		
		return ' ';
	};

	const auto printDescriptorSetPointers = [&]{
		for (uint32_t i = 0; i < descriptor_set_allocate_info.descriptorSetCount; ++i)
			std::cout << "\n\t\tPointer " << (i + 1) << ": " << m_descriptorSets[i];
		
		return ' ';
	};

	DEBUG_LOG("Allocated Vulkan Descriptor Sets; Descriptor Pool: " << descriptor_set_allocate_info.descriptorPool << ", Count" << descriptor_set_allocate_info.descriptorSetCount << printDescriptorSetLayouts() << printDescriptorSetPointers());
#endif

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		const vk::DescriptorBufferInfo descriptor_buffer_info{
			m_uniformBuffers[i], // Buffer
			0, // Offset
			VK_WHOLE_SIZE // Range
		};

		const vk::DescriptorImageInfo descriptor_image_info{
			m_textureSampler, // Sampler
			m_textureImageView, // Image view
			vk::ImageLayout::eShaderReadOnlyOptimal // Image layout
		};

		const vk::WriteDescriptorSet buffer_descriptor_write{
			m_descriptorSets[i], // Dst set: the descriptor set to update
			0, // Dst binding: the uniform binding; layout(binding = 0)
			0, // Dst array element: the first index in the array of uniform buffers that we want to update
			1, // Descriptor count: how many array elements to update
			vk::DescriptorType::eUniformBuffer, // Descriptor type
			nullptr, // Image info: used if refering to image data
			&descriptor_buffer_info, // Buffer info: used if refering to buffers
			nullptr // Texel buffer view: used if refering to buffer views
		};

		const vk::WriteDescriptorSet sampler_descriptor_write{
			m_descriptorSets[i],
			1,
			0,
			1,
			vk::DescriptorType::eCombinedImageSampler,
			&descriptor_image_info
		};

		const vk::WriteDescriptorSet descriptor_writes[DESCRIPTOR_SET_WRITE_COUNT] = {
			std::move(buffer_descriptor_write),
			std::move(sampler_descriptor_write)
		};

		m_logicalDevice.updateDescriptorSets(
			DESCRIPTOR_SET_WRITE_COUNT, // Descriptor write count
			descriptor_writes, // Descriptor writes
			0, // Descriptor copy count
			nullptr // Descriptor copies
		);
	}

	delete[] descriptor_set_layouts;
}

void Application::createCommandBuffers() noexcept
{
	vk::Result result;

	[[unlikely]] if (m_commandBuffers == nullptr) 
		m_commandBuffers = new vk::CommandBuffer[m_swapChainImageCount];

	// Infomation for allocating memory
	const vk::CommandBufferAllocateInfo command_buffer_allocate_info{
		m_graphicsCommandPool, // Command pool
		vk::CommandBufferLevel::ePrimary, // Level: if the allocated command buffers are primary or secondary command buffers
		m_swapChainImageCount // Command buffer count
	};

	result = m_logicalDevice.allocateCommandBuffers(&command_buffer_allocate_info, m_commandBuffers);
	assert(result == vk::Result::eSuccess);
#ifdef _DEBUG
	const auto printCommandBufferPointers = [&]{
		for (uint32_t i = 0; i < command_buffer_allocate_info.commandBufferCount; ++i)
			std::cout << "\n\t\tPointer " << (i + 1) << ": " << m_descriptorSets[i];
		
		return ' ';
	};

	DEBUG_LOG("Allocated Vulkan Command Buffers; Command Pool: " << command_buffer_allocate_info.commandPool << ", Level: "  << static_cast<uint32_t>(command_buffer_allocate_info.level) << ", Count: " << command_buffer_allocate_info.commandBufferCount << printCommandBufferPointers());
#endif

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		constexpr vk::CommandBufferBeginInfo command_buffer_begin_info{
			{}, // Flags
			nullptr // Inheritance info: which state to inherit from the calling primary command buffers (already are primary)
		};

		// Begin the command buffer
		result = m_commandBuffers[i].begin(&command_buffer_begin_info);
		assert(result == vk::Result::eSuccess);

		// Defines where shader loads and stores will take place (matches the size of the attachments for the best performance)
		const vk::Rect2D render_area{
			{
				0, // X
				0 // Y
			}, // Offset
			m_swapChainExtent // Extent
		};

		// Wrapper over the clear color value
		const vk::ClearValue clear_color{
			vk::ClearColorValue{
				std::array<float, 4>{
					CLEAR_COLOR_R, // Red
					CLEAR_COLOR_G, // Green
					CLEAR_COLOR_B, // Blue
					CLEAR_COLOR_A // Alpha
				} // Temporary array
			} // Temporary vk::ClearColorValue
		};

		// Wrapper over depth stencil clear value
		const vk::ClearValue clear_depth_stencil{
			vk::ClearDepthStencilValue{
				1.0f, // Depth
				0 // Stencil
			}
		};

		// Create array of clear values
		const vk::ClearValue clear_values[CLEAR_VALUE_COUNT] = {
			std::move(clear_color),
			std::move(clear_depth_stencil)
		};

		// Command to render to the screen
		const vk::RenderPassBeginInfo render_pass_begin_info{
			m_parentGraphicsPipelineInfo.renderPass, // Render pass
			m_swapChainFramebuffers[i], // Framebuffer
			render_area, // Render area
			CLEAR_VALUE_COUNT, // Clear value count
			clear_values // Clear values
		};

		constexpr vk::SubpassBeginInfoKHR subpass_begin_info{
			vk::SubpassContents::eInline // Contents (inline; the render pass commands will be embedded in the primary command buffer itself)
		};

		// Begin the render pass
		m_commandBuffers[i].beginRenderPass2KHR(&render_pass_begin_info, &subpass_begin_info, m_dynamicLoader);
		
		// Bind the graphics pipeline with the command buffer
		m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_childGraphicsPipeline);

		// Array of offsets
		const vk::DeviceSize offsets[VERTEX_BUFFER_COUNT] = {
			0
		};

		// Bind the vertex buffers
		m_commandBuffers[i].bindVertexBuffers(
			0, // First binding: offset
			VERTEX_BUFFER_COUNT, // Binding count: buffer count
			&m_vertexBuffer, // Buffers
			offsets // Offsets: byte offset to start reading vertex data from
		);

		// Bind the index buffer (can only bind one)
		m_commandBuffers[i].bindIndexBuffer(
			m_indexBuffer, // Buffer
			0, // Offset
			vk::IndexType::eUint32 // Index type
		);

		// Bind the descriptor sets (uniform buffer)
		m_commandBuffers[i].bindDescriptorSets(
			vk::PipelineBindPoint::eGraphics, // Pipeline bind point: descriptor sets are not unique to graphics pipelines; specify if we want to bind descriptor sets to the graphics or compute pipeline
			m_parentGraphicsPipelineInfo.layout, // Layout: the layout that the descriptors are based on
			0, // First set: the index of the first descriptor set
			1, // Descriptor set count
			m_descriptorSets, // Descriptor sets
			0, // Dynamic offset count
			nullptr // Dynamic offsets
		);

#if ENABLE_PIPELINE_DYNAMIC_STATE
		m_parentGraphicsPipelineInfo.viewports[0] = std::move(vk::Viewport{
			0.0f, // X
			0.0f, // Y
			static_cast<float>(m_swapChainExtent.width), // Width
			static_cast<float>(m_swapChainExtent.height), // Height
			0.0f, // Min depth: the minimum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
			1.0f // Max depth: the maximum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
		});

		// Set the graphics pipeline viewport
		m_commandBuffers[i].setViewport(
			0, // First viewport
			PIPELINE_VIEWPORT_COUNT, // Viewport count
			m_parentGraphicsPipelineInfo.viewports // Viewports
		);

		m_parentGraphicsPipelineInfo.scissors[0] = std::move(vk::Rect2D{
			{
				0, // X
				0 // Y
			}, // Offset
			m_swapChainExtent // Extent (the chosen extent of the screen)
		});

		// Set the graphics pipeline scissor rectangle
		m_commandBuffers[i].setScissor(
			0, // First scissor
			PIPELINE_SCISSOR_COUNT, // Scissor count
			m_parentGraphicsPipelineInfo.scissors // Scissors
		);
#endif

		// DRAW THE GEOMETRY!!!!
		m_commandBuffers[i].drawIndexed(
			static_cast<uint32_t>(m_indices.size()), // Index count
			1, // Instance count: used for instanced rendering (not doing, so 1)
			0, // First index: a value offset into the index buffer
			0, // Vertex offset: an offset to add to the indices in the index buffer
			0 // First instance: an offset for instanced rendering; defines the lowest value of gl_InstanceIndex
		);

		// Information for ending the renderpass
		constexpr vk::SubpassEndInfoKHR subpass_end_info; // No constructor paramaters

		// End the render pass
		m_commandBuffers[i].endRenderPass2KHR(&subpass_end_info, m_dynamicLoader);

		// End the command buffer
		result = m_commandBuffers[i].end();
		assert(result == vk::Result::eSuccess);
	}
}

void Application::createSyncObjects() noexcept
{
	vk::Result result;

	// Create the right amount of fences
	[[unlikely]] if (m_imagesInFlight == nullptr)
		m_imagesInFlight = new vk::Fence[m_swapChainImageCount];

	// Information for semaphore creation (currently there is basically no info required)
	constexpr vk::SemaphoreCreateInfo semaphore_create_info{
		vk::SemaphoreCreateFlags{} // Flags
	};

	// Information for fence creation (currently there is basically no info required)
	constexpr vk::FenceCreateInfo fence_create_info{
		vk::FenceCreateFlagBits::eSignaled // Flags (initializes the flag as signaled)
	};

	// Create the semaphores
	for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		result = m_logicalDevice.createSemaphore(&semaphore_create_info, &m_allocationCallbacks, &m_imageAvailableSemaphores[i]);
		assert(result == vk::Result::eSuccess);
		DEBUG_LOG("Created Vulkan Semaphore; Pointer: " << m_imageAvailableSemaphores[i]);

		result = m_logicalDevice.createSemaphore(&semaphore_create_info, &m_allocationCallbacks, &m_renderFinishedSemaphores[i]);
		assert(result == vk::Result::eSuccess);
		DEBUG_LOG("Created Vulkan Semaphore; Pointer: " << m_renderFinishedSemaphores[i]);

		result = m_logicalDevice.createFence(&fence_create_info, &m_allocationCallbacks, &m_inFlightFences[i]);
		assert(result == vk::Result::eSuccess);
		DEBUG_LOG("Created Vulkan Fence; Pointer: " << m_inFlightFences[i]);
	}
}

// Check if validation layers are supported
template <size_t S>
bool Application::areLayersSupported(const char * const * const layers) noexcept
{
	vk::Result result;
	vk::LayerProperties *layer_properties;
	uint32_t layer_property_count;

	result = vk::enumerateInstanceLayerProperties(&layer_property_count, nullptr);
	assert(result == vk::Result::eSuccess);

	layer_properties = new vk::LayerProperties[layer_property_count];
	result = vk::enumerateInstanceLayerProperties(&layer_property_count, layer_properties);
	assert(result == vk::Result::eSuccess);

	bool found;
	for (size_t i = 0; i < S; ++i) {
		found = false;
		for (uint32_t j = 0; j < layer_property_count; ++j) {
			if (strcmp(layers[i], static_cast<const char *>(layer_properties[j].layerName)) == 0) {
				found = true;
				break;
			}
		}

		[[unlikely]] if (!found) {
			delete[] layer_properties;
			return false;
		}
	}

	delete[] layer_properties;
	return true;
}

std::vector<const char *> Application::getRequiredInstanceExtensions() noexcept
{
	uint32_t glfw_extension_count;
	const char * const * const glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count); // Get required vulkan extensions for glfw
	std::vector<const char *> extensions = {glfw_extensions, glfw_extensions + glfw_extension_count};

#ifdef _DEBUG
	extensions.reserve(2);
	extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
	extensions.emplace_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
	return extensions;
}

// Many different ways of selecting a suitable GPU
bool Application::physicalDeviceSupportsRequirements(const vk::PhysicalDevice &physical_device) noexcept
{
	const QueueFamilyIndices indices = getQueueFamilies(physical_device);
	const SwapChainSupportDetails swap_chain_support_details = querySwapChainSupport(physical_device);

	vk::PhysicalDeviceFeatures2 features;
	physical_device.getFeatures2(&features);

	return 	indices.isComplete() && 
			physicalDeviceSupportsExtensions(physical_device) && 
			swapChainAdequate(swap_chain_support_details) && 
			features.features.samplerAnisotropy;
}

QueueFamilyIndices Application::getQueueFamilies(const vk::PhysicalDevice &physical_device) noexcept
{
	vk::Result result;
	QueueFamilyIndices indices;
	uint32_t queue_family_property_count;
	vk::QueueFamilyProperties2 *queue_family_properties; 

	physical_device.getQueueFamilyProperties2(&queue_family_property_count, nullptr);
	queue_family_properties = new vk::QueueFamilyProperties2[queue_family_property_count];
	physical_device.getQueueFamilyProperties2(&queue_family_property_count, queue_family_properties);

	vk::Bool32 surface_support;
	for (uint32_t i = 0; i < queue_family_property_count; ++i) {
		result = physical_device.getSurfaceSupportKHR(i, m_surface, &surface_support); // Check is GPU supports present family with given surface
		assert(result == vk::Result::eSuccess);

		if (queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;
		
		if (queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer && !(queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics))
			indices.transferFamily = i;

		if (queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eCompute && !(queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics))
			indices.computeFamily = i;

		if (surface_support)
			indices.presentFamily = i;
		
		[[unlikely]] if (indices.isComplete()) {
			delete[] queue_family_properties;
			return indices;
		}
	}

	if (indices.graphicsFamily.has_value() && !(indices.transferFamily.has_value()))
		indices.transferFamily = indices.graphicsFamily;
	
	if (indices.graphicsFamily.has_value() && !(indices.computeFamily.has_value()))
		indices.computeFamily = indices.graphicsFamily;

	delete[] queue_family_properties;
	return indices;
}

bool Application::physicalDeviceSupportsExtensions(const vk::PhysicalDevice &physical_device) noexcept
{
	vk::Result result;
	vk::ExtensionProperties *extension_properties;
	uint32_t extension_property_count;

	result = physical_device.enumerateDeviceExtensionProperties(nullptr, &extension_property_count, nullptr);
	assert(result == vk::Result::eSuccess);
	
	extension_properties = new vk::ExtensionProperties[extension_property_count];
	result = physical_device.enumerateDeviceExtensionProperties(nullptr, &extension_property_count, extension_properties);
	assert(result == vk::Result::eSuccess);
	
	std::set<std::string_view> required_extensions(&m_deviceExtensions[0], &m_deviceExtensions[NUM_DEVICE_EXTENSIONS - 1]);

	for (uint32_t i = 0; i < extension_property_count; ++i)
		required_extensions.erase(static_cast<std::string_view>(extension_properties[i].extensionName));
	
	delete[] extension_properties;
	return required_extensions.empty();
}

SwapChainSupportDetails Application::querySwapChainSupport(const vk::PhysicalDevice &physical_device) noexcept
{
	vk::Result result;
	SwapChainSupportDetails support_details;

	const vk::PhysicalDeviceSurfaceInfo2KHR surface_info{
		m_surface // Surface
	};

	result = physical_device.getSurfaceCapabilities2KHR(&surface_info, &support_details.capabilities);
	assert(result == vk::Result::eSuccess);

	result = physical_device.getSurfaceFormats2KHR(&surface_info, &support_details.formatCount, nullptr);
	assert(result == vk::Result::eSuccess);

	support_details.formats = new vk::SurfaceFormat2KHR[support_details.formatCount];
	result = physical_device.getSurfaceFormats2KHR(&surface_info, &support_details.formatCount, support_details.formats);
	assert(result == vk::Result::eSuccess);

	result = physical_device.getSurfacePresentModesKHR(m_surface, &support_details.presentModeCount, nullptr);
	assert(result == vk::Result::eSuccess);

	support_details.presentModes = new vk::PresentModeKHR[support_details.presentModeCount];
	result = physical_device.getSurfacePresentModesKHR(m_surface, &support_details.presentModeCount, support_details.presentModes);
	assert(result == vk::Result::eSuccess);

	return support_details;
}

bool Application::swapChainAdequate(const SwapChainSupportDetails &swap_chain) noexcept
{
	return swap_chain.formatCount > 0 && swap_chain.presentModeCount > 0;
}

constexpr vk::SurfaceFormat2KHR Application::chooseSwapSurfaceFormat(const vk::SurfaceFormat2KHR * const formats, const uint32_t count) noexcept
{
	for (uint32_t i = 0; i < count; ++i)
		if (formats[i].surfaceFormat.format == vk::Format::eB8G8R8Srgb && // Formats pixels as: B, G, R, A; 8 bits each
			formats[i].surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) // Is Srbg format supported
			return formats[i];
	
	return formats[0]; // Could rank formats
}

constexpr vk::PresentModeKHR Application::chooseSwapPresentMode(const vk::PresentModeKHR * const present_modes, const uint32_t count) noexcept
{
	for (uint32_t i = 0; i < count; ++i)
		if (present_modes[i] == vk::PresentModeKHR::eMailbox) // Triple Buffering (great performance, no tearing, high energy usage)
			return vk::PresentModeKHR::eMailbox;

	for (uint32_t i = 0; i < count; ++i)
		if (present_modes[i] == vk::PresentModeKHR::eFifo) // Standard double buffering (good performance, no tearing, average energy usage)
			return vk::PresentModeKHR::eFifo;

	// Guaranteed to be available
	return vk::PresentModeKHR::eImmediate; // Images are immediately transferred to the screen (great preformanace, possible tearing, average energy usage)
}

vk::Extent2D Application::chooseSwapExtent(const vk::SurfaceCapabilities2KHR &capabilities) noexcept
{
	if (capabilities.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.surfaceCapabilities.currentExtent; // Width and height are already set
	
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	// The max and min functions are used here to clamp 
	// the value of WIDTH and HEIGHT between the allowed 
	// minimum and maximum extents that are supported by the implementation
	return {
		std::clamp(static_cast<uint32_t>(width),  capabilities.surfaceCapabilities.minImageExtent.width,  capabilities.surfaceCapabilities.maxImageExtent.width),
		std::clamp(static_cast<uint32_t>(height), capabilities.surfaceCapabilities.minImageExtent.height, capabilities.surfaceCapabilities.maxImageExtent.height)
	};
}

std::vector<char> Application::readFile(const std::string_view &&filename) noexcept
{
	std::ifstream file{
		filename,           // Open the given file
		std::ios::ate |     // Start reading at the end of the file
		std::ios::binary    // Read the file as binary file (avoid text transformations)
	};

	assert(file.is_open() && "ASSERT: Failed to open file!");

	const std::streampos file_size = file.tellg();
	std::vector<char> buffer(static_cast<size_t>(file_size));

	file.seekg(0);
	file.read(buffer.data(), file_size);

	return buffer;
}

vk::ShaderModule Application::createShaderModule(const std::vector<char> &code) noexcept
{
	vk::Result result;

	const vk::ShaderModuleCreateInfo shader_module_create_info{
		{}, // Flags
		code.size(), // Size of the code (in bytes)
		reinterpret_cast<const uint32_t *>(code.data()) // The buffer of data
	};

	vk::ShaderModule shader_module;
	result = m_logicalDevice.createShaderModule(&shader_module_create_info, &m_allocationCallbacks, &shader_module);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Shader Module; Pointer: " << shader_module);

	return shader_module;
}

constexpr uint32_t Application::findMemoryType(const uint32_t type_filter, const vk::MemoryPropertyFlags required_properties) noexcept
{
	for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryProperties.memoryTypeCount; ++i) {
		if (type_filter & (1 << i) && (m_physicalDeviceMemoryProperties.memoryProperties.memoryTypes[i].propertyFlags & required_properties) == required_properties) {
			return i;
		}
	}
	
	assert(false && "VULKAN ASSERT: failed to find suitable memory type!");
#ifdef NDEBUG
	return std::numeric_limits<uint32_t>::max();
#endif
}

vk::Format Application::findSupportedFormat(const vk::Format * const candidate_formats, const uint32_t count, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) noexcept
{
	for (uint32_t i = 0; i < count; ++i) {
		vk::FormatProperties2 format_properties;
		m_physicalDevice.getFormatProperties2(candidate_formats[i], &format_properties);

		if (tiling == vk::ImageTiling::eLinear && (format_properties.formatProperties.linearTilingFeatures & features) == features)
			return candidate_formats[i];
		
		if (tiling == vk::ImageTiling::eOptimal && (format_properties.formatProperties.optimalTilingFeatures & features) == features)
			return candidate_formats[i];
	}

	assert(false && "VULKAN ASSERT: Failed to find supported format!");
#ifdef NDEBUG
	return {};
#endif
}

vk::Format Application::findDepthFormat() noexcept
{
	constexpr vk::Format requested_formats[DEPTH_FORMAT_COUNT] = {
		vk::Format::eD32Sfloat,
		vk::Format::eD32SfloatS8Uint,
		vk::Format::eD24UnormS8Uint
	};

	return findSupportedFormat(
		requested_formats,
		DEPTH_FORMAT_COUNT,
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
}

constexpr bool Application::hasStencilComponent(const vk::Format format) noexcept
{
	return format != vk::Format::eD32Sfloat;
}

constexpr vk::SampleCountFlagBits Application::getMaxUsableSampleCount() noexcept
{
	const vk::SampleCountFlags sample_counts =  m_physicalDeviceProperties.properties.limits.framebufferColorSampleCounts & 
												m_physicalDeviceProperties.properties.limits.framebufferDepthSampleCounts;
	
	if (sample_counts & vk::SampleCountFlagBits::e64)
		return vk::SampleCountFlagBits::e64;
	if (sample_counts & vk::SampleCountFlagBits::e32)
		return vk::SampleCountFlagBits::e32;
	if (sample_counts & vk::SampleCountFlagBits::e16)
		return vk::SampleCountFlagBits::e16;
	if (sample_counts & vk::SampleCountFlagBits::e8)
		return vk::SampleCountFlagBits::e8;
	if (sample_counts & vk::SampleCountFlagBits::e4)
		return vk::SampleCountFlagBits::e4;
	if (sample_counts & vk::SampleCountFlagBits::e2)
		return vk::SampleCountFlagBits::e2;

	return vk::SampleCountFlagBits::e1;
}

vk::ImageView Application::createImageView(const vk::Image image, const vk::Format format, const vk::ImageAspectFlags aspect_flags, const uint32_t mip_levels) noexcept
{
	vk::Result result;

	// Allows you to swizzle the color channels around (Identity for default mapping)
	constexpr vk::ComponentMapping image_view_color_components{
		vk::ComponentSwizzle::eIdentity,
		vk::ComponentSwizzle::eIdentity,
		vk::ComponentSwizzle::eIdentity,
		vk::ComponentSwizzle::eIdentity
	};

	// Describes what the image's purpose is and which parts of the image should be accessed
	const vk::ImageSubresourceRange image_view_subresource_range{
		aspect_flags, // Aspect mask (using color targets)
		0, // Base mip level 
		mip_levels, // Mip level amount
		0, // Base array layer (first array layer)
		1 // Array layer amount (one array layer)
	};

	// Information for creating the image view
	const vk::ImageViewCreateInfo image_view_create_info{
		{}, // Flags
		image, // Image
		vk::ImageViewType::e2D, // View type (treat images as 2D textures)
		format, // Format: chosen image format
		image_view_color_components, // Color components
		image_view_subresource_range // Subresource range
	};

	// Create the image view
	vk::ImageView image_view;
	result = m_logicalDevice.createImageView(&image_view_create_info, &m_allocationCallbacks, &image_view);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Image View; Pointer: " << image_view);

	return image_view;
}

void Application::run() noexcept
{
	vk::Result result;

	// Set up FPS counter
	m_previousTimeFPS = std::chrono::high_resolution_clock::now();
	m_frameCountFPS = 0;

	// Main game loop
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
		drawFrame();
		printFPS();
	}

	result = m_logicalDevice.waitIdle();
	assert(result == vk::Result::eSuccess);
}

void Application::printFPS() noexcept
{
	const std::chrono::steady_clock::time_point current_time = std::chrono::high_resolution_clock::now();
	const float duration = std::chrono::duration<float>(current_time - m_previousTimeFPS).count();
	++m_frameCountFPS;

	[[unlikely]] if (duration >= 1.0f) {
		std::cout << std::setprecision(4) << "Application Average: " << 1000.0f / m_frameCountFPS << "ms\t(" << m_frameCountFPS << " FPS)\n";

		m_frameCountFPS = 0;
		m_previousTimeFPS = current_time;
	}
}

void Application::drawFrame() noexcept
{
	vk::Result result;

	// Wait for fence to be signaled
	result = m_logicalDevice.waitForFences(
		1, // Fence count
		&m_inFlightFences[m_currentFrame], // Fences: which fences to wait on
		VK_TRUE, // Wait all: whether to wait for all the fences or just one
		std::numeric_limits<uint64_t>::max() // Timeout: timeout in nanoseconds (uint64 max disables the timeout)
	);

	// Check result
	assert(result == vk::Result::eSuccess);

	// Index refers to the vk::Image in the swap chain images array
	uint32_t image_index;

	// Get the next image in the swap chain (using old method because newer method considers eErrorOutOfDateKHR as an exception)
	result = m_logicalDevice.acquireNextImageKHR(
		m_swapChain,
		std::numeric_limits<uint64_t>::max(),
		m_imageAvailableSemaphores[m_currentFrame],
		VK_NULL_HANDLE,
		&image_index
	);

	assert(result == vk::Result::eSuccess || result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR);

	// Only recreate swap chain if out of date; because we already have the next image
	[[unlikely]] if (result == vk::Result::eErrorOutOfDateKHR) {
		m_framebufferResized = false;
		recreateSwapChain();
		return;
	}

	// Check if a previous frame is using this image; wait on it's fence
	if (static_cast<bool>(m_imagesInFlight[image_index])) {
		result = m_logicalDevice.waitForFences(1, &m_imagesInFlight[image_index], VK_TRUE, std::numeric_limits<uint64_t>::max());
		assert(result == vk::Result::eSuccess);
	}
	
	// Mark image as now being in use by this frame
	m_imagesInFlight[image_index] = m_inFlightFences[m_currentFrame];

	// Update uniform buffer
	updateUniformBuffer(image_index);

	const vk::Semaphore wait_semaphores[DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT] = {
		m_imageAvailableSemaphores[m_currentFrame]
	};

	constexpr vk::PipelineStageFlags wait_stages[DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT] = {
		vk::PipelineStageFlagBits::eColorAttachmentOutput
	};

	const vk::Semaphore signal_semaphores[DRAW_FRAME_SUBMIT_INFO_SIGNAL_SEMAPHORE_COUNT] = {
		m_renderFinishedSemaphores[m_currentFrame]
	};

	// Information for submitting to the graphics queue
	const vk::SubmitInfo submit_info{
		DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT, // Wait semaphore count
		wait_semaphores, // Wait semaphores: which semaphores to wait on before execution begins
		wait_stages, // Wait dst stage mask: which stages of the graphics pipeline to wait on before execution; corresponds to the wait_semaphores
		1, // Command buffer count
		&m_commandBuffers[image_index], // Command buffers: which command buffers to actually submit for execution
		DRAW_FRAME_SUBMIT_INFO_SIGNAL_SEMAPHORE_COUNT, // Signal semaphore count
		signal_semaphores // Signal semaphores: which semaphores to signal once the command buffer/s have finished execution
	};

	// Set fence back to unsignaled state
	result = m_logicalDevice.resetFences(1, &m_inFlightFences[m_currentFrame]);
	assert(result == vk::Result::eSuccess);

	// Submit the command buffer to the graphics queue
	result = m_queueHandles.m_graphicsQueue.submit(1, &submit_info, m_inFlightFences[m_currentFrame]);
	assert(result == vk::Result::eSuccess);

	// Swap chains
	const vk::SwapchainKHR swap_chains[PRESENT_INFO_SWAP_CHAIN_COUNT] = {
		m_swapChain
	};

	// Presentation configurations
	vk::PresentInfoKHR present_info{
		DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT, // Wait semaphore count
		signal_semaphores, // Wait semaphores (using render_finished not image_available)
		PRESENT_INFO_SWAP_CHAIN_COUNT, // Swap chain count
		swap_chains, // Swap chains
		&image_index, // Image indices
		nullptr // Results: specify an array of vk::Result values to check for every individual swap chain if presentation was successful (not necessary if only using 1 swap chain)
	};

	static constexpr vk::PresentTimeGOOGLE present_times[PRESENT_INFO_SWAP_CHAIN_COUNT] = {
		{
			0, // Present ID
			0 // Desired present time
		}
	};

	constexpr vk::PresentTimesInfoGOOGLE present_times_info{
		PRESENT_INFO_SWAP_CHAIN_COUNT, // Swap chain count
		present_times // Times
	};

	present_info.pNext = &present_times_info;

	// Request the presentation of the image
	result = m_queueHandles.m_presentQueue.presentKHR(&present_info);
	assert(result == vk::Result::eSuccess || result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR);

	// Recreate swap chain if it is
	if (result == vk::Result::eErrorOutOfDateKHR || // Out of date
		result == vk::Result::eSuboptimalKHR || // Of if the swap chain is not optimal
		m_framebufferResized) { // Or if the framebuffer was resized
		m_framebufferResized = false;
		recreateSwapChain();
	}

	// Wait for image to be submitted
	// m_presentQueue.waitIdle(); // Using fences instead

	// Advance to the next frame
	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Application::destroySwapChain() noexcept
{
	m_logicalDevice.freeCommandBuffers(m_graphicsCommandPool, m_swapChainImageCount, m_commandBuffers); // Free command buffers instead of destroying command pool
	m_logicalDevice.destroyDescriptorPool(m_descriptorPool, &m_allocationCallbacks);

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		m_logicalDevice.destroyBuffer(m_uniformBuffers[i], &m_allocationCallbacks);
		m_logicalDevice.freeMemory(m_uniformBuffersMemory[i], &m_allocationCallbacks);
	}

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i)
		m_logicalDevice.destroyFramebuffer(m_swapChainFramebuffers[i], &m_allocationCallbacks);

	m_logicalDevice.destroyImageView(m_depthImageView, &m_allocationCallbacks);
	m_logicalDevice.destroyImage(m_depthImage, &m_allocationCallbacks);
	m_logicalDevice.freeMemory(m_depthImageMemory, &m_allocationCallbacks);

#if ENABLE_MULTISAMPLING
	m_logicalDevice.destroyImageView(m_colorImageView, &m_allocationCallbacks);
	m_logicalDevice.destroyImage(m_colorImage, &m_allocationCallbacks);
	m_logicalDevice.freeMemory(m_colorImageMemory, &m_allocationCallbacks);
#endif

#if !(ENABLE_PIPELINE_DYNAMIC_STATE)
	m_logicalDevice.destroyPipeline(m_childGraphicsPipeline, &m_allocationCallbacks);
	m_logicalDevice.destroyPipeline(m_parentGraphicsPipeline, &m_allocationCallbacks);
	m_logicalDevice.destroyPipelineLayout(m_parentGraphicsPipelineInfo.layout, &m_allocationCallbacks);
	m_logicalDevice.destroyRenderPass(m_parentGraphicsPipelineInfo.renderPass, &m_allocationCallbacks);

	m_logicalDevice.destroyShaderModule(m_parentGraphicsPipelineInfo.vertexShaderModule, &m_allocationCallbacks);
	m_logicalDevice.destroyShaderModule(m_parentGraphicsPipelineInfo.fragmentShaderModule, &m_allocationCallbacks);
	m_parentGraphicsPipelineInfo.created = false;
#endif

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i)
		m_logicalDevice.destroyImageView(m_swapChainImageViews[i], &m_allocationCallbacks);

	m_logicalDevice.destroySwapchainKHR(m_swapChain, &m_allocationCallbacks);
}

void Application::recreateSwapChain() noexcept
{
	vk::Result result;

	// Wait for window to be in the foreground
	int width, height;
	do {
		glfwGetFramebufferSize(m_window, &width, &height);
		glfwWaitEvents();
	}
	while (width == 0 && height == 0);

	// Wait for gpu to finish
	result = m_logicalDevice.waitIdle();
	assert(result == vk::Result::eSuccess);

	// Destroy the previous swap chain
	destroySwapChain();

	// Create the new swap chain
	createSwapChain();

	// Call creation functions that depend on the swap chain
	createSwapChainImageViews(); // Based directly on the swap chain images
#if !(ENABLE_PIPELINE_DYNAMIC_STATE)
	createRenderPass(); // Depends on the format of the swap chain images
	createGraphicsPipeline(); // Viewport and scissor rectangle size is specified
#endif
#if ENABLE_MULTISAMPLING
	createColorResources(); // Depends on the swap chain extent
#endif
	createDepthResources(); // Depends on the swap chain extent
	createFrameBuffers(); // Directly depends on the swap chain images
	createUniformBuffers(); // Directly depends on the swap chain images
	createDescriptorPool(); // Directly depends on the swap chain images
	createDescriptorSets(); // Directly depends on the swap chain images
	createCommandBuffers(); // Directly depends on the swap chain images
}

void Application::writePipelineCache() noexcept
{
	vk::Result result;

	size_t cache_size;
	char *cache_data;

	result = m_logicalDevice.getPipelineCacheData(m_graphicsPipelineCache, &cache_size, nullptr);
	assert(result == vk::Result::eSuccess);

	cache_data = new char[cache_size];

	result = m_logicalDevice.getPipelineCacheData(m_graphicsPipelineCache, &cache_size, cache_data);
	assert(result == vk::Result::eSuccess);

	std::ofstream cache_file{
		"pipeline_cache_data.bin",
		std::ios::binary
	};

	if (cache_file.is_open())
		cache_file.write(cache_data, static_cast<std::streamsize>(cache_size));

	delete[] cache_data;
}

template <class T>
void Application::createAndFillBuffer(const T * const buffer_data, const size_t count, const vk::BufferUsageFlags usage, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) noexcept
{
	vk::Result result;

	// Create the staging buffer handles (vertex buffer in CPU visible memory)
	vk::Buffer staging_buffer;
	vk::DeviceMemory staging_buffer_memory;

	// Size of buffer memory
	const vk::DeviceSize buffer_size = sizeof(buffer_data[0]) * count;

	// Create the staging buffer
	createBuffer(
		buffer_size, // Size
		vk::BufferUsageFlagBits::eTransferSrc, // Usage (transfer source)
		vk::MemoryPropertyFlagBits::eHostVisible | // Properties (Visible to CPU)
		vk::MemoryPropertyFlagBits::eHostCoherent,// (Writes to buffer are visible immediately)
		staging_buffer, // Buffer
		staging_buffer_memory // Buffer memory
	);

	// Map the staging buffer into CPU accessable memory
	void *mapped_data;
	result = m_logicalDevice.mapMemory(
		staging_buffer_memory, // Memory
		0, // Offset
		VK_WHOLE_SIZE, // Size
		{}, // Flags
		&mapped_data
	);
	
	assert(result == vk::Result::eSuccess);

	// Copy the staging data into the mapped memory
	memcpy(mapped_data, buffer_data, static_cast<size_t>(buffer_size));

	// Unmap the data
	m_logicalDevice.unmapMemory(staging_buffer_memory);

	// Create the requested buffer
	createBuffer(
		buffer_size,
		usage | vk::BufferUsageFlagBits::eTransferDst,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		buffer,
		buffer_memory
	);

	// Copy the data from the staging buffer to the GPU-local buffer
	copyBuffer(staging_buffer, buffer, buffer_size);

	// Memory cleanup
	m_logicalDevice.destroyBuffer(staging_buffer, &m_allocationCallbacks);
	m_logicalDevice.freeMemory(staging_buffer_memory, &m_allocationCallbacks);
}

void Application::createBuffer(const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) noexcept
{
	vk::Result result;

	// Possibly needed queue families
	const uint32_t buffer_queue_family_indices[2] = {
		m_queueFamilyIndices.graphicsFamily.value(),
		m_queueFamilyIndices.transferFamily.value()
	};

	// Information for the buffer
	vk::BufferCreateInfo buffer_create_info{
		{}, // Flags
		size, // Size: the size of the buffer in bytes
		usage, // Usage: which purposes the data in the buffer is going to be used for
		vk::SharingMode::eExclusive, // Sharing mode: whether the buffer is used by one queue family at a time or shared between multiple concurrently
		1, // Queue family index count
		buffer_queue_family_indices // Queue family indices
	};

	// Check if queue families are different
	if (m_queueFamilyIndices.graphicsFamily.value() != m_queueFamilyIndices.transferFamily.value()) {
		buffer_create_info.sharingMode = vk::SharingMode::eConcurrent;
		buffer_create_info.queueFamilyIndexCount = 2;
	}

	// Creating the buffer
	result = m_logicalDevice.createBuffer(&buffer_create_info, &m_allocationCallbacks, &buffer);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Buffer; Pointer: " << buffer);

	// Get the memory requirements for the buffer
	const vk::BufferMemoryRequirementsInfo2 memory_requirements_info{
		buffer
	};

	vk::MemoryRequirements2 buffer_memory_requirements;
	m_logicalDevice.getBufferMemoryRequirements2(&memory_requirements_info, &buffer_memory_requirements);

	// Memory type index
	const uint32_t buffer_memory_type_index = findMemoryType(buffer_memory_requirements.memoryRequirements.memoryTypeBits, properties);

	// Information for allocating memory
	const vk::MemoryAllocateInfo buffer_memory_allocate_info{
		buffer_memory_requirements.memoryRequirements.size, // Allocation size
		buffer_memory_type_index // Memory type index
	};

	// Allocate memory for the buffer
	result = m_logicalDevice.allocateMemory(&buffer_memory_allocate_info, &m_allocationCallbacks, &buffer_memory);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Allocated Vulkan Memory; Allocation Size: " << buffer_memory_allocate_info.allocationSize << ", Memory Type Index: " << buffer_memory_allocate_info.memoryTypeIndex << ", Pointer: " << buffer_memory);

	// Bind the allocated memory with the buffer
	const vk::BindBufferMemoryInfo bind_buffer_memory_info{
		buffer, // Buffer
		buffer_memory, // Memory
		0 // Memory offset
	};

	result = m_logicalDevice.bindBufferMemory2(1, &bind_buffer_memory_info); // The 3rd parameter is the offset within the region of memory
	assert(result == vk::Result::eSuccess);
}

void Application::copyBuffer(const vk::Buffer &src_buffer, const vk::Buffer &dst_buffer, const vk::DeviceSize size) noexcept
{
	// Data copying information
	const vk::BufferCopy buffer_copy_region{
		0, // Src offset
		0, // Dst offset
		size // Size
	};

	// Begin the command
	const vk::CommandBuffer command_buffer = beginSingleTimeCommand(false);

	// Copy the data
	command_buffer.copyBuffer(
		src_buffer, // Src buffer
		dst_buffer, // Dst buffer
		1, // Region count
		&buffer_copy_region // Regions 
	);

	// End the command
	endSingleTimeCommand(command_buffer, false);
}

void Application::updateUniformBuffer(const uint32_t current_image) noexcept
{
	vk::Result result;

	// Deltatime
	static std::chrono::steady_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	const std::chrono::steady_clock::time_point current_time = std::chrono::high_resolution_clock::now();
	const float duration = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

	// Uniform buffer object
	UniformBufferObject UBO{
		.model = glm::rotate(glm::mat4(1.0f), duration * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // Rotate around an axis at 90 degrees/second
		.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // Look at the geometry from above at a 45 degree angle
		.proj = glm::perspective(glm::radians(40.0f), m_swapChainExtent.width / static_cast<float>(m_swapChainExtent.height), 0.1f, 10.0f) // Perspective projection with a 40 degree vertical field-of-view
	};

	// Flip the sign on the scaling factor of the Y axis to render geometry right way up
	UBO.proj[1][1] *= -1;

	// Update the uniform buffer
	void *mapped_data; 
	result = m_logicalDevice.mapMemory(
		m_uniformBuffersMemory[current_image], // Memory
		0, // Offset
		VK_WHOLE_SIZE, // Size
		{}, // Flags
		&mapped_data // Pointer: to mapped memory
	);

	assert(result == vk::Result::eSuccess);

	// Copy the uniform data into the mapped memory
	memcpy(mapped_data, &UBO, sizeof(UBO));

	// Unmap the data
	m_logicalDevice.unmapMemory(m_uniformBuffersMemory[current_image]);
}

void Application::createImage(const uint32_t width, const uint32_t height, const uint32_t mip_levels, const vk::SampleCountFlagBits sample_count, const vk::Format format, const vk::ImageTiling tiling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &image_memory) noexcept
{
	vk::Result result;

	// Image extent
	const vk::Extent3D image_extent{
		width, // Width
		height, // Height
		1 // Depth
	};

	// Possibly needed queue families
	const uint32_t image_queue_family_indices[2] = {
		m_queueFamilyIndices.graphicsFamily.value(),
		m_queueFamilyIndices.transferFamily.value()
	};

	// Information for creating the image
	vk::ImageCreateInfo image_create_info{
		{}, // Flags
		vk::ImageType::e2D, // Image type: what coordinate system the texels are going to be addressed in
		format, // Format: the format of data
		image_extent, // Extent: the dimensions of the image; amount of texels on each axis
		mip_levels, // Mip levels
		1, // Array layers
		sample_count, // Samples: for multisampling
		tiling, // Tiling: the tiling of the texels in theimage
		usage, // Usage: similar to buffer usage
		vk::SharingMode::eExclusive, // Sharing mode
		1, // Queue family index count
		image_queue_family_indices, // Queue family indices
		vk::ImageLayout::eUndefined // Initial layout
	};

	// Using the right amount of queue families
	if (m_queueFamilyIndices.graphicsFamily.value() != m_queueFamilyIndices.transferFamily.value()) {
		image_create_info.sharingMode = vk::SharingMode::eConcurrent;
		image_create_info.queueFamilyIndexCount = 2;
	}

	// Creating the image
	result = m_logicalDevice.createImage(&image_create_info, &m_allocationCallbacks, &image);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Created Vulkan Image; Pointer: " << image);

	// Memory requirements for the GPU being used
	const vk::ImageMemoryRequirementsInfo2 memory_requirements_info{
		image
	};

	vk::MemoryDedicatedRequirements dedicated_requirements;

	vk::MemoryRequirements2 image_memory_requirements;
	image_memory_requirements.pNext = &dedicated_requirements;

	m_logicalDevice.getImageMemoryRequirements2(&memory_requirements_info, &image_memory_requirements);

	if (dedicated_requirements.prefersDedicatedAllocation || dedicated_requirements.requiresDedicatedAllocation) {
		const vk::MemoryDedicatedAllocateInfo dedicated_allocate_info{
			image,
			VK_NULL_HANDLE
		};

		vk::MemoryAllocateInfo memory_allocate_info{
			image_memory_requirements.memoryRequirements.size,
			findMemoryType(image_memory_requirements.memoryRequirements.memoryTypeBits, properties)
		};

		memory_allocate_info.pNext = &dedicated_allocate_info;

		result = m_logicalDevice.allocateMemory(&memory_allocate_info, &m_allocationCallbacks, &image_memory);
		assert(result == vk::Result::eSuccess);
		DEBUG_LOG("Allocated Vulkan Memory; Allocation Size: " << memory_allocate_info.allocationSize << ", Memory Type Index: " << memory_allocate_info.memoryTypeIndex << ", Pointer: " << image_memory);

		const vk::BindImageMemoryInfo bind_image_memory_info{
			image, 
			image_memory,
			0
		};

		result = m_logicalDevice.bindImageMemory2(1, &bind_image_memory_info);
		assert(result == vk::Result::eSuccess);
		return;
	}

	// Information for allocating memory
	const vk::MemoryAllocateInfo memory_allocate_info{
		image_memory_requirements.memoryRequirements.size,
		findMemoryType(image_memory_requirements.memoryRequirements.memoryTypeBits, properties)
	};

	// Allocate memory for the texture buffer
	result = m_logicalDevice.allocateMemory(&memory_allocate_info, &m_allocationCallbacks, &image_memory);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Allocated Vulkan Memory; Allocation Size: " << memory_allocate_info.allocationSize << ", Memory Type Index: " << memory_allocate_info.memoryTypeIndex << ", Pointer: " << image_memory);

	// Bind the memory to the buffer handle
	const vk::BindImageMemoryInfo bind_image_memory_info{
		image, // Image
		image_memory, // Memory
		0 // Memory offset
	};

	result = m_logicalDevice.bindImageMemory2(1, &bind_image_memory_info);
	assert(result == vk::Result::eSuccess);
}

void Application::transitionImageLayout(const vk::Image image, const vk::Format /* format */, const vk::ImageAspectFlags image_aspect_flags, const vk::ImageLayout old_layout, const vk::ImageLayout new_layout, const uint32_t mip_levels) noexcept
{
	const vk::ImageSubresourceRange subresource_range{
		image_aspect_flags, // Aspect mask
		0, // Base mip level
		mip_levels, // Level count
		0, // Base array layer
		1 // Layer count
	};

	vk::ImageMemoryBarrier image_memory_barrier{
		{}, // Src access mask
		{}, // Dst access mask
		old_layout, // Old layout
		new_layout, // New layout
		VK_QUEUE_FAMILY_IGNORED, // Src queue family index
		VK_QUEUE_FAMILY_IGNORED, // Dst queue family index
		image, // Image
		subresource_range // Subresource range
	};

	// pipeline stages to wait on
	vk::PipelineStageFlags source_stage;
	vk::PipelineStageFlags destination_stage;

	// Set to correct transfer
	if (old_layout == vk::ImageLayout::eUndefined && new_layout == vk::ImageLayout::eTransferDstOptimal) {
		// image_memory_barrier.srcAccessMask = {}; // Already done in constructor
		image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		source_stage = vk::PipelineStageFlagBits::eTopOfPipe; // Writes don't have to wait on anything
		destination_stage = vk::PipelineStageFlagBits::eTransfer; // Not a real pipeline stage; pseudo-stage
	} 
	else if (old_layout == vk::ImageLayout::eTransferDstOptimal && new_layout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		source_stage = vk::PipelineStageFlagBits::eTransfer;
		destination_stage = vk::PipelineStageFlagBits::eFragmentShader; // Image will be read be the fragment shader
	} 
	else if (old_layout == vk::ImageLayout::eUndefined && new_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
		destination_stage = vk::PipelineStageFlagBits::eEarlyFragmentTests; // Image will be read be the fragment shader
	} 
	else [[unlikely]] {
		assert(false && "ASSERT: Unsupported image layout transition!");
	}

	const vk::CommandBuffer command_buffer = beginSingleTimeCommand(false);

	// Transfer the image
	command_buffer.pipelineBarrier(
		source_stage, // Src stage mask
		destination_stage, // Dst stage mask
		{}, // Dependency flags
		0, // Memory barrier count
		nullptr, // Memory barriers
		0, // Buffer memory count
		nullptr, // Buffer memory barriers
		1, // Image memory barrier count
		&image_memory_barrier // Image memory barriers
	);

	endSingleTimeCommand(command_buffer, false);
}

void Application::copyBufferToImage(const vk::Buffer src_buffer, const vk::Image dst_image, const uint32_t width, const uint32_t height) noexcept
{
	constexpr vk::ImageSubresourceLayers image_subresource{
		vk::ImageAspectFlagBits::eColor, // Aspect mask
		0, // Mip level
		0, // Base array layer
		1 // Layer count
	};

	constexpr vk::Offset3D image_offset{
		0, // X
		0, // Y
		0 // Z
	};

	const vk::Extent3D image_extent{
		width, // Width
		height, // Height
		1 // Depth
	};

	const vk::BufferImageCopy image_copy_region{
		0, // Buffer offset: byte offset in the buffer at which the pixel values start
		0, // Buffer row length: how the pixels are laid out in memory
		0, // Buffer image height: how the pixels are laid out in memory
		image_subresource, // Image subresource
		image_offset, // Image offset
		image_extent // Image extent
	};

	const vk::CommandBuffer command_buffer = beginSingleTimeCommand(false);

	command_buffer.copyBufferToImage(
		src_buffer, // Src buffer
		dst_image, // Dst image
		vk::ImageLayout::eTransferDstOptimal, // Dst image layout: which layout the image is currently using
		1, // Region count
		&image_copy_region // Regions
	);

	endSingleTimeCommand(command_buffer, false);
}

#if ENABLE_MIPMAPS

void Application::generateMipmaps(const vk::Image image, const vk::Format image_format, int tex_width, int tex_height, const uint32_t mip_levels) noexcept
{
	// Check if image format supports linear blitting
	vk::FormatProperties2 format_properties;
	m_physicalDevice.getFormatProperties2(image_format, &format_properties);
	assert(format_properties.formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear && "VULKAN ASSERT: Texture image format does not support linear blitting!");

	vk::ImageMemoryBarrier image_memory_barrier{
		{}, // Src access mask
		{}, // Dst access mask
		{}, // Old layout
		{}, // New layout
		VK_QUEUE_FAMILY_IGNORED, // Src queue family index
		VK_QUEUE_FAMILY_IGNORED, // Dst queue family index
		image, // Image
		{
			vk::ImageAspectFlagBits::eColor,
			{},
			1,
			0,
			1
		} // Subresource range
	};

	const vk::CommandBuffer command_buffer = beginSingleTimeCommand(true);

	for (uint32_t i = 1; i < mip_levels; ++i) {
		image_memory_barrier.subresourceRange.baseMipLevel = i - 1;
		image_memory_barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		image_memory_barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
		image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

		command_buffer.pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer, // Src stage mask
			vk::PipelineStageFlagBits::eTransfer, // Dst stage mask
			{}, // Dependency flags
			0, // Memory barrier count
			nullptr, // Memory barriers
			0, // Buffer memory count
			nullptr, // Buffer memory barriers
			1, // Image memory barrier count
			&image_memory_barrier // Image memory barriers
		);

		const vk::ImageSubresourceLayers src_image_subresource{
			vk::ImageAspectFlagBits::eColor, // Aspect mask
			i - 1, // Mip level
			0, // Base array layer
			1 // Layer count
		};

		const std::array<vk::Offset3D, 2> src_offsets{
			vk::Offset3D{ 0, 0, 0 },
			vk::Offset3D{ static_cast<int32_t>(tex_width), static_cast<int32_t>(tex_height), 1 }
		};

		const vk::ImageSubresourceLayers dst_image_subresource{
			vk::ImageAspectFlagBits::eColor, // Aspect mask
			i, // Mip level
			0, // Base array layer
			1 // Layer count
		};

		const std::array<vk::Offset3D, 2> dst_offsets{
			vk::Offset3D{ 0, 0, 0 },
			vk::Offset3D{ tex_width > 1 ? tex_width / 2 : 1, tex_height > 1 ? tex_height / 2 : 1, 1 }
		};

		const vk::ImageBlit image_blit{
			std::move(src_image_subresource), // Src subresource
			std::move(src_offsets), // Src offsets
			std::move(dst_image_subresource), // Dst subresource
			std::move(dst_offsets) // Dst offsets
		};

		command_buffer.blitImage(
			image, // Src image
			vk::ImageLayout::eTransferSrcOptimal, // Src image layout
			image, // Dst image
			vk::ImageLayout::eTransferDstOptimal, // Dst image layout
			1, // Region count
			&image_blit, // Regions
			vk::Filter::eLinear // Filter
		);

		image_memory_barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
		image_memory_barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
		image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		command_buffer.pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer,
			vk::PipelineStageFlagBits::eFragmentShader,
			{},
			0, 
			nullptr,
			0, 
			nullptr,
			1, 
			&image_memory_barrier 
		);

		if (tex_width > 1) 
			tex_width /= 2;
		if (tex_height > 1) 
			tex_height /= 2;
	}

	// Transition the last mip level
	image_memory_barrier.subresourceRange.baseMipLevel = mip_levels - 1;
	image_memory_barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	image_memory_barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	image_memory_barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

	command_buffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eFragmentShader,
		{},
		0, 
		nullptr,
		0, 
		nullptr,
		1, 
		&image_memory_barrier 
	);

	endSingleTimeCommand(command_buffer, true);
}

#endif // ENABLE_MIPMAPS

vk::CommandBuffer Application::beginSingleTimeCommand(const bool need_graphics) noexcept
{
	vk::Result result;

	const vk::CommandBufferAllocateInfo command_buffer_allocate_info{
		need_graphics ? m_graphicsCommandPool : m_transferCommandPool,
		vk::CommandBufferLevel::ePrimary,
		1
	};

	vk::CommandBuffer command_buffer;
	result = m_logicalDevice.allocateCommandBuffers(&command_buffer_allocate_info, &command_buffer);
	assert(result == vk::Result::eSuccess);
	DEBUG_LOG("Allocated Vulkan Command Buffers; Command Pool: " << command_buffer_allocate_info.commandPool << ", Level: "  << static_cast<uint32_t>(command_buffer_allocate_info.level) << ", Count: " << command_buffer_allocate_info.commandBufferCount << ", Pointer: " << command_buffer);

	constexpr vk::CommandBufferBeginInfo command_buffer_begin_info{
		vk::CommandBufferUsageFlagBits::eOneTimeSubmit // Only submitting to the command buffer once
	};

	result = command_buffer.begin(&command_buffer_begin_info);
	assert(result == vk::Result::eSuccess);

	return command_buffer;
}

void Application::endSingleTimeCommand(const vk::CommandBuffer command_buffer, const bool need_graphics) noexcept
{
	vk::Result result;

	result = command_buffer.end();
	assert(result == vk::Result::eSuccess);

	const vk::SubmitInfo submit_info{
		0,
		nullptr,
		nullptr,
		1,
		&command_buffer
	};

	[[unlikely]] if (need_graphics) {
		result = m_queueHandles.m_graphicsQueue.submit(1, &submit_info, VK_NULL_HANDLE);
		assert(result == vk::Result::eSuccess);

		result = m_queueHandles.m_graphicsQueue.waitIdle();
		assert(result == vk::Result::eSuccess);

		m_logicalDevice.freeCommandBuffers(m_graphicsCommandPool, 1, &command_buffer);
		return;
	}

	result = m_queueHandles.m_transferQueue.submit(1, &submit_info, VK_NULL_HANDLE);
	assert(result == vk::Result::eSuccess);

	result = m_queueHandles.m_transferQueue.waitIdle();
	assert(result == vk::Result::eSuccess);

	m_logicalDevice.freeCommandBuffers(m_transferCommandPool, 1, &command_buffer);
}

void Application::setFramebufferResized() noexcept
{
	m_framebufferResized = true;
}

#ifdef _DEBUG

void Application::addDebugCallback() noexcept
{
	++m_debugCallbackCount;
}

void Application::addAllocationCallback() noexcept
{
	++m_debugAllocationCount;
}

void Application::addReallocationCallback() noexcept
{
	++m_debugReallocationCount;
}

void Application::addDeallocationCallback() noexcept
{
	++m_debugDeallocationCount;
}

#endif
