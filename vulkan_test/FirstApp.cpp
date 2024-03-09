#include "FirstApp.hpp"
#include <stdexcept>
#include <memory>
#include <chrono>
#include <array>
#include "Lve_Camera.hpp"
#include "Simple_Render_System.hpp"
#include "Keyboard_Movement_Controller.hpp"
#include "Lve_Buffer.hpp"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace lve
{
	struct GlobalUbo //buffer object structures
	{
		glm::mat4 projectionView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f }; //w is intensity
		glm::vec3 lightPosition{ -1.f };
		//uint32 padding;
		alignas(16)  glm::mat4 lightColor{ 1.f }; //w is light intensity (r, g,  b, intensity) => (r x i, g x i, b x i)
		//glm::vec3 pointLight;
		// ...
	};


	FirstApp::FirstApp(const std::string& path) : shadersPath{ path }
	{
		globalPool = LveDescriptorPool::Builder(lveDevice)
			.setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<LveBuffer>(
				lveDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}
		auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			LveDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRendererSystem(lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout(), shadersPath);
		LveCamera camera{};
		camera.setViewTarget(glm::vec3{ -1.f, -2.f, 2.f }, glm::vec3{ 0.0f, 0.f, 2.5f });

		auto viewerObject = LveGameObject::createGamerObject();
		viewerObject.transform.translation.z = -2.5f;

		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			frameTime = glm::min(frameTime, MAX_FRAME_TIME); 

			cameraController.moveInPlaneXZ(lveWindow.gwtGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = lveRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f); //clipping plane

			//drawFrame
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				int frameIndex = lveRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex]
				};
				//update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToIndex(&ubo, frameIndex);
				uboBuffers[frameIndex]->flush();

				//render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRendererSystem.renderGameObjects(frameInfo, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device()); //await finish commands
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, shadersPath + "..\\..\\models\\flat_vase.obj.txt");
		auto flatVase = LveGameObject::createGamerObject();
		flatVase.model = lveModel;
		flatVase.transform.translation = { -.5f, .5f, .0f };
		flatVase.transform.scale = { 3.f, 1.5f, 3.f }; // glm::vec3(3.f);

		gameObjects.push_back(std::move(flatVase));

		lveModel = LveModel::createModelFromFile(lveDevice, shadersPath + "..\\..\\models\\smooth_vase.obj.txt");
		auto smoothVase = LveGameObject::createGamerObject();
		smoothVase.model = lveModel;
		smoothVase.transform.translation = { .5f, .5f, 0.f };
		smoothVase.transform.scale = { 3.f, 1.5f, 3.f }; // glm::vec3(3.f);

		gameObjects.push_back(std::move(smoothVase));


		lveModel = LveModel::createModelFromFile(lveDevice, shadersPath + "..\\..\\models\\quad.obj.txt");
		auto floor = LveGameObject::createGamerObject();
		floor.model = lveModel;
		floor.transform.translation = { 0.f, .5f, 0.f };
		floor.transform.scale = { 3.f, 1.f, 3.f }; 

		gameObjects.push_back(std::move(floor));



	}


} //namespace lve