#pragma once

#include <glad/glad.h>

#include "main.h"
#include "VertexBuffer.h"

class VertexArray{
public:
	u32 id;
	VertexArray();
	void DefineVBOLayout(VertexArray vbo);
	void Bind();
	void Unbind();
	void Delete();
};