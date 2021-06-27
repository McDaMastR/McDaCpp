#include "application.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

#define SWAP_CHAIN_QUEUE_FAMILY_INDEX_COUNT 2
#define PIPELINE_SHADER_STAGE_COUNT 2
#define FRAMEBUFFER_ATTACHMENT_COUNT 3
#define DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT 1
#define DRAW_FRAME_SUBMIT_INFO_SIGNAL_SEMAPHORE_COUNT 1
#define PRESENT_INFO_SWAP_CHAIN_COUNT 1
#define DESCRIPTOR_SET_LAYOUT_BINDING_COUNT 2
#define DESCRIPTOR_POOL_SIZE_COUNT 2
#define VERTEX_BUFFER_COUNT 1
#define DESCRIPTOR_SET_WRITE_COUNT 2
#define DEPTH_FORMAT_COUNT 3
#define RENDER_PASS_ATTACHMENT_COUNT 3
#define CLEAR_VALUE_COUNT 2

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

#define APP_VERSION VK_MAKE_VERSION(1, 0, 0)
#define ENGINE_VERSION VK_MAKE_VERSION(1, 0, 0)

#ifdef _DEBUG
	#define DEBUG_LOG(x) std::cout << "Debug Log:\n\t" << x << "\n\n";
#else
	#define DEBUG_LOG(x)
#endif

#ifdef _DEBUG

#define DEBUG_CALLBACK(o, s, t, d) std::c##o << "Debug Callback:\n"\
											 << "\tServerity: " << vk::to_string(s)\
											 << "\n\tType: "    << vk::to_string(t)\
											 << "\n\tMessage: \n\t\t" << d << "\n\n"


static VKAPI_ATTR vk::Bool32 VKAPI_CALL vkDebugCallback(
	vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	vk::DebugUtilsMessageTypeFlagsEXT messageType,
	const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
	[[maybe_unused]] void *pUserData) noexcept
{
	if (messageSeverity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError) [[unlikely]]
		DEBUG_CALLBACK(err, messageSeverity, messageType, pCallbackData->pMessage);
	else [[likely]]
		DEBUG_CALLBACK(out, messageSeverity, messageType, pCallbackData->pMessage);

	// Can pass data into pUserData if needed
	return VK_FALSE; // Do not abort program
}

static void glfwErrorCallback(int error_code, const char* description) noexcept
{
	std::cerr << "GLFW Error has occured!\n\tError code: 0x" << std::hex << error_code << std::dec 
			  << "\n\tError description: " << description << '\n';
}

#endif

static void glfwFramebufferResizeCallback(GLFWwindow* window, int /* width */, int /* height */) noexcept
{
	Application * const app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
	app->setFramebufferResized();
}

Application::Application() RELEASE_NOEXCEPT
	: m_dynamicLoader(vkGetInstanceProcAddr)
{
	// GLFW
	createWindow();

	// Vulkan
	initVulkan();
}

Application::~Application() RELEASE_NOEXCEPT
{
	// GLFW
	glfwTerminate();

	// Vulkan
	for (uint8_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		m_logicalDevice.destroySemaphore(m_imageAvailableSemaphores[i], nullptr);
		m_logicalDevice.destroySemaphore(m_renderFinishedSemaphores[i], nullptr);
		m_logicalDevice.destroyFence(m_inFlightFences[i], nullptr);
	}

	destroySwapChain();
	m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout, nullptr);

	m_logicalDevice.destroySampler(m_textureSampler, nullptr);
	m_logicalDevice.destroyImageView(m_textureImageView, nullptr);
	m_logicalDevice.destroyImage(m_textureImage, nullptr);
	m_logicalDevice.freeMemory(m_textureImageMemory, nullptr);

	m_logicalDevice.destroyBuffer(m_vertexBuffer, nullptr);
	m_logicalDevice.freeMemory(m_vertexBufferMemory, nullptr); // Free buffer memory after the buffer is destroyed
	m_logicalDevice.destroyBuffer(m_indexBuffer, nullptr);
	m_logicalDevice.freeMemory(m_indexBufferMemory, nullptr);

	m_logicalDevice.destroyCommandPool(m_graphicsCommandPool, nullptr);
	m_logicalDevice.destroyCommandPool(m_transferCommandPool, nullptr);
	m_logicalDevice.destroy(nullptr);
	
	m_instance.destroySurfaceKHR(m_surface, nullptr);
#ifdef _DEBUG
	m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr, m_dynamicLoader);
#endif
	m_instance.destroy(nullptr);

	// Memory cleanup
	delete[] m_swapChainImages;
	delete[] m_swapChainImageViews;
	delete[] m_swapChainFramebuffers;
	delete[] m_commandBuffers;
	delete[] m_imagesInFlight;
	delete[] m_uniformBuffers;
	delete[] m_uniformBuffersMemory;
	delete[] m_descriptorSets;
}

void Application::createWindow() RELEASE_NOEXCEPT
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Not using OpenGL

	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

#ifdef _DEBUG
	glfwSetErrorCallback(glfwErrorCallback);
#endif

	glfwSetWindowUserPointer(m_window, this); // Bind the application with the glfw window pointer
	glfwSetFramebufferSizeCallback(m_window, glfwFramebufferResizeCallback);

	assert(glfwVulkanSupported() && "VULKAN ASSERT: Vulkan not supported on device!");
}

void Application::initVulkan() RELEASE_NOEXCEPT
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
	createGraphicsPipeline();
	createCommandPools();
	createColorResources();
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

void Application::createInstance() RELEASE_NOEXCEPT
{
	// Create result variable for result values at the start of every vulkan function (may be commented out if not used)
	vk::Result result;

	// Check validation layers
	assert(areLayersSupported<NUM_VALIDATION_LAYERS>(m_validationLayers) && "VULKAN ASSERT: Requested validation layers are not supported!");

	// Application info
	const vk::ApplicationInfo app_info(
		APP_NAME, 
		APP_VERSION, 
		ENGINE_NAME, 
		ENGINE_VERSION, 
		VK_API_VERSION_1_1 // Highest version supported by MoltenVK
	);

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
		0,
		nullptr,
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
		reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(vkDebugCallback),
		nullptr
	};
	instance_create_info.pNext = static_cast<const void *>(&debug_messenger_create_info);
#endif

	// Create instance
	result = vk::createInstance(&instance_create_info, nullptr, &m_instance);
	createResultValue(result, "vk::createInstance");

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
		reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(vkDebugCallback), // Pointer to debug callback function
		nullptr // Pointer to data for callback function to fill (not being used)
	};

	// Create debug messenger
	result = m_instance.createDebugUtilsMessengerEXT(&debug_messenger_create_info, nullptr, &m_debugMessenger, m_dynamicLoader); // Extension functions need to be dynamically loaded
	createResultValue(result, "vk::Instance::createDebugUtilsMessengerEXT");
}

#endif

void Application::createSurface() RELEASE_NOEXCEPT
{
	vk::Result result;
	VkSurfaceKHR temp_surface;

	result = static_cast<vk::Result>(glfwCreateWindowSurface(m_instance, m_window, nullptr, &temp_surface));
	createResultValue(result, "glfwCreateWindowSurface");

	m_surface = temp_surface;
}

