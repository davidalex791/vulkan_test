#include "FirstApp.hpp"
#include <stdexcept>
#include <memory>
#include <chrono>
#include <array>
#include "Lve_Camera.hpp"
#include "Simple_Render_System.hpp"
#include "Keyboard_Movement_Controller.hpp"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace lve
{
	FirstApp::FirstApp(const std::string& path) : shadersPath{ path }
	{
		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		SimpleRenderSystem simpleRendererSystem(lveDevice, lveRenderer.getSwapChainRenderPass(), shadersPath);
		LveCamera camera{};
		camera.setViewTarget(glm::vec3{ -1.f, -2.f, 2.f }, glm::vec3{ 0.0f, 0.f, 2.5f });

		auto viewerObject = LveGameObject::createGamerObject();
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
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			//drawFrame
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				//begin offscreen shadow pass
				//rander shadow casting objects
				//end offscreen shadow pass

				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRendererSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device()); //await finish commands
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, shadersPath + "..\\..\\models\\smooth_vase.obj.txt");
		auto gameObj = LveGameObject::createGamerObject();
		gameObj.model = lveModel;
		gameObj.transform.translation = { .0f, .0f, 2.5f };
		gameObj.transform.scale = glm::vec3(3.f);

		gameObjects.push_back(std::move(gameObj));

	}


} //namespace lve