#pragma once

#include <glm/glm.hpp>
#include "main.h"

struct Camera{
	glm::vec3 position_f  = glm::vec3(-9.5f, 10.5f, 15.5f);
	glm::vec3 front_f     = glm::vec3(0.0f, -1.5f, -1.0f);
	glm::vec3 up_f        = glm::vec3(0.0f, 1.0f, 0.0f);
	f32 maximumSpeed      = 2.5f;
	f32 fovDegrees        = 45.0f;
	f32 nearClipDistance  = 0.1f;
	f32 farClipDistance   = 100.0f;
	b8 hasToMoveLeft      = false;
	b8 hasToMoveRight     = false;
	b8 hasToMoveFront     = false;
	b8 hasToMoveBack      = false;
	f32 yaw               = -90.0f;
	f32 pitch             = 0.0f;
};