void Application::choosePhysicalDevice() RELEASE_NOEXCEPT
{
    vk::Result result;
	vk::PhysicalDevice *physical_devices;
    uint32_t physical_device_count;

    result = m_instance.enumeratePhysicalDevices(&physical_device_count, nullptr);
    createResultValue(result, "vk::Instance::enumeratePhysicalDevices");

	physical_devices = new vk::PhysicalDevice[physical_device_count];
	result = m_instance.enumeratePhysicalDevices(&physical_device_count, physical_devices);
	createResultValue(result, "vk::Instance::enumeratePhysicalDevices");

	assert(physical_device_count && "VULKAN ASSERT: Failed to find GPU/s with Vulkan support!");
	for (uint32_t i = 0; i < physical_device_count; ++i) {
		if (physicalDeviceSupportsRequirements(physical_devices[i])) {
			m_physicalDevice = physical_devices[i];
			m_physicalDeviceProperties.pNext = &m_physicalDeviceDriverProperties;

			m_physicalDevice.getFeatures2(&m_physicalDeviceFeatures);
			m_physicalDevice.getProperties2(&m_physicalDeviceProperties);
			m_physicalDevice.getMemoryProperties2(&m_physicalDeviceMemoryProperties);

			m_msaaSamples = getMaxUsableSampleCount();
			delete[] physical_devices;

			const auto printMemoryTypes = [&]{
				for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryProperties.memoryTypeCount; ++i) {
					std::cout << "\t\tMemory Type " << i << " Property Flags:\t\t\t\t" << vk::to_string(m_physicalDeviceMemoryProperties.memoryProperties.memoryTypes[i].propertyFlags) << '\n'
							  << "\t\tMemory Type " << i << " Heap Index:\t\t\t\t" << m_physicalDeviceMemoryProperties.memoryProperties.memoryTypes[i].heapIndex << '\n';
				}

				return ' ';
			};

			const auto printMemoryHeaps = [&]{
				for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryProperties.memoryHeapCount; ++i) {
					std::cout << "\t\tMemory Heap " << i << " Size:\t\t\t\t\t" << m_physicalDeviceMemoryProperties.memoryProperties.memoryHeaps[i].size << '\n'
							  << "\t\tMemory Heap " << i << " Flags:\t\t\t\t\t" << vk::to_string(m_physicalDeviceMemoryProperties.memoryProperties.memoryHeaps[i].flags) << '\n';
				}

				return ' ';
			};

			DEBUG_LOG(
				"Physical Device Properties:\n" << std::boolalpha <<
				"\t\tAPI version:\t\t\t\t\t\t" << VK_VERSION_MAJOR(m_physicalDeviceProperties.properties.apiVersion) << '.'
								     	 	   << VK_VERSION_MINOR(m_physicalDeviceProperties.properties.apiVersion) << '.'
								     	 	   << VK_VERSION_PATCH(m_physicalDeviceProperties.properties.apiVersion) << '\n' << 

				"\t\tDriver Version:\t\t\t\t\t\t" << VK_VERSION_MAJOR(m_physicalDeviceProperties.properties.driverVersion) << '.'
								   				  << VK_VERSION_MINOR(m_physicalDeviceProperties.properties.driverVersion) << '.' // Not always the right way to get the driver version (it's vendor-specific)
								   				  << VK_VERSION_PATCH(m_physicalDeviceProperties.properties.driverVersion) << '\n' <<

				"\t\tDriver Conformance Version:\t\t\t\t" << +m_physicalDeviceDriverProperties.conformanceVersion.major << '.' 
														  << +m_physicalDeviceDriverProperties.conformanceVersion.minor << '.' 
														  << +m_physicalDeviceDriverProperties.conformanceVersion.subminor << '.' 
														  << +m_physicalDeviceDriverProperties.conformanceVersion.patch << '\n' <<

				"\t\tDriver Info:\t\t\t\t\t\t" << m_physicalDeviceDriverProperties.driverInfo.data() << '\n' << 
				"\t\tDriver ID:\t\t\t\t\t\t" << vk::to_string(m_physicalDeviceDriverProperties.driverID) << '\n' << 
				"\t\tDriver Name:\t\t\t\t\t\t" << m_physicalDeviceDriverProperties.driverName.data() << '\n' << 

				"\t\tVendor ID:\t\t\t\t\t\t0x" << std::hex << m_physicalDeviceProperties.properties.vendorID << '\n' << 
				"\t\tDevice ID:\t\t\t\t\t\t0x" << m_physicalDeviceProperties.properties.deviceID << std::dec << '\n' << 
				"\t\tDevice Type:\t\t\t\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.deviceType) << '\n' <<
				"\t\tDevice Name:\t\t\t\t\t\t" << m_physicalDeviceProperties.properties.deviceName.data() << '\n' <<
				"\t\tPipeline Cache UUID:\t\t\t\t\t0x" << std::hex << +m_physicalDeviceProperties.properties.pipelineCacheUUID[0]
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
				
				"\t\tMax Image Dimension 1D:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimension1D << '\n' <<
				"\t\tMax Image Dimension 2D:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimension2D << '\n' <<
				"\t\tMax Image Dimension 3D:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimension3D << '\n' <<
				"\t\tMax Image Dimension Cube:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageDimensionCube << '\n' <<
				"\t\tMax Image Array layers:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxImageArrayLayers << '\n' <<
				"\t\tMax Texel Buffer Elements:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTexelBufferElements << '\n' <<
				"\t\tMax Uniform Buffer Range:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxUniformBufferRange << '\n' <<
				"\t\tMax Storage Buffer Range:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxStorageBufferRange << '\n' <<
				"\t\tMax Push Constants Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxPushConstantsSize << '\n' <<
				"\t\tMax Memory Allocation Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxMemoryAllocationCount << '\n' <<
				"\t\tMax Sampler Allocation Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSamplerAllocationCount << '\n' <<
				"\t\tBuffer Image Granularity:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.bufferImageGranularity << '\n' <<
				"\t\tSparse Address Space Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.sparseAddressSpaceSize << '\n' <<
				"\t\tMax Bound Descriptor Sets:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxBoundDescriptorSets << '\n' <<
				"\t\tMax Per Stage Descriptor Samplers:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorSamplers << '\n' <<
				"\t\tMax Per Stage Descriptor Uniform Buffers:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorUniformBuffers << '\n' <<
				"\t\tMax Per Stage Descriptor Storage Buffers:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorStorageBuffers << '\n' << 
				"\t\tMax Per Stage Descriptor Sampled Images:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorSampledImages << '\n' <<
				"\t\tMax Per Stage Descriptor Storage Images:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorStorageImages << '\n' <<
				"\t\tMax Per Stage Descriptor Input Attachments:\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageDescriptorInputAttachments << '\n' <<
				"\t\tMax Per Stage Resources:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxPerStageResources << '\n' <<
				"\t\tMax Descriptor Set Samplers:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetSamplers << '\n' <<
				"\t\tMax Descriptor Set Uniform Buffers:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetUniformBuffers << '\n' <<
				"\t\tMax Descriptor Set Uniform Buffers Dynamic:\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetUniformBuffersDynamic << '\n' <<
				"\t\tMax Descriptor Set Storage Buffers:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetStorageBuffers << '\n' <<
				"\t\tMax Descriptor Set Storage Buffers Dynamic:\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetStorageBuffersDynamic << '\n' <<
				"\t\tMax Descriptor Set Sampled Images:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetSampledImages << '\n' <<
				"\t\tMax Descriptor Set Storage Images:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetStorageImages << '\n' <<
				"\t\tMax Descriptor Set Input Attchments:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDescriptorSetInputAttachments << '\n' <<
				"\t\tMax Vertex Input Attributes:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputAttributes << '\n' <<
				"\t\tMax Vertex Input Bindings:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputBindings << '\n' <<
				"\t\tMax Vertex Input Attribute Offset:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputAttributeOffset << '\n' <<
				"\t\tMax Vertex Input Binding Stride:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexInputBindingStride << '\n' <<
				"\t\tMax Vertex Output Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxVertexOutputComponents << '\n' <<
				"\t\tMax Tessellation Generation Level:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationGenerationLevel << '\n' <<
				"\t\tMax Tessellation Patch Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationPatchSize << '\n' <<
				"\t\tMax Tessellation Control Per Vertex Input Components:\t" << m_physicalDeviceProperties.properties.limits.maxTessellationControlPerVertexInputComponents << '\n' <<
				"\t\tMax Tessellation Control Per Vertex Output Components:\t" << m_physicalDeviceProperties.properties.limits.maxTessellationControlPerVertexOutputComponents << '\n' <<
				"\t\tMax Tessellation Control Per Patch Output Components:\t" << m_physicalDeviceProperties.properties.limits.maxTessellationControlPerPatchOutputComponents << '\n' <<
				"\t\tMax Tessellation Evaluation Input Components:\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationEvaluationInputComponents << '\n' <<
				"\t\tMax Tessellation Evaluation Output Components:\t\t" << m_physicalDeviceProperties.properties.limits.maxTessellationEvaluationOutputComponents << '\n' <<
				"\t\tMax Geometry Shader Invocations:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryShaderInvocations << '\n' <<
				"\t\tMax Geometry Input Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryInputComponents << '\n' <<
				"\t\tMax Geometry Output Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryOutputComponents << '\n' <<
				"\t\tMax Geometry Output Vertices:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryOutputVertices << '\n' <<
				"\t\tMax Geometry Total Output Components:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxGeometryTotalOutputComponents << '\n' <<
				"\t\tMax Fragment Input Components:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentInputComponents << '\n' <<
				"\t\tMax Fragment Output Attachments:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentOutputAttachments << '\n' <<
				"\t\tMax Fragment Dual Src Attachments:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentDualSrcAttachments << '\n' <<
				"\t\tMax Fragment Combined Output Resources:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFragmentCombinedOutputResources << '\n' <<
				"\t\tMax Compute Shared Memory Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeSharedMemorySize << '\n' <<
				"\t\tMax Compute Work Group Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupCount[0] << ", " 
															<< m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupCount[1] << ", " 
															<< m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupCount[2] << '\n' <<

				"\t\tMax Compute Work Group Invocations:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupInvocations << '\n' <<
				"\t\tMax Compute Work Group Size:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupSize[0] << ", " 
														   << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupSize[1] << ", " 
														   << m_physicalDeviceProperties.properties.limits.maxComputeWorkGroupSize[2] << '\n' <<

				"\t\tSub Pixel Precesion Bits:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.subPixelPrecisionBits << '\n' <<
				"\t\tSub Texel Precesion Bits:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.subTexelPrecisionBits << '\n' <<
				"\t\tMipmap Precesion Bits:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.mipmapPrecisionBits << '\n' <<
				"\t\tMax Draw Indexed Index Value:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDrawIndexedIndexValue << '\n' <<
				"\t\tMax Draw Indirect Count:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxDrawIndirectCount << '\n' <<
				"\t\tMax Sampler LOD Bias:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSamplerLodBias << '\n' <<
				"\t\tMax Sampler Anisotropy:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSamplerAnisotropy << '\n' <<
				"\t\tMax Viewports:\t\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxViewports << '\n' <<
				"\t\tMax Viewport Dimensions:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxViewportDimensions[0] << ", " 
													   << m_physicalDeviceProperties.properties.limits.maxViewportDimensions[1] << '\n' <<
				
				"\t\tViewport Bounds Range:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.viewportBoundsRange[0] << ", " 
													   << m_physicalDeviceProperties.properties.limits.viewportBoundsRange[1] << '\n' <<
				
				"\t\tViewport Sub Pixel Bits:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.viewportSubPixelBits << '\n' <<
				"\t\tMin Memory Map Alignment:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minMemoryMapAlignment << '\n' <<
				"\t\tMin Texel Buffer Offset Alignment:\t\t\t" << m_physicalDeviceProperties.properties.limits.minTexelBufferOffsetAlignment << '\n' <<
				"\t\tMin Uniform Buffer Offset Alignment:\t\t\t" << m_physicalDeviceProperties.properties.limits.minUniformBufferOffsetAlignment << '\n' <<
				"\t\tMin Storage Buffer Offset Alignment:\t\t\t" << m_physicalDeviceProperties.properties.limits.minStorageBufferOffsetAlignment << '\n' <<
				"\t\tMin Texel Offset:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minTexelOffset << '\n' <<
				"\t\tMax Texel Offset:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTexelOffset << '\n' <<
				"\t\tMin Texel Gather Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minTexelGatherOffset << '\n' <<
				"\t\tMax Texel Gather Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxTexelGatherOffset << '\n' <<
				"\t\tMin Interpolation Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.minInterpolationOffset << '\n' <<
				"\t\tMax Interpolation Offset:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxInterpolationOffset << '\n' <<
				"\t\tSub Pixel Interpolation Offset Bits:\t\t\t" << m_physicalDeviceProperties.properties.limits.subPixelInterpolationOffsetBits << '\n' <<
				"\t\tMax Framebuffer Width:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFramebufferWidth << '\n' <<
				"\t\tMax Framebuffer Height:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFramebufferHeight << '\n' <<
				"\t\tMax Framebuffer Layers:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxFramebufferLayers << '\n' <<
				"\t\tFramebuffer Color Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferColorSampleCounts) << '\n' <<
				"\t\tFramebuffer Depth Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferDepthSampleCounts) << '\n' <<
				"\t\tFramebuffer Stencil Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferStencilSampleCounts) << '\n' <<
				"\t\tFramebuffer No Attachments Sample Counts:\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.framebufferNoAttachmentsSampleCounts) << '\n' <<
				"\t\tMax Color Attachments:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxColorAttachments << '\n' <<
				"\t\tSampled Image Color Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageColorSampleCounts) << '\n' <<
				"\t\tSampled Image Integer Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageIntegerSampleCounts) << '\n' <<
				"\t\tSampled Image Depth Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageDepthSampleCounts) << '\n' <<
				"\t\tSampled Image Stencil Sample Counts:\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.sampledImageStencilSampleCounts) << '\n' <<
				"\t\tStorage Image Sample Counts:\t\t\t\t" << vk::to_string(m_physicalDeviceProperties.properties.limits.storageImageSampleCounts) << '\n' <<
				"\t\tMax Sample Mask Words:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxSampleMaskWords << '\n' <<
				"\t\tTimestamp Compute And Graphics:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.limits.timestampComputeAndGraphics) << '\n' <<
				"\t\tTimestamp Period:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.timestampPeriod << '\n' <<
				"\t\tMax Clip Distances:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxClipDistances << '\n' <<
				"\t\tMax Cull Distances:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.maxCullDistances << '\n' <<
				"\t\tMax Combined Clip And Cull Distances:\t\t\t" << m_physicalDeviceProperties.properties.limits.maxCombinedClipAndCullDistances << '\n' <<
				"\t\tDiscrete Queue Priorities:\t\t\t\t" << m_physicalDeviceProperties.properties.limits.discreteQueuePriorities << '\n' <<
				"\t\tPoint Size Range:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.pointSizeRange[0] << ", "
												  << m_physicalDeviceProperties.properties.limits.pointSizeRange[1] << '\n' <<
				
				"\t\tLine Width Range:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.lineWidthRange[0] << ", "
												  << m_physicalDeviceProperties.properties.limits.lineWidthRange[1] << '\n' <<
				
				"\t\tPoint Size Granularity:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.pointSizeGranularity << '\n' <<
				"\t\tLine Width Granularity:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.lineWidthGranularity << '\n' <<
				"\t\tStrict Lines:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.limits.strictLines) << '\n' <<
				"\t\tStandard Sample Locations:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.limits.standardSampleLocations) << '\n' <<
				"\t\tOptimal Buffer Copy Offset Alignement:\t\t\t" << m_physicalDeviceProperties.properties.limits.optimalBufferCopyOffsetAlignment << '\n' <<
				"\t\tOptimal Buffer Copy Row Pitch Alignement:\t\t" << m_physicalDeviceProperties.properties.limits.optimalBufferCopyRowPitchAlignment << '\n' <<
				"\t\tNon Coherent Atom Size:\t\t\t\t\t" << m_physicalDeviceProperties.properties.limits.nonCoherentAtomSize << '\n' <<
				"\t\tResidency Standard 2D Block Shape:\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyStandard2DBlockShape) << '\n' <<
				"\t\tResidency Standard 2D Multisample Block Shape:\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyStandard2DMultisampleBlockShape) << '\n' <<
				"\t\tResidency Standard 3D Block Shape:\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyStandard3DBlockShape) << '\n' <<
				"\t\tResidency Aligned Mip Size:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyAlignedMipSize) << '\n' <<
				"\t\tResidency Non Resident Strict:\t\t\t\t" << static_cast<bool>(m_physicalDeviceProperties.properties.sparseProperties.residencyNonResidentStrict) << '\n' <<
				printMemoryTypes() << 
				printMemoryHeaps() <<
				"\t\tRobust Buffer Access:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.robustBufferAccess) << '\n' <<
				"\t\tFull Draw Index Uint32:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.fullDrawIndexUint32) << '\n' <<
				"\t\tImage Cube Array:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.imageCubeArray) << '\n' <<
				"\t\tIndependent Blend:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.independentBlend) << '\n' <<
				"\t\tGeometry Shader:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.geometryShader) << '\n' <<
				"\t\tTessellation Shader:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.tessellationShader) << '\n' <<
				"\t\tSample Rate Shading:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sampleRateShading) << '\n' <<
				"\t\tDual Src Blend:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.dualSrcBlend) << '\n' <<
				"\t\tLogic Op:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.logicOp) << '\n' <<
				"\t\tMulti Draw Indirect:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.multiDrawIndirect) << '\n' <<
				"\t\tDraw Indirect First Instance:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.drawIndirectFirstInstance) << '\n' <<
				"\t\tDepth Clamp:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.depthClamp) << '\n' <<
				"\t\tDepth Bias Clamp:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.depthBiasClamp) << '\n' <<
				"\t\tFill Mode Non Solid:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.fillModeNonSolid) << '\n' <<
				"\t\tDepth Bounds:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.depthBounds) << '\n' <<
				"\t\tWide Lines:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.wideLines) << '\n' <<
				"\t\tLarge Points:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.largePoints) << '\n' <<
				"\t\tAlpha To One:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.alphaToOne) << '\n' <<
				"\t\tMulti Viewport:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.multiViewport) << '\n' <<
				"\t\tSampler Anisotropy:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.samplerAnisotropy) << '\n' <<
				"\t\tTexture Compression ETC 2:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.textureCompressionETC2) << '\n' <<
				"\t\tTexture Compression ASTC LDR:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.textureCompressionASTC_LDR) << '\n' <<
				"\t\tTexture Compression BC:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.textureCompressionBC) << '\n' <<
				"\t\tOcclusion Query Precise:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.occlusionQueryPrecise) << '\n' <<
				"\t\tPipeline Statistics Query:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.pipelineStatisticsQuery) << '\n' <<
				"\t\tVertex Pipeline Stores And Atomics:\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.vertexPipelineStoresAndAtomics) << '\n' <<
				"\t\tFragment Stores And Atomics:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.fragmentStoresAndAtomics) << '\n' <<
				"\t\tShader Tessellation And Geometry Point Size:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderTessellationAndGeometryPointSize) << '\n' <<
				"\t\tShader Image Gather Extended:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderImageGatherExtended) << '\n' <<
				"\t\tShader Storage Image Extended Formats:\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageExtendedFormats) << '\n' <<
				"\t\tShader Storage Image Multisample:\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageMultisample) << '\n' <<
				"\t\tShader Storage Image Read Without Format:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageReadWithoutFormat) << '\n' <<
				"\t\tShader Storage Image Write Without Format:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageWriteWithoutFormat) << '\n' <<
				"\t\tShader Uniform Buffer Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderUniformBufferArrayDynamicIndexing) << '\n' <<
				"\t\tShader Sampled Image Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderSampledImageArrayDynamicIndexing) << '\n' <<
				"\t\tShader Storage Buffer Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageBufferArrayDynamicIndexing) << '\n' <<
				"\t\tShader Storage Image Array Dynamic Indexing:\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderStorageImageArrayDynamicIndexing) << '\n' <<
				"\t\tShader Clip Distance:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderClipDistance) << '\n' <<
				"\t\tShader Cull Distance:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderCullDistance) << '\n' <<
				"\t\tShader Float64:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderFloat64) << '\n' <<
				"\t\tShader Int64:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderInt64) << '\n' <<
				"\t\tShader Int16:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderInt16) << '\n' <<
				"\t\tShader Resource Residency:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderResourceResidency) << '\n' <<
				"\t\tShader Resource Min LOD:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.shaderResourceMinLod) << '\n' <<
				"\t\tSparse Binding:\t\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseBinding) << '\n' <<
				"\t\tSparse Residency Buffer:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyBuffer) << '\n' <<
				"\t\tSparse Residency Image 2D:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyImage2D) << '\n' <<
				"\t\tSparse Residency Image 3D:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyImage3D) << '\n' <<
				"\t\tSparse Residency 2 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency2Samples) << '\n' <<
				"\t\tSparse Residency 4 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency4Samples) << '\n' <<
				"\t\tSparse Residency 8 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency8Samples) << '\n' <<
				"\t\tSparse Residency 16 Samples:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidency16Samples) << '\n' <<
				"\t\tSparse Residency Aliased:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.sparseResidencyAliased) << '\n' <<
				"\t\tVariable Multisample Rate:\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.variableMultisampleRate) << '\n' <<
				"\t\tInherited Queries:\t\t\t\t\t" << static_cast<bool>(m_physicalDeviceFeatures.features.inheritedQueries)
			);

			return;
		}
	}

	delete[] physical_devices;
	assert(false && "VULKAN ASSERT: Failed to find GPU with rquested requirements!");
}

