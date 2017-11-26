#pragma once
#include <map>
#include "Texture.h"
#include "Rect.h"
#include "Vulkan_.h"
#include "Vertex.h"
#include "Box2D\Box2D.h"

namespace vm {
	struct ShapedBuffers {
		vk::Buffer				vertexBuffer;
		vk::DeviceMemory		vertexBufferMem;
		vk::Buffer				indexBuffer;
		vk::DeviceMemory		indexBufferMem;

		std::vector<Vertex>		vertices;
		std::vector<uint32_t>	indices;
		bool					occupied = false;
	};

	class ResourceManager { //ResourceManager &rm = ResourceManager::getInstance();
	public:
		static ResourceManager& getInstance() {
			static ResourceManager singleton;
			return singleton;
		}

		// accessable singleton variables
		b2World							*world;
		b2Body							*groundBody;
		Rect							groundRect;
		vk::Buffer						spritesVertexBuffer;
		vk::Buffer						spritesIndexBuffer;
		vk::Buffer						spritesUniformBuffer;
		vk::Buffer						pointLightsUniformBuffer;
		vk::DeviceMemory				spritesVertexBufferMem;
		vk::DeviceMemory				spritesIndexBufferMem;
		vk::DeviceMemory				spritesUniformBufferMem;
		vk::DeviceMemory				pointLightsUniformBufferMem;
		vk::DescriptorSet				spritesDescriptorSet;
		vk::DescriptorSet				playerDescriptorSet;
		vk::DescriptorSet				pointLightsDescriptorSet;
		void							*spritesUniformData;
		void							*pointLightsUniformData;

		std::map<std::string, Texture>	textures;
		std::vector<Rect>				definedRects{};
		std::vector<ShapedBuffers>		userShapedBuffers{};
		vk::DescriptorSetLayout         cameraDescriptorSetLayout;
		vk::DescriptorSetLayout			spritesDescriptorSetLayout;
		vk::DescriptorSetLayout			pointLightsDescriptorSetLayout;
		vk::Sampler                     spriteSampler;
		void setUpCameraDescriptorSetLayout();
		void setUpSpriteDescriptorSetLayout();
		void setUpPointLightsDescriptorSetLayout();
		
		int createUserDefinedBuffers(Rect &definedRect, bool reverseY = false);
		void init(vk::PhysicalDevice& pGpu, vk::Device& pDevice, vk::CommandPool& pCommandPool, vk::Queue& pGraphicsQueue, vk::PhysicalDeviceProperties& pGpuProperties, vk::Extent2D& pSChainExt2D);
		void deInit();

		//std::vector<Rect> getRectsFromImage(std::string location, bool flip_vertically_on_load = true) const;

		vk::PhysicalDevice& getGpu();
		vk::Device& getDevice();
		vk::CommandPool& getCommandPool();
		vk::Queue& getGraphicsQueue();
		vk::PhysicalDeviceProperties& getGpuProperties();
		vk::Extent2D& getSwapChainExtend2D();

	private:
		vk::PhysicalDevice				pGpu;
		vk::Device						pDevice;
		vk::CommandPool					pCommandPool;
		vk::Queue						pGraphicsQueue;
		vk::PhysicalDeviceProperties	pGpuProperties;
		vk::Extent2D					pSChainExt2D;
		bool							resourceManagerInitialized = false;
		Helper							helper;

		void createSpriteSampler(); // one immutable sampler
		void createBuffers(ShapedBuffers &sBufffer, vk::PhysicalDevice*gpu, vk::Device *device, vk::CommandPool *commandPool, vk::Queue *graphicsQueue);

		ResourceManager() {}; // ctor hidden
		ResourceManager(ResourceManager const&) {}; // copy ctor hidden
		ResourceManager& operator=(ResourceManager const&) {}; // assign op. hidden
		~ResourceManager() {}; // dtor hidden
	};
}