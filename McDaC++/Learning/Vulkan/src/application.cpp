#include "application.hpp"

#include <iostream>
#include <fstream>
#include <unordered_set>

#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"
#define VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

#define SWAPCHAIN_IMAGE_ARRAY_LAYER_COUNT 1
#define QUEUE_FAMILY_INDEX_COUNT 2

#define IMAGE_VIEW_BASE_MIP_LEVEL 0
#define IMAGE_VIEW_MIP_LEVEL_AMOUNT 1
#define IMAGE_VIEW_BASE_ARRAY_LAYER 0
#define IMAGE_VIEW_ARRAY_LAYER_AMOUNT 1

#define COLOR_ATTACHMENT_REFERENCE_INDEX 0
#define SUBPASS_INPUT_ATTACHMENT_COUNT 0
#define SUBPASS_COLOR_ATTACHMENT_COUNT 1
#define SUBPASS_PRESERVE_ATTACHMENT_COUNT 0
#define SUBPASS_DEPENDENCY_SRC_SUBPASS VK_SUBPASS_EXTERNAL
#define SUBPASS_DEPENDENCY_DST_SUBPASS 0
#define RENDER_PASS_ATTACHMENT_COUNT 1
#define RENDER_PASS_SUBPASS_COUNT 1
#define RENDER_PASS_DEPENDENCY_COUNT 1

#define PIPELINE_SHADER_STAGE_COUNT 2
#define PIPELINE_VERTEX_BINDING_DESCRIPTION_COUNT 0
#define PIPELINE_VERTEX_ATTRIBUTE_DESCRIPTION_COUNT 0
#define PIPELINE_VIEWPORT_X 0.0f
#define PIPELINE_VIEWPORT_Y 0.0f
#define PIPELINE_VIEWPORT_MIN_DEPTH 0.0f
#define PIPELINE_VIEWPORT_MAX_DEPTH 1.0f
#define PIPELINE_SCISSOR_OFFSET {0, 0}
#define PIPELINE_VIEWPORT_COUNT 1
#define PIPELINE_SCISSOR_COUNT 1
#define PIPELINE_RASTERIZATION_DEPTH_BIAS_CONSTANT_FACTOR 0.0f
#define PIPELINE_RASTERIZATION_DEPTH_BIAS_CLAMP 0.0f
#define PIPELINE_RASTERIZATION_DEPTH_BIAS_SLOPE_FACTOR 0.0f
#define PIPELINE_RASTERIZATION_LINE_WIDTH 1.0f
#define PIPELINE_MULTISAMPLE_MIN_SAMPLE_SHADING 1.0f
#define PIPELINE_COLOR_BLEND_ATTACHMENT_COUNT 1
#define PIPELINE_DYNAMIC_STATE_COUNT 2
#define PIPELINE_SET_LAYOUT_COUNT 0
#define PIPELINE_PUSH_CONSTANT_RANGE_COUNT 0

#define FRAMEBUFFER_ATTACHMENT_COUNT 1
#define FRAMEBUFFER_LAYER_COUNT 1

#define COMMAND_BUFFER_RENDER_PASS_RENDER_AREA_OFFSET_X 0
#define COMMAND_BUFFER_RENDER_PASS_RENDER_AREA_OFFSET_Y 0
#define COMMAND_BUFFER_RENDER_PASS_CLEAR_VALUE_COUNT 1

#define SUBMIT_INFO_WAIT_SEMAPHORE_COUNT 1
#define SUBMIT_INFO_WAIT_STAGE_COUNT 1
#define SUBMIT_INFO_COMMAND_BUFFER_COUNT 1
#define SUBMIT_INFO_SIGNAL_SEMAPHORE_COUNT 1
#define PRESENT_INFO_WAIT_SEMAPHORE_COUNT SUBMIT_INFO_WAIT_SEMAPHORE_COUNT
#define PRESENT_INFO_SWAP_CHAIN_COUNT 1

#define CLEAR_COLOR_R 0.0f
#define CLEAR_COLOR_G 0.0f
#define CLEAR_COLOR_B 0.0f
#define CLEAR_COLOR_A 1.0f

#define COMMAND_BUFFER_DRAW_VERTEX_COUNT 3
#define COMMAND_BUFFER_DRAW_INSTANCE_COUNT 1
#define COMMAND_BUFFER_DRAW_FIRST_VERTEX 0
#define COMMAND_BUFFER_DRAW_FIRST_INSTANCE 0

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define APP_NAME "LearnVulkan"
#define ENGINE_NAME "My Engine"

#define APP_VERSION VK_MAKE_VERSION(0, 1, 0)
#define ENGINE_VERSION VK_MAKE_VERSION(0, 1, 0)

#define DEBUG_LOG(x) std::cout << "DEBUG_LOG: " << x << '\n';
#define DEBUG_CALLBACK(o, s, t, d) std::c ## o 	<< "Debug Callback:\n"\
												<< "\tServerity: " << vk::to_string(s)\
												<< "\n\tType: "    << vk::to_string(t)\
												<< "\n\tMessage: \n\t\t" << d << '\n'