void Application::createLogicalDevice() RELEASE_NOEXCEPT
{
	vk::Result result;

	m_queueFamilyIndices = getQueueFamilies(m_physicalDevice);
	if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.transferFamily.value())
		DEBUG_LOG("Graphics family found, transfer family missing: using graphics family");

	// Required device features
	const vk::PhysicalDeviceFeatures2 features{
		{
			VK_FALSE, // Robust buffer access
			VK_FALSE, // Full draw index uint32
			VK_FALSE, // Image cube array
			VK_FALSE, // Independent blend
			VK_FALSE, // Geometry shader
			VK_FALSE, // Tessellation shader
			VK_TRUE, // Sample rate shading
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

	const float queue_priority = 1.0f;
	const std::unordered_set<uint32_t> queue_families = {
		m_queueFamilyIndices.graphicsFamily.value(),
		m_queueFamilyIndices.presentFamily.value(),
		m_queueFamilyIndices.transferFamily.value()
	};

	vk::DeviceQueueCreateInfo * const device_queue_create_infos = new vk::DeviceQueueCreateInfo[queue_families.size()];
	if (queue_families.size() == 1) {
		for (const auto &queue_family : queue_families) {
			const vk::DeviceQueueCreateInfo device_queue_create_info{
				{}, // Flags
				queue_family, // Queue family index
				1, // Queue count
				&queue_priority // Queue priorities
			};

			device_queue_create_infos[0] = std::move(device_queue_create_info);
		}
	} else if (queue_families.size() == 2) {
		uint8_t i = 0;
		for (const auto &queue_family : queue_families) {
			const vk::DeviceQueueCreateInfo device_queue_create_info{
				{},
				queue_family,
				2, 
				&queue_priority
			};

			device_queue_create_infos[i] = std::move(device_queue_create_info);
			++i;
		}
	} else {
		uint8_t i = 0;
		for (const auto &queue_family : queue_families) {
			const vk::DeviceQueueCreateInfo device_queue_create_info{
				{}, 
				queue_family, 
				3, 
				&queue_priority
			};

			device_queue_create_infos[i] = std::move(device_queue_create_info);
			++i;
		}
	}

	const vk::DeviceCreateInfo device_create_info{
		{}, // Flags
		static_cast<uint32_t>(queue_families.size()), // Queue create info count: number of queue create infos
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
	result = m_physicalDevice.createDevice(&device_create_info, nullptr, &m_logicalDevice);
	createResultValue(result, "vk::PhysicalDevice::createDevice");

	// Add more functions to dynamic loader
	m_dynamicLoader.init(m_logicalDevice);

	// Get handles to the queue families
	vk::DeviceQueueInfo2 queue_info{
		{}, // Flags
		m_queueFamilyIndices.graphicsFamily.value(), // Queue family index
		0 // Queue index
	};

	m_logicalDevice.getQueue2(&queue_info, &m_graphicsQueue);

	// Create the right amount of queue family handles
	if (queue_families.size() == 1) {
		m_logicalDevice.getQueue2(&queue_info, &m_presentQueue);
		m_logicalDevice.getQueue2(&queue_info, &m_transferQueue);
		return;
	} 
	if (queue_families.size() == 2) {
		[[likely]] if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.presentFamily.value()) {
			m_logicalDevice.getQueue2(&queue_info, &m_presentQueue);

			queue_info.queueFamilyIndex = m_queueFamilyIndices.transferFamily.value();
			queue_info.queueIndex = 1;
			m_logicalDevice.getQueue2(&queue_info, &m_transferQueue);
			return;
		}

		m_logicalDevice.getQueue2(&queue_info, &m_transferQueue);

		queue_info.queueFamilyIndex = m_queueFamilyIndices.presentFamily.value();
		queue_info.queueIndex = 1;
		m_logicalDevice.getQueue2(&queue_info, &m_presentQueue);
		return;
	}

	queue_info.queueFamilyIndex = m_queueFamilyIndices.presentFamily.value();
	queue_info.queueIndex = 1;
	m_logicalDevice.getQueue2(&queue_info, &m_presentQueue);

	queue_info.queueFamilyIndex = m_queueFamilyIndices.transferFamily.value();
	queue_info.queueIndex = 2;
	m_logicalDevice.getQueue2(&queue_info, &m_transferQueue);

	// Free memory
	delete[] device_queue_create_infos;
}

void Application::createSwapChain() RELEASE_NOEXCEPT
{
	vk::Result result;

	const SwapChainSupportDetails swap_chain_support = querySwapChainSupport(m_physicalDevice);
	const vk::SurfaceFormat2KHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats, swap_chain_support.formatCount);
	const vk::PresentModeKHR present_mode = chooseSwapPresentMode(swap_chain_support.presentModes, swap_chain_support.presentModeCount);
	const vk::Extent2D extent = chooseSwapExtent(swap_chain_support.capabilities);

	uint32_t image_count = swap_chain_support.capabilities.surfaceCapabilities.minImageCount + 1; // Amount of images to be stored in the queue (recommended to request at least 1 more than the minimum)

	if (swap_chain_support.capabilities.surfaceCapabilities.maxImageCount > 0 && // If maxImageCount == 0, then there is no maximum
		image_count > swap_chain_support.capabilities.surfaceCapabilities.maxImageCount) // Wanted queue size is larger than the maxmimum
		image_count = swap_chain_support.capabilities.surfaceCapabilities.maxImageCount;

	vk::SwapchainCreateInfoKHR swapchain_create_info{
		{}, // Flags
		m_surface, // Surface
		image_count, // Min image count
		surface_format.surfaceFormat.format, // Image format
		surface_format.surfaceFormat.colorSpace, // Image color space
		extent, // Image extent
		1, // Image array layers: The amount of layers each image consists of
		vk::ImageUsageFlagBits::eColorAttachment, // The kind of operations we'll use the images in the swap chain for (color attachment == render directly to them)
		vk::SharingMode::eExclusive, // Image sharing mode: whether images are owned by one queue family at a time, or multiple (one; offers the best performance)
		{}, // Queue family index count
		{}, // Queue family indices
		swap_chain_support.capabilities.surfaceCapabilities.currentTransform, // Pre transform (current transform means add no transform)
		vk::CompositeAlphaFlagBitsKHR::eOpaque, // For blending with other windows in the window system (Opaque means to ignore the alpha channel)
		present_mode, // Present mode
		VK_TRUE, // Clipped: true if you don't care about the color of pixels that are obscured, for example because another window is in front of them
		{} // Old swap chain: reference to old swap chain if current is invalid or unoptimized (e.g. because window is resized). No given reference means we'll only create one swap chain
	};
	
	// Queue families
	if (m_queueFamilyIndices.graphicsFamily != m_queueFamilyIndices.presentFamily) { // Use concurrent mode if queue families differ to avoid manual ownership transfer
		const uint32_t queue_family_indices[SWAP_CHAIN_QUEUE_FAMILY_INDEX_COUNT] = {
			m_queueFamilyIndices.graphicsFamily.value(),
			m_queueFamilyIndices.presentFamily.value()
		};

		swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent; // Images can be used across multiple queue families without explicit ownership transfers
		swapchain_create_info.queueFamilyIndexCount = SWAP_CHAIN_QUEUE_FAMILY_INDEX_COUNT;
		swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
	}

	// Create the swapchain
	result = m_logicalDevice.createSwapchainKHR(&swapchain_create_info, nullptr, &m_swapChain);
	createResultValue(result, "vk::Device::createSwapchainKHR");

	m_swapChainImageFormat = surface_format.surfaceFormat.format;
	m_swapChainExtent = extent;

	result = m_logicalDevice.getSwapchainImagesKHR(m_swapChain, &m_swapChainImageCount, nullptr);
	createResultValue(result, "vk::Device::getSwapchainImagesKHR");

	m_swapChainImages = new vk::Image[m_swapChainImageCount];
	result = m_logicalDevice.getSwapchainImagesKHR(m_swapChain, &m_swapChainImageCount, m_swapChainImages);
	createResultValue(result, "vk::Device::getSwapchainImagesKHR");
}

