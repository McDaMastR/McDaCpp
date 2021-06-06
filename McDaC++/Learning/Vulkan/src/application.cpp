#include "application.hpp"

#include <iostream>
#include <fstream>
#include <set>

#ifdef DEBUG
	#define VK_NUM_LAYERS 1
#else
	#define VK_NUM_LAYERS 0
#endif

#ifdef __APPLE__
	#define VK_NUM_DEVICE_EXTENSIONS 2
#else
	#define VK_NUM_DEVICE_EXTENSIONS 1
#endif

#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#define VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define APP_NAME "LearnVulkan"
#define ENGINE_NAME "My Engine"

#define APP_VERSION VK_MAKE_VERSION(0, 1, 0)
#define ENGINE_VERSION VK_MAKE_VERSION(0, 1, 0)
#define VK_VERSION VK_MAKE_VERSION(1, 2, 162)

#define DEBUG_LOG(x) std::cout << "DEBUG_LOG: " << x << '\n';
#define DEBUG_CALLBACK(o, s, t, d) std::c ## o 	<< "Debug Callback:\n"\
												<< "\tServerity: " << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(s))\
												<< "\n\tType: "    << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(t))\
												<< "\n\tMessage: \n\t\t" << d << '\n'


#ifdef DEBUG
	constexpr static const char * const g_validationLayers[VK_NUM_LAYERS] {
		VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME
	}; // Enables basic validation
#endif

constexpr static const char * const g_deviceExtensions[VK_NUM_DEVICE_EXTENSIONS] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef __APPLE__
	VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME // Needed for apple devices
#endif
};


static VKAPI_ATTR vk::Bool32 VKAPI_CALL vkDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	[[maybe_unused]] void *pUserData)
{
	if (static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity) == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
		DEBUG_CALLBACK(err, messageSeverity, messageType, pCallbackData->pMessage);
	else
		DEBUG_CALLBACK(out, messageSeverity, messageType, pCallbackData->pMessage);

	// Can pass data into pUserData if needed
	return VK_FALSE; // Do not abort program
}

static void glfwErrorCallback(int error_code, const char* description)
{
	std::cerr << "GLFW Error has occured!\n\tError code: 0x" << std::hex << error_code << std::dec 
			  << "\n\tError description: " << description << '\n';
}

Application::Application()  
	: m_appInfo(APP_NAME, APP_VERSION, ENGINE_NAME, ENGINE_VERSION, VK_VERSION), m_dynamicLoader(vkGetInstanceProcAddr)
{
	// GLFW
	createWindow();

	// Vulkan
	initVulkan();
}

Application::~Application()  
{
	// GLFW
	glfwTerminate();

	// Vulkan
	for (const auto &image_view : m_swapChainImageViews)
		m_logicalDevice.destroyImageView(image_view);

	m_logicalDevice.destroySwapchainKHR(m_swapChain);
	m_logicalDevice.destroy();
	m_instance.destroySurfaceKHR(m_surface);
	m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr, m_dynamicLoader);
	m_instance.destroy();
}

void Application::createWindow() 
{
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Resizing requires special care with the swapchain
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Not using OpenGL

	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	glfwSetErrorCallback(glfwErrorCallback);
	assert(glfwVulkanSupported() && "VULKAN ASSERT: Vulkan not supported on device!");
}

void Application::initVulkan() 
{
	// Functions HAVE to be called in this order
	createInstance(); // Everything relies on the instance
	createDebugCallback(); // Enable debugging ASAP
	createSurface(); // Most of graphics relies on the surface
	getPhysicalDevice(); // Need a GPU to render
	createLogicalDevice(); // Need to enable GPU
	createSwapChain(); // Need to render images to the screen
	createImageViews(); // Describes how to access the images in the swap chain
	createGraphicsPipeline(); // Creates the graphics pipeline
}

void Application::createInstance() 
{
	// Check validation layers
	assert(areLayersSupported(std::array<const char * const, VK_NUM_LAYERS>{g_validationLayers[0]}) && "VULKAN ASSERT: Requested validation layers are not supported");

	// Get required extensions
	const std::vector<const char *> extensions = getRequiredInstanceExtensions();

	// Create instance info
	vk::InstanceCreateInfo create_info({}, &m_appInfo, VK_NUM_LAYERS, g_validationLayers, static_cast<uint32_t>(extensions.size()), extensions.data());

	// Add debugging info
	const vk::DebugUtilsMessengerCreateInfoEXT debug_create_info(
		{}, 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		// vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | 
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | 
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		vkDebugCallback,
		nullptr);
	create_info.pNext = static_cast<const void *>(&debug_create_info);

	// Create instance
	m_instance = vk::createInstance(create_info);

	// Add more functions to dynamic loader
	m_dynamicLoader.init(m_instance);
}