static VKAPI_ATTR vk::Bool32 VKAPI_CALL vkDebugCallback(
	vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	vk::DebugUtilsMessageTypeFlagsEXT messageType,
	const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
	[[maybe_unused]] void *pUserData)
{
	if (messageSeverity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
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
	: m_dynamicLoader(vkGetInstanceProcAddr), 
	m_currentFrame(0),
#ifdef DEBUG
	m_validationLayers{VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME}, 
#endif
	m_deviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
#ifdef __APPLE__
		, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#endif
	}
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
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		m_logicalDevice.destroySemaphore(m_imageAvailableSemaphores[i]);
		m_logicalDevice.destroySemaphore(m_renderFinishedSemaphores[i]);
		m_logicalDevice.destroyFence(m_inFlightFences[i]);
	}

	m_logicalDevice.destroyCommandPool(m_commandPool);

	for (const auto &framebuffer : m_swapChainFramebuffers)
		m_logicalDevice.destroyFramebuffer(framebuffer);

	m_logicalDevice.destroyPipeline(m_graphicsPipeline);
	m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
	m_logicalDevice.destroyRenderPass(m_renderPass);

	for (const auto &image_view : m_swapChainImageViews)
		m_logicalDevice.destroyImageView(image_view);

	m_logicalDevice.destroySwapchainKHR(m_swapChain);
	m_logicalDevice.destroy();
	
	m_instance.destroySurfaceKHR(m_surface);
#ifdef DEBUG
	m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr, m_dynamicLoader);
#endif
	m_instance.destroy();
}

void Application::createWindow() 
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Not using OpenGL
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Resizing requires recreating the swapchain

	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	glfwSetErrorCallback(glfwErrorCallback);
	assert(glfwVulkanSupported() && "VULKAN ASSERT: Vulkan not supported on device!");
}

void Application::initVulkan() 
{
	// Functions HAVE to be called in this order
	createInstance(); // Everything relies on the instance
#ifdef DEBUG
	createDebugCallback(); // Enable debugging
#endif
	createSurface(); // Most of graphics relies on the surface
	choosePhysicalDevice(); // Need a GPU to render
	createLogicalDevice(); // Need to view the GPU
	createSwapChain(); // Need to render images to the screen
	createImageViews(); // Describes how to access the images in the swap chain
	createRenderPass(); // Specify framebuffer attachments
	createGraphicsPipeline(); // Creates the graphics pipeline
	createFrameBuffers(); // Creates a framebuffer for each image in the swap chain
	createCommandPool(); // Allocate memory for command buffers
	createCommandBuffers(); // Creates buffers of commands
	createSyncObjects(); // Synchronize the vulkan operations
}