void Application::createSwapChainImageViews() RELEASE_NOEXCEPT
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

void Application::createRenderPass() RELEASE_NOEXCEPT
{
	vk::Result result;

	// Color buffer attachment represented by one of the images from the swap chain
	const vk::AttachmentDescription2KHR color_attachment{ // KHR extension being used because it wasn't core until 1.2 (using 1.1)
		{}, // Flags
		m_swapChainImageFormat, // Format (matches the swap chain image format)
		m_msaaSamples, // Samples
		vk::AttachmentLoadOp::eClear, // Load op: what to do with the color and depth data in the attachment before rendering (clear the values to a constant at the start)
		vk::AttachmentStoreOp::eDontCare, // Store op: what to do with the color and depth data in the attachment after rendering
		vk::AttachmentLoadOp::eDontCare, // Stencil load op: what to do with the stencil data in the attachment before rendering 
		vk::AttachmentStoreOp::eDontCare, // Stencil store op: what to do with the stencil data in the attachment after rendering
		vk::ImageLayout::eUndefined, // Initial layout: the layout the image will have before the render pass begins (don't care)
		vk::ImageLayout::eColorAttachmentOptimal // Final layout: the layout to automatically transition to when the render pass finishes
	};

	// Depth buffer attachment
	const vk::AttachmentDescription2KHR depth_attachment{
		{}, 
		findDepthFormat(), 
		m_msaaSamples,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eDontCare, 
		vk::AttachmentLoadOp::eDontCare, 
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined, 
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};

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

	const vk::AttachmentDescription2KHR attachment_descriptions[RENDER_PASS_ATTACHMENT_COUNT] = {
		std::move(color_attachment),
		std::move(depth_attachment),
		std::move(color_attachment_resolve)
	};

	// Reference attachments using the indices of an array of attachments
	const vk::AttachmentReference2KHR color_attachment_reference{
		0, // Attachment: which attachment to reference by its index in the attachment descriptions array
		vk::ImageLayout::eColorAttachmentOptimal, // Layout: which layout we would like the attachment to have during a subpass that uses this reference (using color buffer and wanting optimal preformance)
		{} // Aspect mask: Only used for input attachents
	};

	const vk::AttachmentReference2KHR depth_attachment_reference{
		1, 
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};

	const vk::AttachmentReference2KHR color_attachment_resolve_reference{
		2, 
		vk::ImageLayout::eColorAttachmentOptimal
	};

	// Subsequent rendering operations that depend on the contents of framebuffers in previous passes
	const vk::SubpassDescription2KHR subpass{
		{}, // Flags
		vk::PipelineBindPoint::eGraphics, // Pipeline bind point: type of subpass (graphics subpass)
		0, // View mask
		0, // Input attachment count: number of input attachments (not using any)
		nullptr, // Input attachments: attachments that are read from a shader (not using any)
		1, // Color attachment count: number of color attachments (using 1)
		&color_attachment_reference, // Color attachments: attachments used for outputing pixel color (this is index 0 of the color attachment array, referenced from the fragment shader: layout(location = 0) out vec4 color)
		&color_attachment_resolve_reference, // Resolve attachments: Attachments used for multisampling color attachments
		&depth_attachment_reference, // Depth stencil attachment: Attachment for depth and stencil data (not using)
		0, // Preserve attachment count: number of preserve attachments (not using any)
		nullptr // Preserve attachments: attachments that are not used by this subpass, but for which the data must be preserved (not using any)
	};

	// Dependencies automatically takes care of image layout transitions
	const vk::SubpassDependency2KHR subpass_dependency{
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
		1, // Dependency count: number of dependecies (using 1)
		&subpass_dependency, // Dependecies: array of subpass dependecies (not using any)
		0, // Correlated view mask count
		nullptr // Correlated view masks
	};

	// Create the render pass
	result = m_logicalDevice.createRenderPass2KHR(&render_pass_create_info, nullptr, &m_renderPass, m_dynamicLoader); // Extension function needs to be dynamically loaded
	createResultValue(result, "vk::Device::createRenderPass2KHR");
}

void Application::createDescriptorSetLayout() RELEASE_NOEXCEPT
{
	vk::Result result;

	// Information for the UBO bindings
	const vk::DescriptorSetLayoutBinding ubo_layout_binding{
		0, // Binding
		vk::DescriptorType::eUniformBuffer, // Descriptor type
		1, // Descriptor count: the number of values in the array of UBOs (only using 1)
		vk::ShaderStageFlagBits::eVertex, // Stage flags: the shader stages the UBO is referenced in
		nullptr // Immutable samplers: for image sampling
	};

	// Information for the image sampler
	const vk::DescriptorSetLayoutBinding sampler_layout_binding{
		1,
		vk::DescriptorType::eCombinedImageSampler,
		1,
		vk::ShaderStageFlagBits::eFragment
	};

	// Array of layout bindings
	const vk::DescriptorSetLayoutBinding layout_bindings[DESCRIPTOR_SET_LAYOUT_BINDING_COUNT] = {
		std::move(ubo_layout_binding),
		std::move(sampler_layout_binding)
	};

	// Information for the creation of the descriptor set
	const vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{
		{}, // Flags
		DESCRIPTOR_SET_LAYOUT_BINDING_COUNT, // Binding count
		layout_bindings // Bindings
	};

	// Create the descriptor set
	result = m_logicalDevice.createDescriptorSetLayout(&descriptor_set_layout_create_info, nullptr, &m_descriptorSetLayout);
	createResultValue(result, "vk::Device::createDescriptorSetLayout");
}

