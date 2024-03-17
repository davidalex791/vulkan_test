#include "FirstApp.hpp"
#include <stdexcept>
#include <memory>
#include <chrono>
#include <array>
#include "Lve_Camera.hpp"
#include "systems/Simple_Render_System.hpp"
#include "systems/Point_Light_System.hpp"
#include "Keyboard_Movement_Controller.hpp"
#include "Lve_Buffer.hpp"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace lve
{



	FirstApp::FirstApp(const std::string& path) : shadersPath{ path }
	{
		globalPool = LveDescriptorPool::Builder(lveDevice)
			.setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

		loadGameObjects(path + "..\\..\\models\\");
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
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
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
		PointLightSystem pointLightSystem(lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout(), shadersPath);


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
					globalDescriptorSets[frameIndex],
					gameObjects
				};
				//update
				GlobalUbo ubo{};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				ubo.inverseView = camera.getInverseView();
				pointLightSystem.update(frameInfo, ubo);
				uboBuffers[frameIndex]->writeToIndex(&ubo, frameIndex);
				uboBuffers[frameIndex]->flush();

				//render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);

				//order here matters (solid objects then transparent objects)
				simpleRendererSystem.render(frameInfo);
				pointLightSystem.render(frameInfo);

				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device()); //await finish commands
	}

	void FirstApp::loadGameObjects(const std::string &modelsPath)
	{
		std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, modelsPath + "flat_vase.obj.txt");
		auto flatVase = LveGameObject::createGamerObject();
		flatVase.model = lveModel;
		flatVase.transform.translation = { -.5f, .5f, .0f };
		flatVase.transform.scale = { 3.f, 1.5f, 3.f }; // glm::vec3(3.f);
		gameObjects.emplace(flatVase.getId(), std::move(flatVase));

		lveModel = LveModel::createModelFromFile(lveDevice, modelsPath + "smooth_vase.obj.txt");
		auto smoothVase = LveGameObject::createGamerObject();
		smoothVase.model = lveModel;
		smoothVase.transform.translation = { .5f, .5f, 0.f };
		smoothVase.transform.scale = { 3.f, 1.5f, 3.f }; // glm::vec3(3.f);
		gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));


		lveModel = LveModel::createModelFromFile(lveDevice, modelsPath + "quad.obj.txt");
		auto floorObj = LveGameObject::createGamerObject();
		floorObj.model = lveModel;
		floorObj.transform.translation = { 0.f, .5f, 0.f };
		floorObj.transform.scale = { 3.f, 1.f, 3.f };
		gameObjects.emplace(floorObj.getId(), std::move(floorObj));


		std::vector<glm::vec3> lightColors{
			 {1.f, .1f, .1f},
			 {.1f, .1f, 1.f},
			 {.1f, 1.f, .1f},
			 {1.f, 1.f, .1f},
			 {.1f, 1.f, 1.f},
			 {1.f, 1.f, 1.f}  //
		};
		for (int i = 0; i < lightColors.size(); i++)
		{
			auto pointLight = LveGameObject::makePointLight(0.2f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(
				glm::mat4(1.f),
				(i * glm::two_pi<float>()) / lightColors.size(),
				{ 0.f, -1.f, 0.f });
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
			gameObjects.emplace(pointLight.getId(), std::move(pointLight));

		}
	}


} //namespace lve