void Application::createDebugCallback() 
{
	vk::DebugUtilsMessengerCreateInfoEXT create_info(
		{}, // Flags
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | // Diagnostic messages
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | // Informational messages like the creation of a resource
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | // Messages about behavior that is not necessarily an error, but very likely a bug in your application
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError, // Messages about behavior that is invalid and may cause crashes
		// vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | // Some event has happened that is unrelated to the specification or performance
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | // Something has happened that violates the specification or indicates a possible mistake
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance, // Potential non-optimal use of Vulkan
		vkDebugCallback, // Pointer to debug callback function
		nullptr // Pointer to data for callback function to fill (not being used)
	);

	m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(create_info, nullptr, m_dynamicLoader); // Extension functions need to be dynamically loaded
}

void Application::createSurface()
{
	VkSurfaceKHR temp_surface;
	assert(glfwCreateWindowSurface(m_instance, m_window, nullptr, &temp_surface) == VK_SUCCESS && "VULKAN ASSERT: Failed to create window surface!");
	m_surface = temp_surface;
}

void Application::getPhysicalDevice()
{
	// Get all physical devices (GPUs) in the cpmputer
	const std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();
	assert(!devices.empty() && "VULKAN ASSERT: Failed to find GPU/s with Vulkan support!");

	for (const auto &device : devices) {
		if (deviceSupportsRequirements(device)) {
			m_physicalDevice = device;
			break;
		}
	}
	assert(static_cast<bool>(m_physicalDevice) && "VULKAN ASSERT: Failed to find GPU with rquested requirements!");
}

void Application::createLogicalDevice()
{
	const QueueFamilyIndices indices = getQueueFamilies(m_physicalDevice);
	const vk::PhysicalDeviceFeatures features;

	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> queue_families = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	const float queue_priority = 1.0f;
	if (indices.graphicsFamily.value() == indices.presentFamily.value()) {
		for (const auto &queue_family : queue_families)
			queue_create_infos.emplace_back(vk::DeviceQueueCreateInfo{{}, queue_family, 1, &queue_priority});
	} else {
		queue_create_infos.reserve(2);
		for (const auto &queue_family : queue_families)
			queue_create_infos.emplace_back(vk::DeviceQueueCreateInfo{{}, queue_family, 2, &queue_priority});
	}

	const vk::DeviceCreateInfo create_info(
		{}, // Flags
		static_cast<uint32_t>(queue_create_infos.size()), queue_create_infos.data(), // Create infos for queues
#ifdef DEBUG
		VK_NUM_LAYERS, g_validationLayers, // Validation layers (when in debug)
#else
		0, nullptr, // No validation layers (when in release)
#endif
		VK_NUM_DEVICE_EXTENSIONS, g_deviceExtensions, // Required device extensions
		&features // Features of physical device
	);

	m_logicalDevice = m_physicalDevice.createDevice(create_info);
	m_dynamicLoader.init(m_logicalDevice); // Add more functions to dynamic loader

	m_graphicsQueue = m_logicalDevice.getQueue(indices.graphicsFamily.value(), 0);
	m_presentQueue = m_logicalDevice.getQueue(indices.presentFamily.value(), 0); // TODO Get back to this index later
}

