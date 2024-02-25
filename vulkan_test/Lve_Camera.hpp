#pragma once

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve
{
	class LveCamera
	{
	public:
																												 //	   -y               +--------+ (right, top, far)
																												 //     | +z           /        /|        
																												 //     |/            +--------+ |
		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far); //[-x--|--+x]        |        | /     
																												 //   / |             |________|/
                                                                                                                 // -z  |           (left, bottom, near)   
																												 //	   +y
		void setPerspectiveProjection(float fovy, float aspect, float near, float far);
		const glm::mat4 &getProjection() const { return projectionMatrix; }

	private:
		glm::mat4 projectionMatrix{ 1.f };
	};

}//namespace lve