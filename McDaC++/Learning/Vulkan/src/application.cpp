#include "application.hpp"

#include <iostream>
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

#define APP_VERSION VK_MAKE_VERSION(1, 0, 0)
#define ENGINE_VERSION VK_MAKE_VERSION(1, 0, 0)
#define VK_VERSION VK_MAKE_VERSION(1, 2, 162)

#define DEBUG_LOG(x) std::cout << "DEBUG_LOG: " << x << '\n';
#define DEBUG_CALLBACK(o, s, t, d) std::c ## o 	<< "Debug Callback\n"\
												<< "\tServerity: " << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(s))\
												<< "\n\tType: "    << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(t))\
												<< "\n\tMessage: \n\t\t" << d << '\n'


constexpr static const std::array<const char * const, VK_NUM_LAYERS> g_validationLayers = {
    VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME
}; // Enables basic validation

constexpr static const std::array<const char * const, VK_NUM_DEVICE_EXTENSIONS> g_deviceExtensions = {
#ifdef __APPLE__
    VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, // Needed for apple devices
#endif
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
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
    std::cerr << "GLFW Error has occured!\n\tError code: 0x" << std::hex << error_code 
              << std::dec << "\n\tError description: " << description << '\n';
}

Application Application::app; // application

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
    m_logicalDevice.destroy();
    m_instance.destroySurfaceKHR(m_surface);
    m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr, m_dynamicLoader);
    m_instance.destroy();
}

void Application::createWindow() 
{
    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Resizing requires special care
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Not using OpenGL

    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    glfwSetErrorCallback(glfwErrorCallback);
    assert(glfwVulkanSupported());
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
}

void Application::createInstance() 
{
    // Check validation layers
    assert(areLayersSupported(g_validationLayers)); // Used Validation layers are not supported

    // Get required extensions
    const std::vector<const char *> extensions = getRequiredInstanceExtensions();

    // Create instance info
    vk::InstanceCreateInfo create_info({}, &m_appInfo, static_cast<uint32_t>(g_validationLayers.size()), g_validationLayers.data(), static_cast<uint32_t>(extensions.size()), extensions.data());

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
    m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(
        {
            // CreateInfo
			{},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | 
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | 
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError, // Enable all serverities
            // vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | 
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | 
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
            vkDebugCallback,
            nullptr
        }, 
        nullptr, 
		m_dynamicLoader); // Extension functions need to be dynamically loaded
}

void Application::createSurface()
{
#ifdef __APPLE__
    m_surface = m_instance.createMetalSurfaceEXT({{}, glfwGetCocoaWindow(m_window)});
#else
    VkSurfaceKHR temp_surface;
    assert(glfwCreateWindowSurface(m_instance, m_window, nullptr, &temp_surface) == VK_SUCCESS); // Failed to create window surface
    m_surface = temp_surface;
#endif
}

void Application::getPhysicalDevice()
{
    // Get all physical devices (GPUs) in the cpmputer
    const std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();
    assert(!devices.empty()); // Failed to find GPU/s with Vulkan support

    for (const auto &device : devices) {
        if (deviceSupportsRequirements(device)) {
            m_physicalDevice = device;
            break;
        }
    }
    assert(static_cast<bool>(m_physicalDevice)); // Failed to find GPU with wanted requirements
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
    queue_create_infos.reserve(indices.queueCount());
    for (const auto &queue_family : queue_families)
        queue_create_infos.emplace_back(vk::DeviceQueueCreateInfo{{}, queue_family, indices.queueCount(), &queue_priority});

    m_logicalDevice = m_physicalDevice.createDevice(
        {
			{}, 
			static_cast<uint32_t>(queue_create_infos.size()), queue_create_infos.data(), 
			static_cast<uint32_t>(g_validationLayers.size()), g_validationLayers.data(),
			static_cast<uint32_t>(g_deviceExtensions.size()), g_deviceExtensions.data(), 
			&features
		}
    );
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
		m_surface,
		image_count,
		surface_format.format,
		surface_format.colorSpace,
		extent,
		1, // The amount of layers each image consists of
		vk::ImageUsageFlagBits::eColorAttachment // The kind of operations we'll use the images in the swap chain for (color attachment == render directly to them)
	);
	
	const QueueFamilyIndices indices = getQueueFamilies(m_physicalDevice);
	const uint32_t queue_family_indices[] = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};
	
	create_info.imageSharingMode = vk::SharingMode::eExclusive;
	if (indices.graphicsFamily != indices.presentFamily) {
		create_info.imageSharingMode = vk::SharingMode::eConcurrent;
		create_info.
	}
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
	std::set<std::string_view> required_extensions(g_deviceExtensions.begin(), g_deviceExtensions.end());

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

	// Guaranteed to be avalible
	return vk::PresentModeKHR::eFifo; // Standard double buffering (good preformance, no tearing, average energy usage)
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
		std::max(capabilities.minImageExtent.width,  std::min(capabilities.maxImageExtent.width,  static_cast<uint32_t>(width))),
		std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, static_cast<uint32_t>(height)))
	};
}

void Application::_run() 
{
    // Main game loop
    while (!glfwWindowShouldClose(m_window)) {
        // glfwSwapBuffers(m_window);
	    glfwPollEvents();
    }
}

void Application::_printRequiredExtensions() 
{
    const std::vector<const char *> extensions = getRequiredInstanceExtensions();

    std::cout << "Required Vulkan Extensions:\n";
    for (const auto &extension : extensions)
        std::cout << '\t' << extension << '\n';
}

void Application::_printAllExtensions() 
{
    const std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();

    std::cout << "All Vulkan Extensions:\n";
    for (const auto &extension : extensions)
        std::cout << '\t' << static_cast<const char *>(extension.extensionName) << ", Version " << extension.specVersion << '\n';
}

void Application::_printLayerSupport() 
{
    const std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();

    std::cout << "Vulkan Layers:\n";
    for (const auto &layer : layers)
        std::cout << '\t' << static_cast<const char *>(layer.layerName) << ", Version " << layer.specVersion 
        << "\n\t\t" << static_cast<const char *>(layer.description) << '\n';
}
