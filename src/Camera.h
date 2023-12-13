#pragma once

#include <glm/glm.hpp>
#include "main.h"

#include <glm/gtc/matrix_transform.hpp>

struct Camera{
	glm::vec3 position_f  	= glm::vec3(-11.0f, 3.0f, 13.1f);
	glm::vec3 front_f     	= glm::vec3(0.0f, -1.2f, -1.0f);
	glm::vec3 worldUp_f   	= glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 up_f   		= glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right_f   	= glm::normalize(glm::cross(front_f, up_f));
	f32 maximumSpeed      	= 2.5f;
	f32 fovDegrees        	= 45.0f;
	f32 nearClipDistance  	= 0.1f;
	f32 farClipDistance   	= 100.0f;
	b8 hasToMoveLeft      	= false;
	b8 hasToMoveRight     	= false;
	b8 hasToMoveUp  	  	= false;
	b8 hasToMoveDown      	= false;
	b8 hasToMoveFront  	  	= false;
	b8 hasToMoveBack      	= false;
	b8 toggleFPS  			= false;
	f32 yaw               	= -90.0f;
	f32 pitch             	= 0.0f;
	f32 mouseSensitivity 	= 0.2f;

public:
	glm::mat4 GetViewMatrix();
	void ProcessMouseMovement(f32 xoffset, f32 yoffset);
	void ProcessMouseScroll(f32 yoffset);

private:
	void updateCameraVectors();
};
