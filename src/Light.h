#pragma once

#include <glm/glm.hpp>
#include "types.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Light {
public:
	Light(glm::vec3 pos, glm::vec3 dimension, glm::vec3 color, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular);

	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;
	glm::vec3 dimension;
	b8 hasToMoveLeft = false;
	b8 hasToMoveRight = false;
	b8 hasToMoveUp = false;
	b8 hasToMoveDown = false;
	b8 hasToMoveForward = false;
	b8 hasToMoveBack = false;

	VertexArray* vaoLight = nullptr;
	VertexBuffer* vboLight = nullptr;
	IndexBuffer* iboLight = nullptr;
};