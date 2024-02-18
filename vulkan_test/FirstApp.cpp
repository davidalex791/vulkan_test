#include "FirstApp.hpp"
#include <stdexcept>
#include <memory>
#include <array>
#include "Simple_Render_System.hpp"

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

		while (!lveWindow.ShouldClose())
		{
			glfwPollEvents();

			//drawFrame
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				//begin offscreen shadow pass
				//rander shadow casting objects
				//end offscreen shadow pass

				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRendererSystem.renderGameObjects(commandBuffer, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device()); //await finish commands
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<LveModel::Vertex> vertices 
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);
		auto triangle = LveGameObject::createGamerObject();
		triangle.model = lveModel;
		triangle.color = { .1f,.8f,.1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}


} //namespace lve