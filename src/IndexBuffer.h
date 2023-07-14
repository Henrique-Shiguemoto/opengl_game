#pragma once

#include <glad/glad.h>

#include "main.h"

class IndexBuffer{
public:
	u32 id;
	IndexBuffer(u32* data, u32 sizeInBytes);
	void Bind();
	void Unbind();
	void Delete();
};