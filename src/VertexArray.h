#pragma once

#include <glad/glad.h>

#include "main.h"
#include "VertexBuffer.h"

class VertexArray{
public:
	u32 id;
	VertexArray();
	void DefineVBOLayout(VertexBuffer& vbo, u32 layoutIndex, i32 sizeInComponents, u32 strideInBytes, u32 offsetInComponents);
	void Bind();
	void Unbind();
	void Delete();
};