void Application::createGraphicsPipeline() RELEASE_NOEXCEPT
{
	vk::Result result;

	const std::vector<char> vertex_shader_code = readFile(std::move("bin/vert.spv"));
	const std::vector<char> fragment_shader_code = readFile(std::move("bin/frag.spv"));

	const vk::ShaderModule vertex_shader_module = createShaderModule(vertex_shader_code);
	const vk::ShaderModule fragment_shader_module = createShaderModule(fragment_shader_code);

	// Describes the shader module
	const vk::PipelineShaderStageCreateInfo vertex_shader_stage_create_info{
		{}, // Flags
		vk::ShaderStageFlagBits::eVertex, // Shader Stage/Type (Vertex)
		std::move(vertex_shader_module), // Shader module containing the code
		"main", // Entrypoint of the shader (main function)
		{} // Specialization Info: specify values for shader constants (not using)
	};

	const vk::PipelineShaderStageCreateInfo fragment_shader_stage_create_info{
		{},
		vk::ShaderStageFlagBits::eFragment,
		std::move(fragment_shader_module),
		"main",
		{}
	};

	// Create an array containing the shader stage create infos
	const vk::PipelineShaderStageCreateInfo shader_stages[PIPELINE_SHADER_STAGE_COUNT] = {
		std::move(vertex_shader_stage_create_info),
		std::move(fragment_shader_stage_create_info)
	};

	const vk::VertexInputBindingDescription vertex_binding_description = Vertex::getBindingDescription();
	const std::array<vk::VertexInputAttributeDescription, VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT> vertex_attribute_descriptions = Vertex::getAttributeDescriptions();

	// Describes the format of the vertex data that will be passed to the vertex shader
	const vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info{
		{}, // Flags
		VERTEX_INPUT_BINDING_DESCRIPTION_COUNT, // Vertex binding description count (no data being passed into vertex shader)
		&vertex_binding_description, // Vertex binding descriptions: points to an array of structs that describe the details for loading vertex bindings (no data is being loaded)
		VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_COUNT, // Vertex attribute description count (no data being passed into vertex shader)
		vertex_attribute_descriptions.data() // Vertex attribute descriptions: points to an array of structs that describe the details for loading vertex attributes (no data is being loaded)
	};

	// Describes what kind of geometry will be drawn from the vertices and if primitive restart should be enabled
	const vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{
		{}, // Flags
		vk::PrimitiveTopology::eTriangleList, // Topology: the kind of geometry that will be drawn from the vertices (1 triangle from every 3 vertices without reuse)
		VK_FALSE // Primitive restart: can break up lines and triangles in the strip topology modes (not using strip topology mode)
	};

	// Describes the region of the framebuffer that the output will be rendered to
	// This will almost always be (0, 0) to (width, height)
	const vk::Viewport viewport{
		0.0f, // X
		0.0f, // Y
		static_cast<float>(m_swapChainExtent.width), // Width
		static_cast<float>(m_swapChainExtent.height), // Height
		0.0f, // Min depth: the minimum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
		1.0f // Max depth: the maximum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
	};

	// Scissor rectangle offset
	const vk::Offset2D scissor_rectangle_offset{
		0, // X
		0 // Y
	};

	// Defines which regions pixels will actually be stored
	// Any pixels outside the scissor rectangles will be discarded by the rasterizer
	const vk::Rect2D scissor_rectangle{
		std::move(scissor_rectangle_offset), // Offset
		m_swapChainExtent // Extent (the chosen extent of the screen)
	};

	// Viewport state containing viewport and scissor rectangle
	const vk::PipelineViewportStateCreateInfo viewport_state_create_info{
		{}, // Flags
		1, // Viewport count: number of viewports (only using 1)
		&viewport, // Viewports: pointer to array of viewports being used (only using 1)
		1, // Scissor count: number of scissor rectangles (only using 1)
		&scissor_rectangle // Scissors: pointor to array of scissor rectangles being used (only using 1)
	};

	// Takes the geometry that is shaped by the vertices from the vertex shader and turns it into fragments to be colored by the fragment shader
	// Performs depth testing, face culling and the scissor test, and it can be configured to output fragments that fill entire polygons or just the edges
	const vk::PipelineRasterizationStateCreateInfo rasterization_state_create_info{
		{}, // Flags
		VK_FALSE, // Depth clamp enable: if true, then fragments that are beyond the near and far planes are clamped to them as opposed to discarding them (not using)
		VK_FALSE, // Rasterizer discard enable: if true, then geometry never passes through the rasterizer stage; disables any output to the framebuffer
		vk::PolygonMode::eFill, // Polygon mode: how fragments are generated for geometry (fill the area of the polygon with fragments)
		vk::CullModeFlagBits::eBack, // Cull mode: the type of face culling to use (cull the back faces)
		vk::FrontFace::eCounterClockwise, // Front face: specifies the vertex order for faces to be considered front-facing (clockwise)
		VK_FALSE, // Depth bias enable: alters the depth values by adding a constant value or biasing them based on a fragment's slope (not using)
		0.0f, // Depth bias constant factor (not using)
		0.0f, // Depth bias clamp (not using)
		0.0f, // Depth bias slope factor (not using)
		1.0f // Line width: the thickness of lines in terms of number of fragments (1 fragment thick)
	};

	// Configures multisampling by combining the fragment shader results of multiple polygons that rasterize to the same pixel
	// Not using multisampling, so struct is disabled
	const vk::PipelineMultisampleStateCreateInfo multisample_state_create_info{
		{}, // Flags
		m_msaaSamples, // Rasterization samples
		VK_TRUE, // Sample shading enable
		0.2f, // Min sample shading: min fraction for sample shading; closer to one is smoother
		nullptr, // Sample mask
		VK_FALSE, // Alpha to coverage enable
		VK_FALSE // Alpha to one enable
	};

	// Configures the depth and stencil tests
	const vk::PipelineDepthStencilStateCreateInfo depth_stencil_state_create_info{
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
	// Not using blending, so struct is disabled
	const vk::PipelineColorBlendAttachmentState color_blend_attachment_state{
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
	const std::array<float, 4> blend_constants = {0.0f};

	// Contains the global color blending settings
	// References the array of structures for all of the framebuffers and allows you to set blend constants
	const vk::PipelineColorBlendStateCreateInfo color_blend_state_create_info{
		{}, // Flags
		VK_FALSE, // Logic op enable: use bitwise combination for blending (disabled)
		vk::LogicOp::eCopy, // Logic op: the bitwise operation for blending
		1, // Attachment count: number of color blend attachments
		&color_blend_attachment_state, // Attachments: pointer to array of color blend attachments
		blend_constants // Blend constants
	};

	// States that can and have to be changed at drawing time (not using)
	// const vk::DynamicState dynamic_states[PIPELINE_DYNAMIC_STATE_COUNT] = {
	// 	vk::DynamicState::eViewport,
	// 	vk::DynamicState::eLineWidth
	// };

	// Describes which pipeline configurations can be changed without recreating the pipeline (not using)
	// const vk::PipelineDynamicStateCreateInfo dynamic_state_create_info{
	// 	{}, // Flags
	// 	PIPELINE_DYNAMIC_STATE_COUNT, // Dynamic state count: number of dynamic states
	// 	dynamic_states // Dynamic states: pointer to array of dynamic states
	// };

	// Infomation for the pipeline layout
	const vk::PipelineLayoutCreateInfo pipeline_layout_create_info{
		{}, // Flags
		1, // Set layout count
		&m_descriptorSetLayout, // Set layouts
		0, // Push constant range count
		nullptr // Push constant ranges
	};

	// Create pipeline layout
	result = m_logicalDevice.createPipelineLayout(&pipeline_layout_create_info, nullptr, &m_pipelineLayout);
	createResultValue(result, "vk::Device::createPipelineLayout");

	// Graphics pipeline info
	const vk::GraphicsPipelineCreateInfo pipeline_create_info{
		{}, // Flags
		PIPELINE_SHADER_STAGE_COUNT, // Stage count: number of shader stages (2: vertex and fragment)
		shader_stages, // Stages: the array of shader stages
		&vertex_input_state_create_info, // Vertex input state
		&input_assembly_state_create_info, // Input assembly state
		nullptr, // Tessellation state
		&viewport_state_create_info, // Viewport state
		&rasterization_state_create_info, // Rasterization state
		&multisample_state_create_info, // Multisample state
		&depth_stencil_state_create_info, // Depth stencil state
		&color_blend_state_create_info, // Color blend state
		nullptr, // Dynamic state
		m_pipelineLayout, // Layout
		m_renderPass, // Render pass
		0, // Subpass: index of the subpass where this graphics pipeline will be used
		nullptr, // Base pipeline handle
		-1 // Base pipeline index
	};

	// Create the graphics pipeline
	result = m_logicalDevice.createGraphicsPipelines(nullptr, 1, &pipeline_create_info, nullptr, &m_graphicsPipeline);
	createResultValue(result, "vk::Device::createGraphicsPipelines");

	// Shader modules are no longer needed
	m_logicalDevice.destroyShaderModule(vertex_shader_module, nullptr);
	m_logicalDevice.destroyShaderModule(fragment_shader_module, nullptr);
}

void Application::createFrameBuffers() RELEASE_NOEXCEPT
{
	vk::Result result;

	[[unlikely]] if (m_swapChainFramebuffers == nullptr) // Unlikely because this function may be called many times, but this branch will only be gone into once
		m_swapChainFramebuffers = new vk::Framebuffer[m_swapChainImageCount];

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		const vk::ImageView attachments[FRAMEBUFFER_ATTACHMENT_COUNT] = {
			m_colorImageView,
			m_depthImageView,
			m_swapChainImageViews[i]
		};

		const vk::FramebufferCreateInfo framebuffer_create_info{
			{}, // Flags
			m_renderPass, // Render pass
			FRAMEBUFFER_ATTACHMENT_COUNT, // Attachment count
			attachments, // Attachments: the vk::ImageView objects that should be bound to the respective attachment descriptions in the render pass Attachment array
			m_swapChainExtent.width, // Width
			m_swapChainExtent.height, // Height
			1 // Layers: number of layers in image arrays (using single images)
		};

		// Create the framebuffer
		result = m_logicalDevice.createFramebuffer(&framebuffer_create_info, nullptr, &m_swapChainFramebuffers[i]);
		createResultValue(result, "vk::Device::createFramebuffer");
	}
}

void Application::createCommandPools() RELEASE_NOEXCEPT
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
	result = m_logicalDevice.createCommandPool(&graphics_command_pool_create_info, nullptr, &m_graphicsCommandPool);
	createResultValue(result, "vk::Device::createCommandPool");

	result = m_logicalDevice.createCommandPool(&transfer_command_pool_create_info, nullptr, &m_transferCommandPool);
	createResultValue(result, "vk::Device::createCommandPool");
}

void Application::createColorResources() RELEASE_NOEXCEPT
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

