#include "application.hpp"

#include <iostream>
#include <set>

#define VK_NUM_LAYERS 1
#define VK_NUM_DEVICE_EXTENSIONS 1

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SWAP_INTERVAL 1

#define APP_NAME "LearnVulkan"
#define ENGINE_NAME "My Engine"

#define APP_VERSION VK_MAKE_VERSION(1, 0, 0)
#define ENGINE_VERSION VK_MAKE_VERSION(1, 0, 0)
#define VK_VERSION VK_MAKE_VERSION(1, 2, 162)

#define DEBUG_LOG(x) std::cout << "DEBUG_LOG: " << x << '\n';


constexpr static const std::array<const char * const, VK_NUM_LAYERS> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
}; // Enables basic validation

static VKAPI_ATTR vk::Bool32 VKAPI_CALL vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    [[maybe_unused]] void *pUserData)
{
    if (static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity) == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError) {
        std::cerr << "Debug Callback:\n" 
                  << "\tServerity: " << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) 
                  << "\n\tType: " << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageType))
                  << "\n\tMessage: \n\t\t" << pCallbackData->pMessage << '\n';
    } else {
        std::cout << "Debug Callback:\n" 
                  << "\tServerity: " << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) 
                  << "\n\tType: " << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageType))
                  << "\n\tMessage: \n\t\t" << pCallbackData->pMessage << '\n';
    }

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
    createWindow();
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

    glfwSwapInterval(SWAP_INTERVAL); // Enable/Disable vsync (wait to swap buffers)
    glfwSetErrorCallback(glfwErrorCallback);

    assert(glfwVulkanSupported());
}

void Application::initVulkan() 
{
    // Functions HAVE to be called in this order
    createInstance();
    createDebugCallback();
    createSurface();
    getPhysicalDevice();
    createLogicalDevice();
}

void Application::createInstance() 
{
    // Check validation layers
    assert(areLayersSupported(validation_layers)); // Used Validation layers are not supported

    // Get required extensions
    const std::vector<const char *> extensions = getRequiredExtensions();

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

    // Create instance info
    vk::InstanceCreateInfo create_info({}, &m_appInfo, static_cast<uint32_t>(validation_layers.size()), validation_layers.data(), static_cast<uint32_t>(extensions.size()), extensions.data());
    create_info.pNext = static_cast<const void *>(&debug_create_info);

    // Create instance
    m_instance = vk::createInstance(create_info);
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
        nullptr, m_dynamicLoader);
}

void Application::createSurface()
{
#ifdef __APPLE__
    const id window = glfwGetCocoaWindow(m_window);
    m_surface = m_instance.createMetalSurfaceEXT({{}, window});
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

    constexpr const std::array<const char * const, VK_NUM_DEVICE_EXTENSIONS> extensions = {
        "VK_KHR_portability_subset" // Needed for MacOS
    };

    m_logicalDevice = m_physicalDevice.createDevice(
        {{}, static_cast<uint32_t>(queue_create_infos.size()), queue_create_infos.data(), 
        static_cast<uint32_t>(validation_layers.size()), validation_layers.data(),
        static_cast<uint32_t>(extensions.size()), extensions.data(), &features}
    );
    m_dynamicLoader.init(m_logicalDevice);

    m_graphicsQueue = m_logicalDevice.getQueue(indices.graphicsFamily.value(), 0);
    m_presentQueue = m_logicalDevice.getQueue(indices.presentFamily.value(), 0); // TODO Get back to this index later
}

// Check if validation layers are supported
template <size_t S>
bool Application::areLayersSupported(const std::array<const char * const, S> &layers) 
{
    const std::vector<vk::LayerProperties> properties = vk::enumerateInstanceLayerProperties();
    bool found;
    for (const auto &layer : layers) {
        found = false;
        for (const auto &property : properties) {
            if (!strcmp(layer, static_cast<const char * const>(property.layerName))) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

std::vector<const char *> Application::getRequiredExtensions() 
{
    uint32_t glfw_extension_count;
    const char * const * const glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count); // Get required vulkan extensions for glfw
    std::vector<const char *> extensions = {glfw_extensions, glfw_extensions + glfw_extension_count};

    extensions.reserve(extensions.size() + 2);
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    return extensions;
}

// Many different ways of selecting a suitable GPU
bool Application::deviceSupportsRequirements(const vk::PhysicalDevice &device)
{
    // const vk::PhysicalDeviceProperties properties = device.getProperties();
    // const vk::PhysicalDeviceFeatures features = device.getFeatures();

    // const QueueFamilyIndices indices = getQueueFamilies();
    return getQueueFamilies(device).isComplete();
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
    const std::vector<const char *> extensions = getRequiredExtensions();

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
    const std::vector<vk::LayerProperties> properties = vk::enumerateInstanceLayerProperties();

    std::cout << "Vulkan Layers:\n";
    for (const auto &property : properties)
        std::cout << '\t' << static_cast<const char *>(property.layerName) << ", Version " << property.specVersion 
        << "\n\t\t" << static_cast<const char *>(property.description) << '\n';
}
