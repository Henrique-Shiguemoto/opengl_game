#pragma once

#include <glm/glm.hpp>
#include "types.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Player{
public:
	Player(glm::vec3 pos, glm::vec3 dimension, f32 maxSpeed);

	glm::vec3 pos;
	glm::vec3 dimension;
	glm::vec3 vel = glm::vec3(0.0f);
	glm::vec3 locationPlayerHasToGo;
	f32 maxSpeed;
	b8 startedMoving = false;
	b8 isMoving = false;
	VertexArray* vaoPlayer = nullptr;
	VertexBuffer* vboPlayer = nullptr;
	IndexBuffer* iboPlayer = nullptr;
};