void Application::createInstance() 
{
	// Check validation layers
	assert(areLayersSupported(std::array<const char * const, NUM_VALIDATION_LAYERS>{m_validationLayers[0]}) && "VULKAN ASSERT: Requested validation layers are not supported");

	// Application info
	const vk::ApplicationInfo app_info(
		APP_NAME, 
		APP_VERSION, 
		ENGINE_NAME, 
		ENGINE_VERSION, 
		VK_API_VERSION_1_2
	);

	// Get required extensions
	const std::vector<const char *> extensions = getRequiredInstanceExtensions();

	// Create instance info
	vk::InstanceCreateInfo instance_create_info{
		{}, // Flags
		&app_info, // Application info
#ifdef DEBUG
		NUM_VALIDATION_LAYERS, // Enabled layer count: number of wanted validation layers
		m_validationLayers, // Enabled layer names: wanted validation layers
#else
		0,
		nullptr,
#endif
		static_cast<uint32_t>(extensions.size()), // Enabled extension count: number of wanted extensions
		extensions.data() // Enabled extension names: wanted extensions
	};

#ifdef DEBUG
	// Add debugging info
	const vk::DebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{
		{}, 
		// vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
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
	m_instance = vk::createInstance(instance_create_info);

	// Add more functions to dynamic loader
	m_dynamicLoader.init(m_instance);
}

void Application::createDebugCallback() 
{
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
	m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(debug_messenger_create_info, nullptr, m_dynamicLoader); // Extension functions need to be dynamically loaded
}

void Application::createSurface()
{
	VkSurfaceKHR temp_surface;
#ifdef DEBUG
	assert(glfwCreateWindowSurface(m_instance, m_window, nullptr, &temp_surface) == VK_SUCCESS && "VULKAN ASSERT: Failed to create window surface!");
#else
	glfwCreateWindowSurface(m_instance, m_window, nullptr, &temp_surface);
#endif
	m_surface = temp_surface;
}

void Application::choosePhysicalDevice()
{
	// Get all physical devices (GPUs) in the computer
	// const std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices(); // Could use, but I use old way to learn

	vk::PhysicalDevice *physical_devices;
    uint32_t physical_device_count;
    vk::Result result;

    result = m_instance.enumeratePhysicalDevices(&physical_device_count, nullptr);
	assert(physical_device_count && "VULKAN ASSERT: Failed to find GPU/s with Vulkan support!");

	physical_devices = new vk::PhysicalDevice[physical_device_count];
    result = m_instance.enumeratePhysicalDevices(&physical_device_count, physical_devices);
    physical_devices = createResultValue(result, physical_devices, "Application::getPhysicalDevice");

	for (uint32_t i = 0; i < physical_device_count; ++i) {
		if (physicalDeviceSupportsRequirements(physical_devices[i])) {
			m_physicalDevice = physical_devices[i];
			delete[] physical_devices;
			return;
		}
	}

	delete[] physical_devices;
	assert(false && "VULKAN ASSERT: Failed to find GPU with rquested requirements!");
}

void Application::createLogicalDevice()
{
	m_queueFamilyIndices = getQueueFamilies(m_physicalDevice);
	const vk::PhysicalDeviceFeatures features;

	std::vector<vk::DeviceQueueCreateInfo> device_queue_create_infos;
	const std::unordered_set<uint32_t> queue_families = {
		m_queueFamilyIndices.graphicsFamily.value(),
		m_queueFamilyIndices.presentFamily.value()
	};

	constexpr const float queue_priority = 1.0f;
	if (m_queueFamilyIndices.graphicsFamily.value() == m_queueFamilyIndices.presentFamily.value()) {
		for (const auto &queue_family : queue_families)
			device_queue_create_infos.emplace_back(vk::DeviceQueueCreateInfo{{}, queue_family, 1, &queue_priority});
	} else {
		device_queue_create_infos.reserve(2);
		for (const auto &queue_family : queue_families)
			device_queue_create_infos.emplace_back(vk::DeviceQueueCreateInfo{{}, queue_family, 2, &queue_priority});
	}

	const vk::DeviceCreateInfo device_create_info{
		{}, // Flags
		static_cast<uint32_t>(device_queue_create_infos.size()), // Queue create info count: number of queue create infos
		device_queue_create_infos.data(), // Queue create infos: array of queue create infos
#ifdef DEBUG
		NUM_VALIDATION_LAYERS, // Enabled layer count: number of validation layers
		m_validationLayers, // Enabled layer names: required validation layers (when in debug)
#else
		0, nullptr, // No validation layers required (when in release)
#endif
		NUM_DEVICE_EXTENSIONS, // Enabled extension count: number of device extensions
		m_deviceExtensions, // Enabled extension names: required device extensions
		&features // Enabled features: features of physical device
	};

	m_logicalDevice = m_physicalDevice.createDevice(device_create_info);
	// m_dynamicLoader.init(m_logicalDevice); // Add more functions to dynamic loader // Not needed right now

	m_graphicsQueue = m_logicalDevice.getQueue(m_queueFamilyIndices.graphicsFamily.value(), 0);
	m_presentQueue = m_logicalDevice.getQueue(m_queueFamilyIndices.presentFamily.value(), 0);

}

void Application::createSwapChain()
{
	const SwapChainSupportDetails swap_chain_support = querySwapChainSupport(m_physicalDevice);

	const vk::SurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swap_chain_support.formats);
	const vk::PresentModeKHR present_mode = chooseSwapPresentMode(swap_chain_support.presentModes);
	extent = chooseSwapExtent(swap_chain_support.capabilities);

	uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1; // Amount of images to be stored in the queue (recommended to request at least 1 more than the minimum)

	if (swap_chain_support.capabilities.maxImageCount > 0 && // If maxImageCount == 0, then there is no maximum
		image_count > swap_chain_support.capabilities.maxImageCount) // Wanted queue size is larger than the maxmimum
		image_count = swap_chain_support.capabilities.maxImageCount;

	vk::SwapchainCreateInfoKHR swapchain_create_info{
		{}, // Flags
		m_surface, // Surface
		image_count, // Min image count
		surface_format.format, // Image format
		surface_format.colorSpace, // Image color space
		extent, // Image extent
		SWAPCHAIN_IMAGE_ARRAY_LAYER_COUNT, // Image array layers: The amount of layers each image consists of
		vk::ImageUsageFlagBits::eColorAttachment, // The kind of operations we'll use the images in the swap chain for (color attachment == render directly to them)
		vk::SharingMode::eExclusive, // Image sharing mode: An image is owned by one queue family at a time and ownership must be explicitly transferred before using it in another queue family (offers the best performance)
		{}, // Queue family index count
		{}, // Queue family indices
		swap_chain_support.capabilities.currentTransform, // Pre transform (current transform means add no transform)
		vk::CompositeAlphaFlagBitsKHR::eOpaque, // For blending with other windows in the window system (Opaque means to ignore the alpha channel)
		present_mode, // Present mode
		VK_TRUE, // Clipped: true if you don't care about the color of pixels that are obscured, for example because another window is in front of them
		{} // Old swap chain: reference to old swap chain if current is invalid or unoptimized (e.g. because window is resized). No given reference means we'll only create one swap chain
	};
	
	// Queue families
	if (m_queueFamilyIndices.graphicsFamily != m_queueFamilyIndices.presentFamily) { // Use concurrent mode if queue families differ to avoid manual ownership transfer
		const uint32_t queue_family_indices[QUEUE_FAMILY_INDEX_COUNT] = {
			m_queueFamilyIndices.graphicsFamily.value(),
			m_queueFamilyIndices.presentFamily.value()
		};

		swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent; // Images can be used across multiple queue families without explicit ownership transfers
		swapchain_create_info.queueFamilyIndexCount = QUEUE_FAMILY_INDEX_COUNT;
		swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
	}

	m_swapChain = m_logicalDevice.createSwapchainKHR(swapchain_create_info);
	m_swapChainImages = m_logicalDevice.getSwapchainImagesKHR(m_swapChain);
	m_swapChainImageFormat = surface_format.format;
	m_swapChainExtent = extent;
}