void Application::createSwapChain()
{
	const SwapChainSupportDetails swap_chain_support = querySwapChainSupport(m_physicalDevice);

	const vk::SurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats);
	const vk::PresentModeKHR present_mode = chooseSwapPresentMode(swap_chain_support.presentModes);
	const vk::Extent2D extent = chooseSwapExtent(swap_chain_support.capabilities);

	uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1; // Amount of images to be stored in the queue (recommended to request at least 1 more than the minimum)

	if (swap_chain_support.capabilities.maxImageCount > 0 && // If maxImageCount == 0, then there is no maximum
		image_count > swap_chain_support.capabilities.maxImageCount) // Wanted queue size is larger than the maxmimum
		image_count = swap_chain_support.capabilities.maxImageCount;

	vk::SwapchainCreateInfoKHR create_info(
		{}, // Flags
		m_surface, // surface
		image_count, // min image count
		surface_format.format, // image format
		surface_format.colorSpace, // image color space
		extent, // image extent
		1, // The amount of layers each image consists of
		vk::ImageUsageFlagBits::eColorAttachment, // The kind of operations we'll use the images in the swap chain for (color attachment == render directly to them)
		vk::SharingMode::eExclusive, // Image sharing mode: An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family (offers the best performance)
		{}, // Queue family index count
		{}, // Queue family indices
		swap_chain_support.capabilities.currentTransform, // Pre transform (current transform means add no transform)
		vk::CompositeAlphaFlagBitsKHR::eOpaque, // For blending with other windows in the window system (Opaque means to ignore the alpha channel)
		present_mode, // Present mode
		VK_TRUE, // Clipped: true if you don't care about the color of pixels that are obscured, for example because another window is in front of them
		{} // Old swap chain: reference to old swap chain if current is invalid or unoptimized (e.g. because window is resized). No given reference means we'll only create one swap chain
	);
	
	// Queue families
	const QueueFamilyIndices indices = getQueueFamilies(m_physicalDevice);
	if (indices.graphicsFamily != indices.presentFamily) { // Use concurrent mode if queue families differ to avoid manual ownership transfer
		const uint32_t queue_family_indices[] = {
			indices.graphicsFamily.value(),
			indices.presentFamily.value()
		};

		create_info.imageSharingMode = vk::SharingMode::eConcurrent; // Images can be used across multiple queue families without explicit ownership transfers
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queue_family_indices;
	}

	m_swapChain = m_logicalDevice.createSwapchainKHR(create_info);
	m_swapChainImages = m_logicalDevice.getSwapchainImagesKHR(m_swapChain);
	m_swapChainImageFormat = surface_format.format;
	m_swapChainExtent = extent;
}

void Application::createImageViews()
{
	m_swapChainImageViews.reserve(m_swapChainImages.size());

	for (const auto &image : m_swapChainImages) {
		const vk::ImageViewCreateInfo image_view_create_info(
			{}, // Flags
			image, // Swap chain image
			vk::ImageViewType::e2D, // Treat images as 2D textures
			m_swapChainImageFormat, // Chosen image format
			{
				vk::ComponentSwizzle::eIdentity, // Red channel
				vk::ComponentSwizzle::eIdentity, // Green channel
				vk::ComponentSwizzle::eIdentity, // Blue channel
				vk::ComponentSwizzle::eIdentity // Alpha channel
			}, // Color components: allows you to swizzle the color channels around (Identity for default mapping)
			{
				vk::ImageAspectFlagBits::eColor, // Aspect mask (using color targets)
				0, // Base mip level (first mipmapping level)
				1, // Mip level amount (one mipmapping level)
				0, // Base array layer (first array layer)
				1 // Array layer amount (one array layer)
			} // Subresource range: describes what the image's purpose is and which parts of the image should be accessed
		);

		m_swapChainImageViews.emplace_back( m_logicalDevice.createImageView(image_view_create_info) );
	}
}