void Application::createDepthResources() RELEASE_NOEXCEPT
{
	// Get the supported depth format
	const vk::Format depth_format = findDepthFormat();

	// Create the depth image
	createImage(
		m_swapChainExtent.width,
		m_swapChainExtent.height,
		1,
		m_msaaSamples,
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

void Application::createTextureImage() RELEASE_NOEXCEPT
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

	// Number of mip levels
	m_textureMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(tex_width, tex_height)))) + 1;

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
	createResultValue(result, "vk::Device::mapMemory");

	memcpy(mapped_data, pixels, static_cast<size_t>(image_size));

	m_logicalDevice.unmapMemory(staging_buffer_memory);
	
	// Free the memory used for the image
	stbi_image_free(static_cast<void *>(pixels));

	// Create the image
	createImage(
		static_cast<uint32_t>(tex_width), // Width
		static_cast<uint32_t>(tex_height), // Height
		m_textureMipLevels, // Mip levels
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
		m_textureMipLevels // Mip levels
	);

	// Copy the staging buffer data to the image
	copyBufferToImage(
		staging_buffer, // Src buffer
		m_textureImage, // Dst image
		static_cast<uint32_t>(tex_width), // Width
		static_cast<uint32_t>(tex_height) // Height
	);

	// Generate the mipmaps
	generateMipmaps(
		m_textureImage,
		vk::Format::eR8G8B8A8Srgb,
		tex_width,
		tex_height,
		m_textureMipLevels
	);

	// Transition the image for shader read-only access // Using mipmapping instead of just transitioning
	// transitionImageLayout(
	// 	m_textureImage,
	// 	vk::Format::eR8G8B8A8Srgb,
	// 	vk::ImageAspectFlagBits::eColor,
	// 	vk::ImageLayout::eTransferDstOptimal,
	// 	vk::ImageLayout::eShaderReadOnlyOptimal,
	// 	m_textureMipLevels
	// );

	// Memory cleanup
	m_logicalDevice.destroyBuffer(staging_buffer, nullptr);
	m_logicalDevice.freeMemory(staging_buffer_memory, nullptr);
}

void Application::createTextureImageView() RELEASE_NOEXCEPT
{
	m_textureImageView = createImageView(m_textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor, m_textureMipLevels);
}

void Application::createTextureSampler() RELEASE_NOEXCEPT
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
		static_cast<float>(m_textureMipLevels), // Max lod
		vk::BorderColor::eIntOpaqueBlack, // Boarder color: for ClampToBoarder address mode
		VK_FALSE // Unnormalized coordinates
	};

	// Create the texture sampler
	result = m_logicalDevice.createSampler(&sampler_create_info, nullptr, &m_textureSampler);
	createResultValue(result, "vk::Device::createSampler");
}

void Application::loadModel() RELEASE_NOEXCEPT
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

void Application::createVertexBuffer() RELEASE_NOEXCEPT
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

void Application::createIndexBuffer() RELEASE_NOEXCEPT
{
	createAndFillBuffer<Index>(
		m_indices.data(),
		m_indices.size(),
		vk::BufferUsageFlagBits::eIndexBuffer,
		m_indexBuffer,
		m_indexBufferMemory
	);
}

void Application::createUniformBuffers() RELEASE_NOEXCEPT
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

void Application::createDescriptorPool() RELEASE_NOEXCEPT
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
	result = m_logicalDevice.createDescriptorPool(&descriptor_pool_create_info, nullptr, &m_descriptorPool);
	createResultValue(result, "vk::Device::createDescriptorPool");
}

void Application::createDescriptorSets() RELEASE_NOEXCEPT
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
	createResultValue(result, "vk::Device::allocateDescriptorSets");

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

void Application::createCommandBuffers() RELEASE_NOEXCEPT
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
	createResultValue(result, "Device::allocateCommandBuffers");

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		const vk::CommandBufferBeginInfo command_buffer_begin_info{
			{}, // Flags
			nullptr // Inheritance info: which state to inherit from the calling primary command buffers (already are primary)
		};

		// Begin the command buffer
		result = m_commandBuffers[i].begin(&command_buffer_begin_info);
		createResultValue(result, "vk::CommandBuffer::begin");

		const vk::Offset2D render_area_offset{
			0, 
			0
		};

		// Defines where shader loads and stores will take place (matches the size of the attachments for the best performance)
		const vk::Rect2D render_area{
			render_area_offset, // Offset
			m_swapChainExtent // Extent
		};

		// The color to clear the screen to at the beginning of a frame
		const std::array<float, 4> clear_color_array{
			CLEAR_COLOR_R, // Red
			CLEAR_COLOR_G, // Green
			CLEAR_COLOR_B, // Blue
			CLEAR_COLOR_A // Alpha
		};

		// Wrapper over the pure clear colors
		const vk::ClearColorValue clear_color_value{
			clear_color_array
		};

		// Wrapper over the clear color value
		const vk::ClearValue clear_color{
			clear_color_value
		};

		// Wrapper over depth stencil clear values
		const vk::ClearDepthStencilValue clear_depth_stencil_value{
			1.0f, // Depth
			0 // Stencil
		};

		const vk::ClearValue clear_depth_stencil{
			clear_depth_stencil_value
		};

		// Create array of clear values
		const vk::ClearValue clear_values[CLEAR_VALUE_COUNT] = {
			std::move(clear_color),
			std::move(clear_depth_stencil)
		};

		// Command to render to the screen
		const vk::RenderPassBeginInfo render_pass_begin_info{
			m_renderPass, // Render pass
			m_swapChainFramebuffers[i], // Framebuffer
			render_area, // Render area
			CLEAR_VALUE_COUNT, // Clear value count
			clear_values // Clear values
		};

		const vk::SubpassBeginInfoKHR subpass_begin_info{
			vk::SubpassContents::eInline // Contents (inline; the render pass commands will be embedded in the primary command buffer itself)
		};

		// Begin the render pass
		m_commandBuffers[i].beginRenderPass2KHR(&render_pass_begin_info, &subpass_begin_info, m_dynamicLoader);
		
		// Bind the graphics pipeline with the command buffer
		m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline);

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
			m_pipelineLayout, // Layout: the layout that the descriptors are based on
			0, // First set: the index of the first descriptor set
			1, // Descriptor set count
			m_descriptorSets, // Descriptor sets
			0, // Dynamic offset count
			nullptr // Dynamic offsets
		);

		// DRAW THE GEOMETRY!!!!
		m_commandBuffers[i].drawIndexed(
			static_cast<uint32_t>(m_indices.size()), // Index count
			1, // Instance count: used for instanced rendering (not doing, so 1)
			0, // First index: a value offset into the index buffer
			0, // Vertex offset: an offset to add to the indices in the index buffer
			0 // First instance: an offset for instanced rendering; defines the lowest value of gl_InstanceIndex
		);

		// Information for ending the renderpass
		const vk::SubpassEndInfoKHR subpass_end_info; // No constuctor paramaters

		// End the render pass
		m_commandBuffers[i].endRenderPass2KHR(&subpass_end_info, m_dynamicLoader);

		// End the command buffer
		result = m_commandBuffers[i].end();
		createResultValue(result, "vk::CommandBuffer::end");
	}
}

void Application::createSyncObjects() RELEASE_NOEXCEPT
{
	vk::Result result;

	// Create the right amount of fences
	[[unlikely]] if (m_imagesInFlight == nullptr)
		m_imagesInFlight = new vk::Fence[m_swapChainImageCount]{nullptr}; // TODO: get rid of {nullptr}

	// Information for semaphore creation (currently there is basically no info required)
	const vk::SemaphoreCreateInfo semaphore_create_info{
		vk::SemaphoreCreateFlags{} // Flags
	};

	// Information for fence creation (currently there is basically no info required)
	const vk::FenceCreateInfo fence_create_info{
		vk::FenceCreateFlagBits::eSignaled // Flags (initalizes the flag as signaled)
	};

	// Create the semaphores
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		result = m_logicalDevice.createSemaphore(&semaphore_create_info, nullptr, &m_imageAvailableSemaphores[i]);
		createResultValue(result, "vk::Device::createSemaphore");

		result = m_logicalDevice.createSemaphore(&semaphore_create_info, nullptr, &m_renderFinishedSemaphores[i]);
		createResultValue(result, "vk::Device::createSemaphore");

		result = m_logicalDevice.createFence(&fence_create_info, nullptr, &m_inFlightFences[i]);
		createResultValue(result, "vk::Device::createFence");

	}
}