void Application::createImageViews()
{
	m_swapChainImageViews.reserve(m_swapChainImages.size());

	for (const auto &image : m_swapChainImages) {
		// Allows you to swizzle the color channels around (Identity for default mapping)
		const vk::ComponentMapping image_view_color_components{
			vk::ComponentSwizzle::eIdentity,
			vk::ComponentSwizzle::eIdentity,
			vk::ComponentSwizzle::eIdentity,
			vk::ComponentSwizzle::eIdentity
		};

		// Describes what the image's purpose is and which parts of the image should be accessed
		const vk::ImageSubresourceRange image_view_subresource_range{
			vk::ImageAspectFlagBits::eColor, // Aspect mask (using color targets)
			IMAGE_VIEW_BASE_MIP_LEVEL, // Base mip level (first mipmapping level)
			IMAGE_VIEW_MIP_LEVEL_AMOUNT, // Mip level amount (one mipmapping level)
			IMAGE_VIEW_BASE_ARRAY_LAYER, // Base array layer (first array layer)
			IMAGE_VIEW_ARRAY_LAYER_AMOUNT // Array layer amount (one array layer)
		};

		const vk::ImageViewCreateInfo image_view_create_info{
			{}, // Flags
			image, // Swap chain image
			vk::ImageViewType::e2D, // Treat images as 2D textures
			m_swapChainImageFormat, // Chosen image format
			image_view_color_components, // Color components
			image_view_subresource_range // Subresource range
		};

		m_swapChainImageViews.emplace_back(m_logicalDevice.createImageView(image_view_create_info));
	}
}

void Application::createRenderPass()
{
	// Color buffer attachment represented by one of the images from the swap chain
	const vk::AttachmentDescription color_attachment{
		{}, // Flags
		m_swapChainImageFormat, // Format (matches the swap chain image format)
		vk::SampleCountFlagBits::e1, // Samples (1 sample; not multisampling)
		vk::AttachmentLoadOp::eClear, // Load op: what to do with the color and depth data in the attachment before and after rendering (clear the values to a constant at the start)
		vk::AttachmentStoreOp::eStore, // Store op: what to do with the color and depth data in the attachment before and after rendering (rendered contents will be stored in memory and can be read later)
		vk::AttachmentLoadOp::eDontCare, // Stencil load op: what to do with the stencil data in the attachment before and after rendering (not using; don't care)
		vk::AttachmentStoreOp::eDontCare, // Stencil store op: what to do with the stencil data in the attachment before and after rendering (not using; don't care)
		vk::ImageLayout::eUndefined, // Initial layout: the layout the image will have before the render pass begins (don't care)
		vk::ImageLayout::ePresentSrcKHR // Final layout: the layout to automatically transition to when the render pass finishes (images to be presented in the swap chain)
	};

	// Reference attachments using the indices of an array of attachments
	const vk::AttachmentReference color_attachment_reference{
		COLOR_ATTACHMENT_REFERENCE_INDEX, // Attachment: which attachment to reference by its index in the attachment descriptions array (only using 1, so index 0)
		vk::ImageLayout::eColorAttachmentOptimal // Layout: which layout we would like the attachment to have during a subpass that uses this reference (using color buffer and wanting optimal preformance)
	};

	// Subsequent rendering operations that depend on the contents of framebuffers in previous passes
	const vk::SubpassDescription subpass{
		{}, // Flags
		vk::PipelineBindPoint::eGraphics, // Pipeline bind point: type of subpass (graphics subpass)
		SUBPASS_INPUT_ATTACHMENT_COUNT, // Input attachment count: number of input attachments (not using any)
		nullptr, // Input attachments: attachments that are read from a shader (not using any)
		SUBPASS_COLOR_ATTACHMENT_COUNT, // Color attachment count: number of color attachments (using 1)
		&color_attachment_reference, // Color attachments: attachments used for outputing pixel color (this is index 0 of the color attachment array, referenced from the fragment shader: layout(location = 0) out vec4 color)
		nullptr, // Resolve attachments: Attachments used for multisampling color attachments (not using any)
		nullptr, // Depth stencil attachment: Attachment for depth and stencil data (not using)
		SUBPASS_PRESERVE_ATTACHMENT_COUNT, // Preserve attachment count: number of preserve attachments (not using any)
		nullptr // Preserve attachments: attachments that are not used by this subpass, but for which the data must be preserved (not using any)
	};

	// Dependencies automatically takes care of image layout transitions
	const vk::SubpassDependency subpass_dependency{
		SUBPASS_DEPENDENCY_SRC_SUBPASS, // Src subpass (VK_SUBPASS_EXTERNAL refers to the implicit subpass before or after the render pass)
		SUBPASS_DEPENDENCY_DST_SUBPASS, // Dst subpass; must always be higher than srcSubpass to prevent cycles in the dependency graph; except for VK_SUBPASS_EXTERNAL (index 0 refers to the only created subpass)
		vk::PipelineStageFlagBits::eColorAttachmentOutput, // Src stage mask: the stages to wait on
		vk::PipelineStageFlagBits::eColorAttachmentOutput, // Dst stage mask
		{}, // Src access mask: the operations to wait on
		vk::AccessFlagBits::eColorAttachmentWrite, // Dst access mask
		{} // Dependency flags
	};

	const vk::RenderPassCreateInfo render_pass_create_info{
		{}, // Flags
		RENDER_PASS_ATTACHMENT_COUNT, // Attachment count: number of attachments (using 1)
		&color_attachment, // Attachments: the array of attachments (using 1)
		RENDER_PASS_SUBPASS_COUNT, // Subpass count: number of subpasses (using 1)
		&subpass, // Subpasses: array of subpasses (using 1)
		RENDER_PASS_DEPENDENCY_COUNT, // Dependency count: number of dependecies (using 1)
		&subpass_dependency // Dependecies: array of subpass dependecies (not using any)
	};

	m_renderPass = m_logicalDevice.createRenderPass(render_pass_create_info);
}