void Application::createGraphicsPipeline()
{
	const std::vector<char> vertex_shader_code = readFile(std::move("bin/vert.spv"));
	const std::vector<char> fragment_shader_code = readFile(std::move("bin/frag.spv"));

	const vk::ShaderModule vertex_shader_module = createShaderModule(vertex_shader_code);
	const vk::ShaderModule fragment_shader_module = createShaderModule(fragment_shader_code);

	const vk::PipelineShaderStageCreateInfo vertex_shader_stage_create_info(
		{}, // Flags
		vk::ShaderStageFlagBits::eVertex, // Shader Stage/Type (Vertex)
		vertex_shader_module, // Shader module containing the code
		"main", // Entrypoint of the shader (main function)
		{} // Specialization Info: specify values for shader constants (not using)
	);

	const vk::PipelineShaderStageCreateInfo fragment_shader_stage_create_info(
		{},
		vk::ShaderStageFlagBits::eFragment,
		fragment_shader_module,
		"main",
		{}
	);

	// Create an array containing the shader stage create infos
	const vk::PipelineShaderStageCreateInfo shader_stages[] = {
		vertex_shader_stage_create_info,
		fragment_shader_stage_create_info
	};

	// Describes the format of the vertex data that will be passed to the vertex shader
	const vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info(
		{}, // Flags
		0, // Vertex binding description count (no data being passed into vertex shader)
		nullptr, // Vertex binding descriptions: points to an array of structs that describe the details for loading vertex bindings (no data is being loaded)
		0, // Vertex attribute description count (no data being passed into vertex shader)
		nullptr // Vertex attribute descriptions: points to an array of structs that describe the details for loading vertex attributes (no data is being loaded)
	);

	// Describes what kind of geometry will be drawn from the vertices and if primitive restart should be enabled
	const vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_create_info(
		{}, // Flags
		vk::PrimitiveTopology::eTriangleList, // Topology: the kind of geometry that will be drawn from the vertices (1 triangle from every 3 vertices without reuse)
		VK_FALSE // Primitive restart: can break up lines and triangles in the strip topology modes (not using strip topology mode)
	);

	// Describes the region of the framebuffer that the output will be rendered to
	// This will almost always be (0, 0) to (width, height)
	const vk::Viewport viewport(
		0.0f, // X
		0.0f, // Y
		static_cast<float>(m_swapChainExtent.width), // Width
		static_cast<float>(m_swapChainExtent.height), // Height
		0.0f, // Min depth: the minimum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
		1.0f // Max depth: the maximum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
	);

	// Defines which regions pixels will actually be stored
	// Any pixels outside the scissor rectangles will be discarded by the rasterizer
	const vk::Rect2D scissor_rectangle(
		{0, 0}, // Offset (no offset)
		m_swapChainExtent // Extent (the chosen extent of the screen)
	);

	// Viewport state containing viewport and scissor rectangle
	const vk::PipelineViewportStateCreateInfo viewport_state_create_info(
		{}, // Flags
		1, // Viewport count: number of viewports (only using 1)
		&viewport, // Viewports: pointer to array of viewports being used (only using 1)
		1, // Scissor count: number of scissor rectangles (only using 1)
		&scissor_rectangle // Scissors: pointor to array of scissor rectangles being used (only using 1)
	);

	// Shader modules are no longer needed
	m_logicalDevice.destroyShaderModule(vertex_shader_module);
	m_logicalDevice.destroyShaderModule(fragment_shader_module);
}

// Check if validation layers are supported
template <size_t S>
bool Application::areLayersSupported(const std::array<const char * const, S> &needed_layers) 
{
	const std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();
	bool found;
	for (const auto &needed_layer : needed_layers) {
		found = false;
		for (const auto &layer : layers) {
			if (strcmp(needed_layer, static_cast<const char *>(layer.layerName)) == 0) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

std::vector<const char *> Application::getRequiredInstanceExtensions() 
{
	uint32_t glfw_extension_count;
	const char * const * const glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count); // Get required vulkan extensions for glfw
	std::vector<const char *> extensions = {glfw_extensions, glfw_extensions + glfw_extension_count};

	// extensions.reserve(extensions.size() + 1); // Not used if only 1 more extension is being used
	extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Includes debug_report and debug_marker
	return extensions;
}

// Many different ways of selecting a suitable GPU
bool Application::deviceSupportsRequirements(const vk::PhysicalDevice &device)
{
	// const vk::PhysicalDeviceProperties properties = device.getProperties();
	// const vk::PhysicalDeviceFeatures features = device.getFeatures();
	

	return 	getQueueFamilies(device).isComplete() && 
			deviceSupportsExtensions(device) && 
			swapChainAdequate(querySwapChainSupport(device));
}

// Maybe cache somewhere
QueueFamilyIndices Application::getQueueFamilies(const vk::PhysicalDevice &device)
{
	QueueFamilyIndices indices;
	const std::vector<vk::QueueFamilyProperties> queue_families = device.getQueueFamilyProperties();

	for (uint32_t i = 0; i < queue_families.size(); i++) {
		if (queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics)
			indices.graphicsFamily = i;
		if (device.getSurfaceSupportKHR(i, m_surface))
			indices.presentFamily = i;

		if (indices.isComplete())
			return indices;
	}

	return indices;
}

bool Application::deviceSupportsExtensions(const vk::PhysicalDevice &device)
{
	const std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties();
	std::set<std::string_view> required_extensions(&g_deviceExtensions[0], &g_deviceExtensions[VK_NUM_DEVICE_EXTENSIONS - 1]);

	for (const auto &extension : extensions)
		required_extensions.erase(static_cast<std::string_view>(extension.extensionName));
	
	return required_extensions.empty();
}

inline SwapChainSupportDetails Application::querySwapChainSupport(const vk::PhysicalDevice &device)
{
	return {
		.capabilities = device.getSurfaceCapabilitiesKHR(m_surface), 
		.formats 	  = device.getSurfaceFormatsKHR(m_surface),
		.presentModes = device.getSurfacePresentModesKHR(m_surface)
	};
}

inline bool Application::swapChainAdequate(const SwapChainSupportDetails &swap_chain)
{
	return !swap_chain.formats.empty() && !swap_chain.presentModes.empty();
}

vk::SurfaceFormatKHR Application::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &formats)
{
	for (const auto &format : formats)
		if (format.format == vk::Format::eB8G8R8Srgb && // Formats pixels as: B, G, R, A; 8 bits each
			format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) // Is Srbg format supported
			return format;
	
	return formats[0]; // Could rank formats
}

vk::PresentModeKHR Application::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &present_modes)
{
	for (const auto &mode : present_modes)
		if (mode == vk::PresentModeKHR::eMailbox) // Triple Buffering (great preformance, no tearing, high energy usage)
			return vk::PresentModeKHR::eMailbox;

	for (const auto &mode : present_modes)
		if (mode == vk::PresentModeKHR::eFifo) // Standard double buffering (good preformance, no tearing, average energy usage)
			return vk::PresentModeKHR::eFifo;

	// Guaranteed to be avalible
	return vk::PresentModeKHR::eImmediate; // Images are immediatly transfered to the screen (great preformanace, possible tearing, verge energy usage)
}