// Check if validation layers are supported
template <size_t S>
bool Application::areLayersSupported(const char * const * const layers) RELEASE_NOEXCEPT
{
    vk::Result result;
	vk::LayerProperties *layer_properties;
    uint32_t layer_property_count;

    result = vk::enumerateInstanceLayerProperties(&layer_property_count, nullptr);
    createResultValue(result, "vk::enumerateInstanceLayerProperties");

	layer_properties = new vk::LayerProperties[layer_property_count];
	result = vk::enumerateInstanceLayerProperties(&layer_property_count, layer_properties);
	createResultValue(result, "vk::enumerateInstanceLayerProperties");

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

std::vector<const char *> Application::getRequiredInstanceExtensions() RELEASE_NOEXCEPT
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
bool Application::physicalDeviceSupportsRequirements(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT
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

QueueFamilyIndices Application::getQueueFamilies(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT
{
	vk::Result result;
	QueueFamilyIndices indices;
	uint32_t queue_family_property_count;
	vk::QueueFamilyProperties2 *queue_family_properties; 

	physical_device.getQueueFamilyProperties2(&queue_family_property_count, nullptr);
	queue_family_properties = new vk::QueueFamilyProperties2[queue_family_property_count];
	physical_device.getQueueFamilyProperties2(&queue_family_property_count, queue_family_properties);

	for (uint32_t i = 0; i < queue_family_property_count; ++i) {
		vk::Bool32 surface_support;
		result = physical_device.getSurfaceSupportKHR(i, m_surface, &surface_support); // Check is GPU supports present family with given surface
		createResultValue(result, "vk::PhysicalDevice::getSurfaceSupportKHR");

		[[unlikely]] if (queue_family_properties[i].queueFamilyProperties.queueCount == 0)
			continue;

		if (queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;
		
		if (queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer && !(queue_family_properties[i].queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics))
			indices.transferFamily = i;

		if (surface_support)
			indices.presentFamily = i;
		
		[[unlikely]] if (indices.isComplete()) {
			delete[] queue_family_properties;
			return indices;
		}
	}

	if (indices.graphicsFamily.has_value() && !(indices.transferFamily.has_value()))
        indices.transferFamily = indices.graphicsFamily;

	delete[] queue_family_properties;
	return indices;
}

bool Application::physicalDeviceSupportsExtensions(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT
{
    vk::Result result;
	vk::ExtensionProperties *extension_properties;
    uint32_t extension_property_count;

    result = physical_device.enumerateDeviceExtensionProperties(nullptr, &extension_property_count, nullptr);
    createResultValue(result, "vk::PhysicalDevice::enumerateDeviceExtensionProperties");
	
	extension_properties = new vk::ExtensionProperties[extension_property_count];
	result = physical_device.enumerateDeviceExtensionProperties(nullptr, &extension_property_count, extension_properties);
	createResultValue(result, "vk::PhysicalDevice::enumerateDeviceExtensionProperties");
	
	std::unordered_set<std::string_view> required_extensions(&m_deviceExtensions[0], &m_deviceExtensions[NUM_DEVICE_EXTENSIONS - 1]);

	for (uint32_t i = 0; i < extension_property_count; ++i)
		required_extensions.erase(static_cast<std::string_view>(extension_properties[i].extensionName));
	
	delete[] extension_properties;
	return required_extensions.empty();
}

SwapChainSupportDetails Application::querySwapChainSupport(const vk::PhysicalDevice &physical_device) RELEASE_NOEXCEPT
{
	vk::Result result;
	SwapChainSupportDetails support_details;

	const vk::PhysicalDeviceSurfaceInfo2KHR surface_info{
		m_surface // Surface
	};

	result = physical_device.getSurfaceCapabilities2KHR(&surface_info, &support_details.capabilities);
	createResultValue(result, "vk::PhysicalDevice::getSurfaceCapabilities2KHR");

	result = physical_device.getSurfaceFormats2KHR(&surface_info, &support_details.formatCount, nullptr);
	createResultValue(result, "vk::PhysicalDevice::getSurfaceFormats2KHR");

	support_details.formats = new vk::SurfaceFormat2KHR[support_details.formatCount];
	result = physical_device.getSurfaceFormats2KHR(&surface_info, &support_details.formatCount, support_details.formats);
	createResultValue(result, "vk::PhysicalDevice::getSurfaceFormats2KHR");

	result = physical_device.getSurfacePresentModesKHR(m_surface, &support_details.presentModeCount, nullptr);
	createResultValue(result, "vk::PhysicalDevice::getSurfacePresentModesKHR");

	support_details.presentModes = new vk::PresentModeKHR[support_details.presentModeCount];
	result = physical_device.getSurfacePresentModesKHR(m_surface, &support_details.presentModeCount, support_details.presentModes);
	createResultValue(result, "vk::PhysicalDevice::getSurfacePresentModesKHR");

	return support_details;
}

bool Application::swapChainAdequate(const SwapChainSupportDetails &swap_chain) RELEASE_NOEXCEPT
{
	return swap_chain.formatCount > 0 && swap_chain.presentModeCount > 0;
}

vk::SurfaceFormat2KHR Application::chooseSwapSurfaceFormat(const vk::SurfaceFormat2KHR * const formats, const uint32_t count) RELEASE_NOEXCEPT
{
	for (uint32_t i = 0; i < count; ++i)
		if (formats[i].surfaceFormat.format == vk::Format::eB8G8R8Srgb && // Formats pixels as: B, G, R, A; 8 bits each
			formats[i].surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) // Is Srbg format supported
			return formats[i];
	
	return formats[0]; // Could rank formats
}

vk::PresentModeKHR Application::chooseSwapPresentMode(const vk::PresentModeKHR * const present_modes, const uint32_t count) RELEASE_NOEXCEPT
{
	for (uint32_t i = 0; i < count; ++i)
		if (present_modes[i] == vk::PresentModeKHR::eMailbox) // Triple Buffering (great preformance, no tearing, high energy usage)
			return vk::PresentModeKHR::eMailbox;

	for (uint32_t i = 0; i < count; ++i)
		if (present_modes[i] == vk::PresentModeKHR::eFifo) // Standard double buffering (good preformance, no tearing, average energy usage)
			return vk::PresentModeKHR::eFifo;

	// Guaranteed to be avalible
	return vk::PresentModeKHR::eImmediate; // Images are immediatly transfered to the screen (great preformanace, possible tearing, averge energy usage)
}

vk::Extent2D Application::chooseSwapExtent(const vk::SurfaceCapabilities2KHR &capabilities) RELEASE_NOEXCEPT
{
	if (capabilities.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.surfaceCapabilities.currentExtent; // Width and height are already set
	
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	// The max and min functions are used here to clamp 
	// the value of WIDTH and HEIGHT between the allowed 
	// minimum and maximum extents that are supported by the implementation
	const vk::Extent2D extent{
		std::clamp(static_cast<uint32_t>(width),  capabilities.surfaceCapabilities.minImageExtent.width,  capabilities.surfaceCapabilities.maxImageExtent.width),
		std::clamp(static_cast<uint32_t>(height), capabilities.surfaceCapabilities.minImageExtent.height, capabilities.surfaceCapabilities.maxImageExtent.height)
	};

	return extent;
}

std::vector<char> Application::readFile(const std::string_view &&filename) RELEASE_NOEXCEPT
{
	std::ifstream file(
		filename,           // Open the given file
		std::ios::ate |     // Start reading at the end of the file
		std::ios::binary    // Read the file as binary file (avoid text transformations)
	);

	assert(file.is_open() && "ASSERT: Failed to open file!");

	const std::streampos file_size = file.tellg();
	std::vector<char> buffer(static_cast<size_t>(file_size));

	file.seekg(0);
	file.read(buffer.data(), file_size);

	return buffer;
}

vk::ShaderModule Application::createShaderModule(const std::vector<char> &code) RELEASE_NOEXCEPT
{
	vk::Result result;

	const vk::ShaderModuleCreateInfo shader_module_create_info{
		{}, // Flags
		code.size(), // Size of the code (in bytes)
		reinterpret_cast<const uint32_t *>(code.data()) // The buffer of data
	};

	vk::ShaderModule shader_module;
	result = m_logicalDevice.createShaderModule(&shader_module_create_info, nullptr, &shader_module);
	createResultValue(result, "vk::Device::createShaderModule");

	return shader_module;
}

uint32_t Application::findMemoryType(const uint32_t type_filter, const vk::MemoryPropertyFlags required_properties) RELEASE_NOEXCEPT // HERE
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

vk::Format Application::findSupportedFormat(const vk::Format * const candidate_formats, const uint32_t count, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) RELEASE_NOEXCEPT
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

vk::Format Application::findDepthFormat() RELEASE_NOEXCEPT
{
	const vk::Format requested_formats[DEPTH_FORMAT_COUNT] = {
		vk::Format::eD32Sfloat,
		vk::Format::eD32SfloatS8Uint,
		vk::Format::eD24UnormS8Uint
	};

	const vk::Format format = findSupportedFormat(
		requested_formats,
		DEPTH_FORMAT_COUNT,
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);

	return format;
}

bool Application::hasStencilComponent(const vk::Format format) RELEASE_NOEXCEPT
{
	return format != vk::Format::eD32Sfloat;
}

vk::SampleCountFlagBits Application::getMaxUsableSampleCount() RELEASE_NOEXCEPT
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

vk::ImageView Application::createImageView(const vk::Image image, const vk::Format format, const vk::ImageAspectFlags aspect_flags, const uint32_t mip_levels) RELEASE_NOEXCEPT
{
	vk::Result result;

	// Allows you to swizzle the color channels around (Identity for default mapping)
	const vk::ComponentMapping image_view_color_components{
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
	result = m_logicalDevice.createImageView(&image_view_create_info, nullptr, &image_view);
	createResultValue(result, "vk::Device::createImageView");

	return image_view;
}

void Application::run() RELEASE_NOEXCEPT
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
	createResultValue(result, "vk::Device::waitIdle");
}

void Application::printFPS() RELEASE_NOEXCEPT
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

void Application::drawFrame() RELEASE_NOEXCEPT
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
	createResultValue(result, "Application::drawFrame");

	// Index refers to the vk::Image in the swap chain images array
	uint32_t image_index;

	// Get the next image in the swap chain (using old method because newer method considers eErrorOutOfDateKHR as an exception)
	result = m_logicalDevice.acquireNextImageKHR(
		m_swapChain,
		std::numeric_limits<uint64_t>::max(),
		m_imageAvailableSemaphores[m_currentFrame],
		nullptr,
		&image_index
	);

	createResultValue(result, "Application::drawFrame::acquireNextImageKHR", {vk::Result::eSuccess, vk::Result::eSuboptimalKHR, vk::Result::eErrorOutOfDateKHR});

	// Only recreate swap chain if out of date; because we already have the next image
	[[unlikely]] if (result == vk::Result::eErrorOutOfDateKHR) {
		m_framebufferResized = false;
		recreateSwapChain();
		return;
	}

	// Check if a previous frame is using this image; wait on it's fence
	if (static_cast<bool>(m_imagesInFlight[image_index])) {
		result = m_logicalDevice.waitForFences(1, &m_imagesInFlight[image_index], VK_TRUE, std::numeric_limits<uint64_t>::max());
		createResultValue(result, "vk::Device::waitForFences");
	}
	
	// Mark image as now being in use by this frame
	m_imagesInFlight[image_index] = m_inFlightFences[m_currentFrame];

	// Update uniform buffer
	updateUniformBuffer(image_index);

	const vk::Semaphore wait_semaphores[DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT] = {
		m_imageAvailableSemaphores[m_currentFrame]
	};

	const vk::PipelineStageFlags wait_stages[DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT] = {
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
	createResultValue(result, "vk::Device::resetFences");

	// Submit the command buffer to the graphics queue
	result = m_graphicsQueue.submit(1, &submit_info, m_inFlightFences[m_currentFrame]);
	createResultValue(result, "vk::Queue::submit");

	// Swap chains
	const vk::SwapchainKHR swap_chains[PRESENT_INFO_SWAP_CHAIN_COUNT] = {
		m_swapChain
	};

	// Presentation configurations
	const vk::PresentInfoKHR present_info{
		DRAW_FRAME_SUBMIT_INFO_WAIT_SEMAPHORE_COUNT, // Wait semaphore count
		signal_semaphores, // Wait semaphores (using render_finished not image_available)
		PRESENT_INFO_SWAP_CHAIN_COUNT, // Swap chain count
		swap_chains, // Swap chains
		&image_index, // Image indices
		nullptr // Results: specify an array of vk::Result values to check for every individual swap chain if presentation was successful (not nessassary if only using 1 swap chain)
	};

	// Request the presentation of the image
	result = m_presentQueue.presentKHR(&present_info);

	// Check result value for success
	createResultValue(result, "Application::drawFrame::presentKHR", {vk::Result::eSuccess, vk::Result::eSuboptimalKHR, vk::Result::eErrorOutOfDateKHR});

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

void Application::destroySwapChain() RELEASE_NOEXCEPT
{
	m_logicalDevice.freeCommandBuffers(m_graphicsCommandPool, m_swapChainImageCount, m_commandBuffers); // Free command buffers instead of destroying command pool
	m_logicalDevice.destroyDescriptorPool(m_descriptorPool, nullptr);

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i) {
		m_logicalDevice.destroyBuffer(m_uniformBuffers[i], nullptr);
		m_logicalDevice.freeMemory(m_uniformBuffersMemory[i], nullptr);
	}

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i)
		m_logicalDevice.destroyFramebuffer(m_swapChainFramebuffers[i], nullptr);

	m_logicalDevice.destroyImageView(m_depthImageView, nullptr);
	m_logicalDevice.destroyImage(m_depthImage, nullptr);
	m_logicalDevice.freeMemory(m_depthImageMemory, nullptr);

	m_logicalDevice.destroyImageView(m_colorImageView, nullptr);
	m_logicalDevice.destroyImage(m_colorImage, nullptr);
	m_logicalDevice.freeMemory(m_colorImageMemory, nullptr);

	m_logicalDevice.destroyPipeline(m_graphicsPipeline, nullptr);
	m_logicalDevice.destroyPipelineLayout(m_pipelineLayout, nullptr);
	m_logicalDevice.destroyRenderPass(m_renderPass, nullptr);

	for (uint32_t i = 0; i < m_swapChainImageCount; ++i)
		m_logicalDevice.destroyImageView(m_swapChainImageViews[i], nullptr);

	m_logicalDevice.destroySwapchainKHR(m_swapChain, nullptr);
}

void Application::recreateSwapChain() RELEASE_NOEXCEPT
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
	createResultValue(result, "vk::Device::waitIdle");

	// Destroy the previous swap chain
	destroySwapChain();

	// Create the new swap chain
	createSwapChain();

	// Call creation functions that depend on the swap chain
	createSwapChainImageViews(); // Based directly on the swap chain images
	createRenderPass(); // Depends on the format of the swap chain images
	createGraphicsPipeline(); // Viewport and scissor rectangle size is specified (possible to avoid by using dynamic state)
	createColorResources(); // Depends on the swap chain extent
	createDepthResources(); // Depends on the swap chain extent
	createFrameBuffers(); // Directly depends on the swap chain images
	createUniformBuffers(); // Directly depends on the swap chain images
	createDescriptorPool(); // Directly depends on the swap chain images
	createDescriptorSets(); // Directly depends on the swap chain images
	createCommandBuffers(); // Directly depends on the swap chain images
}

template <class T>
void Application::createAndFillBuffer(const T * const buffer_data, const size_t count, const vk::BufferUsageFlags usage, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) RELEASE_NOEXCEPT
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
	
	createResultValue(result, "vk::Device::mapMemory");

	// Copy the staging data into the mapped memory
	memcpy(mapped_data, buffer_data, static_cast<size_t>(buffer_size));

	// Unmpa the data
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
	m_logicalDevice.destroyBuffer(staging_buffer, nullptr);
	m_logicalDevice.freeMemory(staging_buffer_memory, nullptr);
}

void Application::createBuffer(const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory) RELEASE_NOEXCEPT
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
		{}, // Sharing mode: whether the buffer is used by one queue family at a time or shared between multiple concurrently
		{}, // Queue family index count
		buffer_queue_family_indices // Queue family indices
	};

	// Check if queue familes are different
	if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.transferFamily.value()) {
		buffer_create_info.sharingMode = vk::SharingMode::eExclusive;
		buffer_create_info.queueFamilyIndexCount = 1;
	} else {
		buffer_create_info.sharingMode = vk::SharingMode::eConcurrent;
		buffer_create_info.queueFamilyIndexCount = 2;
	}

	// Creating the buffer
	result = m_logicalDevice.createBuffer(&buffer_create_info, nullptr, &buffer);
	createResultValue(result, "vk::Device::createBuffer");

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
	result = m_logicalDevice.allocateMemory(&buffer_memory_allocate_info, nullptr, &buffer_memory);
	createResultValue(result, "vk::Device::allocateMemory");

	// Bind the allocated memory with the buffer
	const vk::BindBufferMemoryInfo bind_buffer_memory_info{
		buffer, // Buffer
		buffer_memory, // Memory
		0 // Memory offset
	};

	result = m_logicalDevice.bindBufferMemory2(1, &bind_buffer_memory_info); // The 3rd parameter is the offset within the reigon of memory
	createResultValue(result, "vk::Device::bindBufferMemory2");
}