void Application::createGraphicsPipeline()
{
	const std::vector<char> vertex_shader_code = readFile(std::move("bin/vert.spv"));
	const std::vector<char> fragment_shader_code = readFile(std::move("bin/frag.spv"));

	const vk::ShaderModule vertex_shader_module = createShaderModule(vertex_shader_code);
	const vk::ShaderModule fragment_shader_module = createShaderModule(fragment_shader_code);

	// Describes the shader module
	const vk::PipelineShaderStageCreateInfo vertex_shader_stage_create_info{
		{}, // Flags
		vk::ShaderStageFlagBits::eVertex, // Shader Stage/Type (Vertex)
		vertex_shader_module, // Shader module containing the code
		"main", // Entrypoint of the shader (main function)
		{} // Specialization Info: specify values for shader constants (not using)
	};

	const vk::PipelineShaderStageCreateInfo fragment_shader_stage_create_info{
		{},
		vk::ShaderStageFlagBits::eFragment,
		fragment_shader_module,
		"main",
		{}
	};

	// Create an array containing the shader stage create infos
	const vk::PipelineShaderStageCreateInfo shader_stages[PIPELINE_SHADER_STAGE_COUNT] = {
		std::move(vertex_shader_stage_create_info),
		std::move(fragment_shader_stage_create_info)
	};

	// Describes the format of the vertex data that will be passed to the vertex shader
	const vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info{
		{}, // Flags
		PIPELINE_VERTEX_BINDING_DESCRIPTION_COUNT, // Vertex binding description count (no data being passed into vertex shader)
		nullptr, // Vertex binding descriptions: points to an array of structs that describe the details for loading vertex bindings (no data is being loaded)
		PIPELINE_VERTEX_ATTRIBUTE_DESCRIPTION_COUNT, // Vertex attribute description count (no data being passed into vertex shader)
		nullptr // Vertex attribute descriptions: points to an array of structs that describe the details for loading vertex attributes (no data is being loaded)
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
		PIPELINE_VIEWPORT_X, // X
		PIPELINE_VIEWPORT_Y, // Y
		static_cast<float>(m_swapChainExtent.width), // Width
		static_cast<float>(m_swapChainExtent.height), // Height
		PIPELINE_VIEWPORT_MIN_DEPTH, // Min depth: the minimum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
		PIPELINE_VIEWPORT_MAX_DEPTH // Max depth: the maximum depth value to use for the framebuffer; must be within the [0.0f, 1.0f] range
	};

	// Defines which regions pixels will actually be stored
	// Any pixels outside the scissor rectangles will be discarded by the rasterizer
	const vk::Rect2D scissor_rectangle{
		PIPELINE_SCISSOR_OFFSET, // Offset (no offset)
		m_swapChainExtent // Extent (the chosen extent of the screen)
	};

	// Viewport state containing viewport and scissor rectangle
	const vk::PipelineViewportStateCreateInfo viewport_state_create_info{
		{}, // Flags
		PIPELINE_VIEWPORT_COUNT, // Viewport count: number of viewports (only using 1)
		&viewport, // Viewports: pointer to array of viewports being used (only using 1)
		PIPELINE_SCISSOR_COUNT, // Scissor count: number of scissor rectangles (only using 1)
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
		vk::FrontFace::eClockwise, // Front face: specifies the vertex order for faces to be considered front-facing (clockwise)
		VK_FALSE, // Depth bias enable: alters the depth values by adding a constant value or biasing them based on a fragment's slope (not using)
		PIPELINE_RASTERIZATION_DEPTH_BIAS_CONSTANT_FACTOR, // Depth bias constant factor (not using)
		PIPELINE_RASTERIZATION_DEPTH_BIAS_CLAMP, // Depth bias clamp (not using)
		PIPELINE_RASTERIZATION_DEPTH_BIAS_SLOPE_FACTOR, // Depth bias slope factor (not using)
		PIPELINE_RASTERIZATION_LINE_WIDTH // Line width: the thickness of lines in terms of number of fragments (1 fragment thick)
	};

	// Configures multisampling by combining the fragment shader results of multiple polygons that rasterize to the same pixel
	// Not using multisampling, so struct is disabled
	const vk::PipelineMultisampleStateCreateInfo multisample_state_create_info{
		{}, // Flags
		vk::SampleCountFlagBits::e1, // Rasterization samples
		VK_FALSE, // Sample shading enable
		PIPELINE_MULTISAMPLE_MIN_SAMPLE_SHADING, // Min sample shading
		nullptr, // Sample mask
		VK_FALSE, // Alpha to coverage enable
		VK_FALSE // Alpha to one enable
	};

	// Configures the depth and stencil tests
	// Not using depth or stencil tests, so struct is disabled
	// const vk::PipelineDepthStencilStateCreateInfo depth_stencil_state_create_info{};

	// Contains the color blending configuration per attached framebuffer
	// Not using blending, so struct is disabled
	const vk::PipelineColorBlendAttachmentState color_blend_attachment_state{
		VK_FALSE, // Blend enable (disable blending)
		vk::BlendFactor::eSrcAlpha, // Src color blend factor; NOTE - Keep eye on
		vk::BlendFactor::eOneMinusSrcAlpha, // Dst color blend factor; NOTE - Keep eye on
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
		PIPELINE_COLOR_BLEND_ATTACHMENT_COUNT, // Attachment count: number of color blend attachments
		&color_blend_attachment_state, // Attachments: pointer to array of color blend attachments
		blend_constants // Blend constants
	};

	// States that can and have to be changed at drawing time
	const vk::DynamicState dynamic_states[PIPELINE_DYNAMIC_STATE_COUNT] = {
		vk::DynamicState::eViewport,
		vk::DynamicState::eLineWidth
	};

	// Describes which pipeline configurations can be changed without recreating the pipeline
	const vk::PipelineDynamicStateCreateInfo dynamic_state_create_info{
		{}, // Flags
		PIPELINE_DYNAMIC_STATE_COUNT, // Dynamic state count: number of dynamic states
		dynamic_states // Dynamic states: pointer to array of dynamic states
	};

	// Infomation for the pipeline layout
	const vk::PipelineLayoutCreateInfo pipeline_layout_create_info{
		{}, // Flags
		PIPELINE_SET_LAYOUT_COUNT, // Set layout count
		nullptr, // Set layouts
		PIPELINE_PUSH_CONSTANT_RANGE_COUNT, // Push constant range count
		nullptr // Push constant ranges
	};

	// Create pipeline layout
	m_pipelineLayout = m_logicalDevice.createPipelineLayout(pipeline_layout_create_info);

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
		nullptr, // Depth stencil state
		&color_blend_state_create_info, // Color blend state
		nullptr, // Dynamic state
		m_pipelineLayout, // Layout
		m_renderPass, // Render pass
		0, // Subpass: index of the subpass where this graphics pipeline will be used
		nullptr, // Base pipeline handle
		-1 // Base pipeline index
	};

	// Create the graphics pipeline!
	m_graphicsPipeline = m_logicalDevice.createGraphicsPipeline(nullptr, pipeline_create_info).value;

	// Shader modules are no longer needed
	m_logicalDevice.destroyShaderModule(vertex_shader_module);
	m_logicalDevice.destroyShaderModule(fragment_shader_module);
}

