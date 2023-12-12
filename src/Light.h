#pragma once

#include <glm/glm.hpp>

struct Light {
	glm::vec3 		lightPos 						= glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 		lightColor 						= glm::vec3(1.0f, 1.0f, 1.0f);
	b8 				lightHasToMoveLeft		      	= false;
	b8 				lightHasToMoveRight		     	= false;
	b8 				lightHasToMoveUp		  	  	= false;
	b8 				lightHasToMoveDown		      	= false;
	b8 				lightHasToMoveForward 			= false;
	b8 				lightHasToMoveBack 				= false;
};