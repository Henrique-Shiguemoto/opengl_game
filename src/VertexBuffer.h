#pragma once

#include <glad/glad.h>

#include "main.h"

class VertexBuffer{
public:
	u32 id;
	VertexBuffer() = default;
	VertexBuffer(f32* data, u32 sizeInBytes);
	void SetupVertexBuffer(f32* data, u32 sizeInBytes);
	void Bind();
	void Unbind();
	void Delete();
};