void Application::createFrameBuffers()
{
	m_swapChainFramebuffers.reserve(m_swapChainImageViews.size()); // Create a framebuffer for each image

	for (size_t i = 0; i < m_swapChainImageViews.size(); ++i) {
		const vk::ImageView attachments[FRAMEBUFFER_ATTACHMENT_COUNT] = {
			m_swapChainImageViews[i]
		};

		const vk::FramebufferCreateInfo framebuffer_create_info{
			{}, // Flags
			m_renderPass, // Render pass
			FRAMEBUFFER_ATTACHMENT_COUNT, // Attachment count
			attachments, // Attachments: the vk::ImageView objects that should be bound to the respective attachment descriptions in the render pass Attachment array
			m_swapChainExtent.width, // Width
			m_swapChainExtent.height, // Height
			FRAMEBUFFER_LAYER_COUNT // Layers: number of layers in image arrays (using single images)
		};

		m_swapChainFramebuffers.emplace_back(m_logicalDevice.createFramebuffer(framebuffer_create_info));
	}
}

void Application::createCommandPool()
{
	const vk::CommandPoolCreateInfo command_pool_create_info{
		{}, // Flags
		m_queueFamilyIndices.graphicsFamily.value() // Queue family index
	};

	m_commandPool = m_logicalDevice.createCommandPool(command_pool_create_info);
}

void Application::createCommandBuffers()
{
	m_commandBuffers.resize(m_swapChainFramebuffers.size());

	// Infomation for allocating memory
	const vk::CommandBufferAllocateInfo command_buffer_allocate_info{
		m_commandPool, // Command pool
		vk::CommandBufferLevel::ePrimary, // Level: if the allocated command buffers are primary or secondary command buffers
		static_cast<uint32_t>(m_commandBuffers.size()) // Command buffer count
	};

	m_commandBuffers = m_logicalDevice.allocateCommandBuffers(command_buffer_allocate_info);

	for (size_t i = 0; i < m_commandBuffers.size(); ++i) {
		const vk::CommandBufferBeginInfo command_buffer_begin_info{
			vk::CommandBufferUsageFlagBits::eSimultaneousUse, // Flags (use command buffers simultaniously)
			nullptr // Inheritance info: which state to inherit from the calling primary command buffers (already are primary)
		};

		// Begin the command buffer
		m_commandBuffers[i].begin(command_buffer_begin_info);

		const vk::Offset2D render_area_offset{
			COMMAND_BUFFER_RENDER_PASS_RENDER_AREA_OFFSET_X, 
			COMMAND_BUFFER_RENDER_PASS_RENDER_AREA_OFFSET_Y
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

		// Command to render to the screen
		const vk::RenderPassBeginInfo render_pass_begin_info{
			m_renderPass, // Render pass
			m_swapChainFramebuffers[i], // Framebuffer
			render_area, // Render area
			COMMAND_BUFFER_RENDER_PASS_CLEAR_VALUE_COUNT, // Clear value count
			&clear_color // Clear values
		};

		// Begin the render pass
		m_commandBuffers[i].beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline); // Inline: the render pass commands will be embedded in the primary command buffer itself
		
		// Bind the graphics pipeline with the command buffer
		m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline);

		// DRAW THE TRIANGLE!!!!
		m_commandBuffers[i].draw(
			COMMAND_BUFFER_DRAW_VERTEX_COUNT, // Vertex count (drawing three verticies)
			COMMAND_BUFFER_DRAW_INSTANCE_COUNT, // Instance count: used for instanced rendering (not doing, so 1)
			COMMAND_BUFFER_DRAW_FIRST_VERTEX, // First vertex: an offset into the vertex buffer; defines the lowest value of gl_VertexIndex
			COMMAND_BUFFER_DRAW_FIRST_INSTANCE // First instance: an offset for instanced rendering; defines the lowest value of gl_InstanceIndex
		);

		// End the render pass
		m_commandBuffers[i].endRenderPass();

		// End the command buffer
		m_commandBuffers[i].end();
	}
}

