#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Map{
public:
	Map(glm::vec3 pos, glm::vec3 dimension);

	glm::vec3 pos;
	glm::vec3 dimension;
	glm::vec3 point_from_map;
	VertexArray* vaoMap = nullptr;
	VertexBuffer* vboMap = nullptr;
	IndexBuffer* iboMap = nullptr;
};