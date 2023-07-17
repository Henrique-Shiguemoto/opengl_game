#pragma once

#include <iostream>
#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "main.h"

class Game{
public:
	Game(const char* name, i32 windowWidth, i32 windowHeight, const char* vertexShaderFilepath, const char* fragmentShaderFilepath, const char* windowIconFilepath);
	void HandleInput();
	void SimulateWorld();
	void RenderGraphics();
	void UpdateFrametimeInWindowTitle();
	void Quit();

public:
	b8 isValid							= false;
	std::string name 					= "Custom Game Name";
	i32 windowWidth 					= 800;
	i32 windowHeight 					= 600;
	SDL_Window* window 					= nullptr;
	SDL_GLContext context 				= NULL;
	PerformanceData performanceData;
	b8 isRunning 						= false;
	b8 isFullscreen 					= false;
	b8 isVSyncEnabled 					= true;
	Shader* shader 						= nullptr;
	VertexArray* vao 					= nullptr;
	VertexBuffer* vbo 					= nullptr;
	IndexBuffer* ibo 					= nullptr;
};