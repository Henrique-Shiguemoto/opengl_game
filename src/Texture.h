#pragma once

#include "glad/glad.h"
#include "main.h"
#include <iostream>

class Texture{
public:
	u32 id;
	i32 slot;

	Texture(const char* filepath, i32 filterMode, GLenum pixelFormat, i32 slot, b8 generateMipmap);
	void Activate();
	void Bind();
	void Unbind();
	void Delete();
};