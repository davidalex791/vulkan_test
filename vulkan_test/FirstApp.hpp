#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Lve_Window.hpp"
#include "Lve_Device.hpp"
#include "Lve_GameObject.hpp"
#include "Lve_Rederer.hpp"

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 1024;
		static constexpr int HEIGHT = 768;
		static constexpr float MAX_FRAME_TIME = 0.5f;//TODO what should be this limit?

		FirstApp(const std::string& path);
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();

		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		LveRenderer lveRenderer{ lveWindow, lveDevice };
		std::vector<LveGameObject> gameObjects;

		std::string shadersPath;
	};
}