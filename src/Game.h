#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

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
	b8 isValid									= false;
	std::string name 							= "Custom Game Name";
	i32 windowWidth 							= 800;
	i32 windowHeight 							= 600;
	SDL_Window* window 							= nullptr;
	SDL_GLContext context 						= NULL;
	PerformanceData performanceData;
	b8 isRunning 								= false;
	b8 isFullscreen 							= false;
	b8 isVSyncEnabled 							= true;
	Shader* shader 								= nullptr;
	VertexArray* vao 							= nullptr;
	VertexBuffer* vbo 							= nullptr;
	IndexBuffer* ibo 							= nullptr;

	// (X, Y) (not in [-1.0, 1.0f] range, 
	//			it's in [0, windowWidth] and 
	// 					[0, windowHeight] range)
	i32 lastMouseClickPosition[2] 				= {0, 0};
	
	// (X, Y) (in [-1.0, 1.0f] range)
	f32 lastMouseClickPositionNormalized[2] 	= {0.0f, 0.0f};

	// (X, Y, Z) (in [-1.0, 1.0f] range)
	glm::vec3 playerPosition_f 					= glm::vec3(-0.1f, -0.1f, 0.0f);
	glm::vec3 playerVelocity_f 					= glm::vec3(0.0f, 0.0f, 0.0f);
	f32 playerMaximumSpeed 						= 0.005f;
	b8 playerStartedMoving 						= false;
	b8 playerIsMoving 							= false;
	f32 distanceForPlayerToStopMoving 			= 0.005f;

private:
	void PrintMouseClickPosition();
	void PrintPlayerPosition();
	void PrintPlayerVelocity();
};