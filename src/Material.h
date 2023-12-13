#pragma once

struct Material{
	glm::vec3 ambient = glm::vec3(1.0f);
	int diffuse = 0;
	glm::vec3 specular = glm::vec3(1.0f);
	float shininess = 1.0f;
};