void Application::createSyncObjects()
{
	// Create the right amount of fences
	m_imagesInFlight.resize(m_swapChainImages.size(), nullptr); // Initalize to no fence

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
		m_imageAvailableSemaphores[i] = m_logicalDevice.createSemaphore(semaphore_create_info);
		m_renderFinishedSemaphores[i] = m_logicalDevice.createSemaphore(semaphore_create_info);
		m_inFlightFences[i] = m_logicalDevice.createFence(fence_create_info);
	}
}

// Check if validation layers are supported
template <size_t S>
bool Application::areLayersSupported(const std::array<const char * const, S> &needed_layers) 
{
	// Get layers
	// const std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties(); // Could use, but I use old way to learn

	vk::LayerProperties *layer_properties;
    uint32_t layer_property_count;
    vk::Result result;

    result = vk::enumerateInstanceLayerProperties(&layer_property_count, nullptr);
	layer_properties = new vk::LayerProperties[layer_property_count];
    result = vk::enumerateInstanceLayerProperties(&layer_property_count, layer_properties);
    layer_properties = createResultValue(result, layer_properties, "Application::areLayersSupported");

	bool found;
	for (const auto &needed_layer : needed_layers) {
		found = false;
		for (uint32_t i = 0; i < layer_property_count; ++i) {
			if (strcmp(needed_layer, static_cast<const char *>(layer_properties[i].layerName)) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			delete[] layer_properties;
			return false;
		}
	}

	delete[] layer_properties;
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
bool Application::physicalDeviceSupportsRequirements(const vk::PhysicalDevice &device)
{
	// Possible ways of determining which GPU to use
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

	for (uint32_t i = 0; i < queue_families.size(); ++i) {
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
	// Get GPU extensions
	// const std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties(); // Could use, but I use old way to learn

	vk::ExtensionProperties *extension_properties;
    uint32_t extension_property_count;
    vk::Result result;

    result = device.enumerateDeviceExtensionProperties(nullptr, &extension_property_count, nullptr);
	extension_properties = new vk::ExtensionProperties[extension_property_count];
    result = device.enumerateDeviceExtensionProperties(nullptr, &extension_property_count, extension_properties);
    extension_properties = createResultValue(result, extension_properties, "Application::deviceSupportsExtensions");
	
	std::unordered_set<std::string_view> required_extensions(&m_deviceExtensions[0], &m_deviceExtensions[NUM_DEVICE_EXTENSIONS - 1]);

	for (uint32_t i = 0; i < extension_property_count; ++i)
		required_extensions.erase(static_cast<std::string_view>(extension_properties[i].extensionName));
	
	delete[] extension_properties;
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
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent; // Width and height are already set
	
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	// The max and min functions are used here to clamp 
	// the value of WIDTH and HEIGHT between the allowed 
	// minimum and maximum extents that are supported by the implementation
	return {
		std::clamp(static_cast<uint32_t>(width),  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width),
		std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
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

	file.seekg(0);
	file.read(buffer.data(), file_size);

	return buffer;
}

vk::ShaderModule Application::createShaderModule(const std::vector<char> &code)
{
	const vk::ShaderModuleCreateInfo shader_module_create_info{
		{}, // Flags
		code.size(), // Size of the code (in bytes)
		reinterpret_cast<const uint32_t *>(code.data()) // The buffer of data
	};

	const vk::ShaderModule shader_module = m_logicalDevice.createShaderModule(shader_module_create_info);
	return shader_module;
}

void Application::run() 
{
	// Set up FPS counter
	m_previousTimeFPS = glfwGetTime();
	m_frameCountFPS = 0;

	// Main game loop
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
		drawFrame();
		printFPS();
	}

	m_logicalDevice.waitIdle();
}

void Application::printFPS()
{
	const double current_time = glfwGetTime();
	++m_frameCountFPS;

	if (current_time - m_previousTimeFPS >= 1.0) {
		std::cout << std::setprecision(4) << "Application Average: " << 1000.0f / m_frameCountFPS << "ms\t(" << m_frameCountFPS << " FPS)\n";

		m_frameCountFPS = 0;
		m_previousTimeFPS = current_time;
	}
}

void Application::drawFrame()
{
	// Wait for fence to be signaled
	[[maybe_unused]] const vk::Result wait_for_fences_result = m_logicalDevice.waitForFences(
		m_inFlightFences[m_currentFrame], // Fences: which fences to wait on
		VK_TRUE, // Wait all: whether to wait for all the fences or just one
		std::numeric_limits<uint64_t>::max() // Timeout: timeout in nanoseconds (uint64 max disables the timeout)
	);

	// Index refers to the vk::Image in the swap chain images array
	const uint32_t image_index = m_logicalDevice.acquireNextImageKHR(
		m_swapChain, // Swap Chain
		std::numeric_limits<uint64_t>::max(), // Timeout: timeout in nanoseconds for an image to become available (uint64 max disables the timeout)
		m_imageAvailableSemaphores[m_currentFrame], // Semaphore: synchronization object to be signaled when the presentation engine is finished using the image
		nullptr // Fence: similar to semaphore (not using)
	).value;

	// Check if a previous frame is using this image; wait on it's fence
	if (static_cast<bool>(m_imagesInFlight[image_index]))
		[[maybe_unused]] const vk::Result wait_for_fences_result_2 = m_logicalDevice.waitForFences(m_imagesInFlight[image_index], VK_TRUE, std::numeric_limits<uint64_t>::max());
	
	// Mark image as now being in use by this frame
	m_imagesInFlight[image_index] = m_inFlightFences[m_currentFrame];

	const vk::Semaphore wait_semaphores[SUBMIT_INFO_WAIT_SEMAPHORE_COUNT] = {
		m_imageAvailableSemaphores[m_currentFrame]
	};

	const vk::PipelineStageFlags wait_stages[SUBMIT_INFO_WAIT_STAGE_COUNT] = {
		vk::PipelineStageFlagBits::eColorAttachmentOutput
	};

	const vk::Semaphore signal_semaphores[SUBMIT_INFO_SIGNAL_SEMAPHORE_COUNT] = {
		m_renderFinishedSemaphores[m_currentFrame]
	};

	// Information for submitting to the graphics queue
	const vk::SubmitInfo submit_info{
		SUBMIT_INFO_WAIT_SEMAPHORE_COUNT, // Wait semaphore count
		wait_semaphores, // Wait semaphores: which semaphores to wait on before execution begins
		wait_stages, // Wait dst stage mask: which stages of the graphics pipeline to wait on before execution; corresponds to the wait_semaphores
		SUBMIT_INFO_COMMAND_BUFFER_COUNT, // Command buffer count
		&m_commandBuffers[image_index], // Command buffers: which command buffers to actually submit for execution
		SUBMIT_INFO_SIGNAL_SEMAPHORE_COUNT, // Signal semaphore count
		signal_semaphores // Signal semaphores: which semaphores to signal once the command buffer/s have finished execution
	};

	// Set fence back to unsignaled state
	m_logicalDevice.resetFences(
		m_inFlightFences[m_currentFrame] // Fences: which fences to reset
	);

	// Submit the command buffer to the graphics queue
	m_graphicsQueue.submit(submit_info, m_inFlightFences[m_currentFrame]);

	// Swap chains
	const vk::SwapchainKHR swap_chains[PRESENT_INFO_SWAP_CHAIN_COUNT] = {
		m_swapChain
	};

	// Presentation configurations
	const vk::PresentInfoKHR present_info{
		PRESENT_INFO_WAIT_SEMAPHORE_COUNT, // Wait semaphore count
		signal_semaphores, // Wait semaphores (using render_finished not image_available)
		PRESENT_INFO_SWAP_CHAIN_COUNT, // Swap chain count
		swap_chains, // Swap chains
		&image_index, // Image indices
		nullptr // Results: specify an array of vk::Result values to check for every individual swap chain if presentation was successful (not nessassary if only using 1 swap chain)
	};

	// Request the presentation of the image
	[[maybe_unused]] const vk::Result present_result = m_presentQueue.presentKHR(present_info);

	// Wait for image to be submitted
	// m_presentQueue.waitIdle(); // Using fences instead

	// Advance to the next frame
	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Application::printRequiredExtensions() 
{
	const std::vector<const char *> extensions = getRequiredInstanceExtensions();

	std::cout << "Required Extensions:\n";
	for (const auto &extension : extensions)
		std::cout << '\t' << extension << '\n';
}

void Application::printAllExtensions() 
{
	const std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();

	std::cout << "All Extensions:\n";
	for (const auto &extension : extensions)
		std::cout << '\t' << static_cast<const char *>(extension.extensionName) << ", Version " << extension.specVersion << '\n';
}

void Application::printLayerSupport() 
{
	const std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();

	std::cout << "Layers:\n";
	for (const auto &layer : layers)
		std::cout << '\t' << static_cast<const char *>(layer.layerName) << ", Version " << layer.specVersion 
		<< "\n\t\t" << static_cast<const char *>(layer.description) << '\n';
}

void Application::printPhysicalDevices()
{
	const std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();

	std::cout << "Physical Devices:\n";
	for (const auto &device : devices)
		std::cout << '\t' << static_cast<VkPhysicalDevice>(device) << '\n';
}
