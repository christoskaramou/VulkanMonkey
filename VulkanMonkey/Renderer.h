#pragma once

#include "Vulkan_.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "Camera.h"
#include <fstream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>
#include "Light.h"

namespace vm {
	class VulkanQueueFamily
	{
	public:
		uint32_t graphicsFamilyId = -1;
		uint32_t presentFamilyId = -1;
		void findQueueFamilies(vk::PhysicalDevice gpu, vk::SurfaceKHR surface);
		bool isComplete() {
			return (graphicsFamilyId >= 0 && presentFamilyId >= 0);
		}
	};

	class SwapchainInfo
	{
	public:
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> surfaceFormats{};
		std::vector<vk::PresentModeKHR> presentModes{};
		void querySwapChainSupport(vk::PhysicalDevice gpu, vk::SurfaceKHR surface);
		vk::SurfaceFormatKHR chooseSwapchainFormat();
		vk::PresentModeKHR chooseSwapchainPresentMode();
		vk::Extent2D chooseSwapchainExtent();
	};
	class Renderer
	{
		friend struct Camera;
	public:
		Renderer(GLFWwindow * _window);
		~Renderer();

		// draw
		void summit(bool useDynamicCmdBuffer = true);

		// scene
		void pushSpritesToBuffers(); // after the scene is made, all sprites created are auto pushed in a big buffer

		//for resize mainly
		void reInitSwapchain();

		//get gpu name
		std::string getGpuName() const;

		vk::Extent2D swapchainExtent;

		Helper helper;

		Camera mainCamera;
		Camera* getMainCamera();

	private:
		GLFWwindow * window;

		// instance
		vk::Instance instance;
		void createInstance();
		void destroyInstance();

	public:
		// physical device (gpu)
		vk::PhysicalDevice gpu;
		vk::PhysicalDeviceProperties gpuProperties;
	private:
		vk::PhysicalDeviceFeatures gpuFeatures;
		void selectPhysicalDevice();
		bool isDeviceSuitable(vk::PhysicalDevice gpu, vk::SurfaceKHR surface);

	public:
		// logical device
		vk::Device device;
	private:
		vk::Queue graphicsQueue;
		vk::Queue presentQueue;
		void createLogicalDevice();
		void destroyLogicalDevice();

		// surfaceKHR
		vk::SurfaceKHR surface;
		void createSurface();
		void destroySurface();

		// swapchain
		vk::SwapchainKHR swapchain;
		vk::SurfaceCapabilitiesKHR surfaceCapabilitiesKHR;
		vk::SurfaceFormatKHR surfaceFormatKHR;
		vk::PresentModeKHR presentModeKHR;
		//vk::Extent2D swapchainExtent;
		uint32_t swapchainImageCount;
		std::vector<vk::Image> swapchainImages{};
		std::vector<vk::ImageView> swapchainImageViews{};
		void createSwapchain();
		void destroySwapchain();
		void createImageViews();
		void destroyImageViews();

		//textures
		void destroyTextures();

		// render pass
		vk::RenderPass renderPass;
		void createRenderPass();
		void destroyRenderPass();

		// buffers
		std::vector<vk::Framebuffer> swapchainFrameBuffers{};	// frame buffers
		std::vector<vk::CommandBuffer> commandBuffers{};		// command buffers
		vk::CommandBuffer dynamicCmdBuffer;						// one time summit command buffer
		vk::Image depthImage;									// depth image
		vk::DeviceMemory depthImageMemory;
		vk::ImageView depthImageView;
		vk::CommandPool commandPool;
		void createFrameBuffers();
		void destroyFrameBuffers();
		void createVertexBuffers();
		void destroyVertexBuffers();
		void createIndexBuffers();
		void destroyIndexBuffers();
		void createCommandPool();
		void createCommandBuffers();
		void destroyCommandPool();
		void recordOneTimeSubmitCommandBuffer(uint32_t imageIndex);
		void recordSimultaneousUseCommandBuffers();
		void createUniformBuffers();
		void destroyUniformBuffers();
		void createDepthResources();
		void destroyDepthResources();

		// descriptors
		vk::DescriptorPool descriptorPool;
		void createDescriptorSetLayout();
		void destroyDescriptorSetLayout();
		void createDescriptorPool();
		void destroyDescriptorPool();
		void createDescriptorSets();

		// pipeline
		vk::Pipeline pipeline;
		vk::PipelineLayout pipelineLayout;
		vk::Pipeline pipelineLines;
		vk::PipelineLayout pipelineLayoutLines;
		void createGraphicsPipeline();
		void destroyGraphicsPipeline();
		void createShaderModule(const std::vector<char>& code, vk::ShaderModule& shaderModule);

		// semaphores
		vk::Semaphore semaphore_Image_Available;
		vk::Semaphore semaphore_Render_Finished;
		void createSemaphores();
		void destroySemaphores();

		// fences
		vk::Fence fence;
		void createFence();
		void destroyFence();

		std::vector<const char*> instanceLayers{};
		std::vector<const char*> instanceExtensions{};
		std::vector<const char*> deviceLayers{};
		std::vector<const char*> deviceExtensions{};
		void enableLayersAndExtensions();
		static std::vector<char> readFile(const std::string& filename);

		// debug stuff
		vk::DebugReportCallbackEXT debug_report;
		vk::DebugReportCallbackCreateInfoEXT reportInfo;
		void allocateConsole();
		void freeConsole();
		void setUpDebugCallback();
		void destroyDebugCallback();
#ifdef _DEBUG
		static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallBack(
			VkDebugReportFlagsEXT /*flags*/,
			VkDebugReportObjectTypeEXT /*objType*/,
			uint64_t /*obj*/,
			size_t /*location*/,
			int32_t /*code*/,
			const char* layerPrefix,
			const char* msg,
			void* /*userData*/);
		static void E_vkCreateDebugReportCallbackEXT(
			vk::Instance& instance,
			vk::DebugReportCallbackCreateInfoEXT* createInfo,
			vk::AllocationCallbacks* allocator,
			vk::DebugReportCallbackEXT* callback);
		static void E_DestroyDebugReportCallbackEXT(
			vk::Instance& instance,
			vk::DebugReportCallbackEXT* callback,
			vk::AllocationCallbacks* allocator);
#endif
	};
}