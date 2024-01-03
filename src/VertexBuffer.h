#pragma once

#include <glad/glad.h>

#include "types.h"

class VertexBuffer{
public:
	u32 id;
	VertexBuffer(f32* data, u32 sizeInBytes);
	void Bind();
	void Unbind();
	void Delete();
};