vk::Extent2D Application::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
		return capabilities.currentExtent; // Width and height are already set
	
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	// The max and min functions are used here to clamp 
	// the value of WIDTH and HEIGHT between the allowed 
	// minimum and maximum extents that are supported by the implementation
	return {
#if __cplusplus >= 201703L // C++17 or higher
		std::clamp(static_cast<uint32_t>(width),  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width),
		std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
#else
		std::max(capabilities.minImageExtent.width,  std::min(capabilities.maxImageExtent.width,  static_cast<uint32_t>(width))),
		std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, static_cast<uint32_t>(height)))
#endif
	};
}

std::vector<char> Application::readFile(const std::string_view &&filename)
{
	std::ifstream file(
		filename,           // Open the given file
		std::ios::ate |     // Start reading at the end of the file
		std::ios::binary    // Read the file as binary file (avoid text transformations)
	);

	assert(file.is_open() && "ASSERT: Failed to open file!");

	std::streampos file_size = file.tellg();
	std::vector<char> buffer(static_cast<size_t>(file_size));
	DEBUG_LOG("Shader File Size: " << file_size);

	file.seekg(0);
	file.read(buffer.data(), file_size);

	return buffer;
}

vk::ShaderModule Application::createShaderModule(const std::vector<char> &code)
{
	const vk::ShaderModuleCreateInfo create_info(
		{}, // Flags
		code.size(), // Size of the code (in bytes)
		reinterpret_cast<const uint32_t *>(code.data()) // The buffer of data
	);

	const vk::ShaderModule shader_module = m_logicalDevice.createShaderModule(create_info);
	return shader_module;
}

void Application::run() 
{
	// Main game loop
	while (!glfwWindowShouldClose(m_window)) {
		// glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void Application::printRequiredExtensions() 
{
	const std::vector<const char *> extensions = getRequiredInstanceExtensions();

	std::cout << "Required Vulkan Extensions:\n";
	for (const auto &extension : extensions)
		std::cout << '\t' << extension << '\n';
}

void Application::printAllExtensions() 
{
	const std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();

	std::cout << "All Vulkan Extensions:\n";
	for (const auto &extension : extensions)
		std::cout << '\t' << static_cast<const char *>(extension.extensionName) << ", Version " << extension.specVersion << '\n';
}

void Application::printLayerSupport() 
{
	const std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();

	std::cout << "Vulkan Layers:\n";
	for (const auto &layer : layers)
		std::cout << '\t' << static_cast<const char *>(layer.layerName) << ", Version " << layer.specVersion 
		<< "\n\t\t" << static_cast<const char *>(layer.description) << '\n';
}