void Application::copyBuffer(const vk::Buffer &src_buffer, const vk::Buffer &dst_buffer, const vk::DeviceSize size) RELEASE_NOEXCEPT
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

void Application::updateUniformBuffer(const uint32_t current_image) RELEASE_NOEXCEPT
{
	vk::Result result;

	// Deltatime
	static std::chrono::steady_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	const std::chrono::steady_clock::time_point current_time = std::chrono::high_resolution_clock::now();
	const float duration = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

	// Uniform buffer object
	UniformBufferObject UBO{
		.model = glm::rotate(glm::mat4(1.0f), duration * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // Rotate around the Z-axis at 90 degrees/second
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
		&mapped_data // Pointer to mapped memory
	);

	createResultValue(result, "vk::Device::mapMemory");

	// Copy the uniform data into the mapped memory
	memcpy(mapped_data, &UBO, sizeof(UBO));

	// Unmpa the data
	m_logicalDevice.unmapMemory(m_uniformBuffersMemory[current_image]);
}

void Application::createImage(const uint32_t width, const uint32_t height, const uint32_t mip_levels, const vk::SampleCountFlagBits sample_count, const vk::Format format, const vk::ImageTiling tiling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Image &image, vk::DeviceMemory &image_memory) RELEASE_NOEXCEPT
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
		{}, // Sharing mode
		{}, // Queue family index count
		image_queue_family_indices, // Queue family indices
		vk::ImageLayout::eUndefined // Initial layout
	};

	// Using the right amount of queue families
	if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.transferFamily.value()) {
		image_create_info.sharingMode = vk::SharingMode::eExclusive;
		image_create_info.queueFamilyIndexCount = 1;
	} else {
		image_create_info.sharingMode = vk::SharingMode::eConcurrent;
		image_create_info.queueFamilyIndexCount = 2;
	}

	// Creating the image
	result = m_logicalDevice.createImage(&image_create_info, nullptr, &image);
	createResultValue(result, "vk::Device::createImage");

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
			nullptr
		};

		vk::MemoryAllocateInfo memory_allocate_info{
			image_memory_requirements.memoryRequirements.size,
			findMemoryType(image_memory_requirements.memoryRequirements.memoryTypeBits, properties)
		};

		memory_allocate_info.pNext = &dedicated_allocate_info;

		result = m_logicalDevice.allocateMemory(&memory_allocate_info, nullptr, &image_memory);
		createResultValue(result, "vk::Device::allocateMemory");

		const vk::BindImageMemoryInfo bind_image_memory_info{
			image, 
			image_memory,
			0
		};

		result = m_logicalDevice.bindImageMemory2(1, &bind_image_memory_info);
		createResultValue(result, "vk::Device::bindImageMemory2");
		return;
	}

	// Information for allocating memory
	const vk::MemoryAllocateInfo memory_allocate_info{
		image_memory_requirements.memoryRequirements.size,
		findMemoryType(image_memory_requirements.memoryRequirements.memoryTypeBits, properties)
	};

	// Allocate memory for the texture buffer
	result = m_logicalDevice.allocateMemory(&memory_allocate_info, nullptr, &image_memory);
	createResultValue(result, "vk::Device::allocateMemory");

	// Bind the memory to the buffer handle
	const vk::BindImageMemoryInfo bind_image_memory_info{
		image, // Image
		image_memory, // Memory
		0 // Memory offset
	};

	result = m_logicalDevice.bindImageMemory2(1, &bind_image_memory_info);
	createResultValue(result, "vk::Device::bindImageMemory2");
}

void Application::transitionImageLayout(const vk::Image image, const vk::Format /* format */, const vk::ImageAspectFlags image_aspect_flags, const vk::ImageLayout old_layout, const vk::ImageLayout new_layout, const uint32_t mip_levels) RELEASE_NOEXCEPT
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
		destination_stage = vk::PipelineStageFlagBits::eFragmentShader; // Image will be read be the fragemnt shader
	} 
	else if (old_layout == vk::ImageLayout::eUndefined && new_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		image_memory_barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
		destination_stage = vk::PipelineStageFlagBits::eEarlyFragmentTests; // Image will be read be the fragemnt shader
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

void Application::copyBufferToImage(const vk::Buffer src_buffer, const vk::Image dst_image, const uint32_t width, const uint32_t height) RELEASE_NOEXCEPT
{
	const vk::ImageSubresourceLayers image_subresource{
		vk::ImageAspectFlagBits::eColor, // Aspect mask
		0, // Mip level
		0, // Base array layer
		1 // Layer count
	};

	const vk::Offset3D image_offset{
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

void Application::generateMipmaps(const vk::Image image, const vk::Format image_format, int tex_width, int tex_height, const uint32_t mip_levels) RELEASE_NOEXCEPT
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

vk::CommandBuffer Application::beginSingleTimeCommand(const bool need_graphics) RELEASE_NOEXCEPT
{
	vk::Result result;

	const vk::CommandBufferAllocateInfo command_buffer_allocate_info{
		need_graphics ? m_graphicsCommandPool : m_transferCommandPool,
		vk::CommandBufferLevel::ePrimary,
		1
	};

	vk::CommandBuffer command_buffer;
	result = m_logicalDevice.allocateCommandBuffers(&command_buffer_allocate_info, &command_buffer);
	createResultValue(result, "vk::Device::allocateCommandBuffers");

	const vk::CommandBufferBeginInfo command_buffer_begin_info{
		vk::CommandBufferUsageFlagBits::eOneTimeSubmit // Only submitting to the comamnd buffer once
	};

	result = command_buffer.begin(&command_buffer_begin_info);
	createResultValue(result, "vk::CommandBuffer::begin");

	return command_buffer;
}

void Application::endSingleTimeCommand(const vk::CommandBuffer command_buffer, const bool need_graphics) RELEASE_NOEXCEPT
{
	vk::Result result;

	result = command_buffer.end();
	createResultValue(result, "vk::CommandBuffer::end");

	const vk::SubmitInfo submit_info{
		0,
		nullptr,
		nullptr,
		1,
		&command_buffer
	};

	[[unlikely]] if (need_graphics) {
		result = m_graphicsQueue.submit(1, &submit_info, nullptr);
		createResultValue(result, "vk::Queue::submit");

		result = m_graphicsQueue.waitIdle();
		createResultValue(result, "vk::Queue::waitIdle");

		m_logicalDevice.freeCommandBuffers(m_graphicsCommandPool, 1, &command_buffer);
		return;
	}

	result = m_transferQueue.submit(1, &submit_info, nullptr);
	createResultValue(result, "vk::Queue::submit");

	result = m_transferQueue.waitIdle();
	createResultValue(result, "vk::Queue::waitIdle");

	m_logicalDevice.freeCommandBuffers(m_transferCommandPool, 1, &command_buffer);
}

void Application::setFramebufferResized() RELEASE_NOEXCEPT
{
	m_framebufferResized = true;
}
