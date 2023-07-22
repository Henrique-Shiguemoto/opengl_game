#pragma once

#include <glm/glm.hpp>
#include "main.h"

struct Camera{
	glm::vec3 cameraPosition_f	= glm::vec3(0.0f, 5.0f, 10.0f);
	glm::vec3 cameraFront_f 	= glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp_f		= glm::vec3(0.0f, 1.0f, 0.0f);
	f32 cameraMaximumSpeed 		= 2.5f;
	f32 fovDegrees 				= 45.0f;
	f32 nearClipDistance 		= 0.1f;
	f32 farClipDistance 		= 100.0f;
	b8 cameraHasToMoveLeft 		= false;
	b8 cameraHasToMoveRight 	= false;
	b8 cameraHasToMoveFront 	= false;
	b8 cameraHasToMoveBack 		= false;
	f32 cameraYaw 				= -90.0f;
	f32 cameraPitch				= 0.0f;
};